#include <Physics/Public/PhysicsWorld.hpp>
#include <Physics/Public/RigidBody.hpp>
#include <Physics/Public/BulletTypes.hpp>
#include <Physics/Public/PhysicsDebugDraw.hpp>
#include <Physics/Public/Constraint.hpp>
#include <GameFramework/Public/ECS/GameObject.hpp>

struct PhysicsQueryCallback : public btCollisionWorld::ContactResultCallback {
  Array<RigidBody*>& result;
  UInt32 collisionMask;

  PhysicsQueryCallback(Array<RigidBody*>& res, unsigned collision_mask)
      : result{res}, collisionMask{collision_mask}
  { }

  // Add a contact result.
  btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0,
                           const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) override
  {
    auto* body = reinterpret_cast<RigidBody*>(colObj0Wrap->getCollisionObject()->getUserPointer());
    if (body && !result.contains(body) && (body->collisionLayer() & collisionMask))
      result.push_back(body);
    body = reinterpret_cast<RigidBody*>(colObj1Wrap->getCollisionObject()->getUserPointer());
    if (body && !result.contains(body) && (body->collisionLayer() & collisionMask))
      result.push_back(body);

    return 0.0f;
  }

};

void _internal_pre_tick_callback(btDynamicsWorld* world, btScalar timeStep)
{
  static_cast<PhysicsWorld*>(world->getWorldUserInfo())->preStep(timeStep);
}

void _internal_tick_callback(btDynamicsWorld* world, btScalar timeStep)
{
  static_cast<PhysicsWorld*>(world->getWorldUserInfo())->postStep(timeStep);
}

PhysicsWorld::PhysicsWorld()
  : m_debug_drawer{nullptr}, m_configuration{nullptr}, m_collision_dispatcher{nullptr},
    m_broadphase{nullptr}, m_constraint_solver{nullptr}, m_dynamics_world{nullptr},
    m_rigid_bodies{nullptr}, m_collision_shapes{nullptr}, m_constraints{nullptr},
    m_curr_collisions{}, m_prev_collisions{}
{
  m_configuration = makeUnique<PhysicsWorldConfiguration>();
  m_configuration->collisionConfiguration = new btDefaultCollisionConfiguration;
  m_collision_dispatcher = makeUnique<btCollisionDispatcher>(m_configuration->collisionConfiguration);
  m_broadphase = makeUnique<btDbvtBroadphase>();
  m_constraint_solver = makeUnique<btSequentialImpulseConstraintSolver>();
  m_dynamics_world = makeUnique<btDiscreteDynamicsWorld>(m_collision_dispatcher.get(),
      m_broadphase.get(), m_constraint_solver.get(), m_configuration->collisionConfiguration);
  m_dynamics_world->setGravity(Math::toBtVec3(kDefaultGravity));
  m_dynamics_world->getDispatchInfo().m_useContinuous = true;
  m_dynamics_world->getSolverInfo().m_splitImpulse = false; // disable by defautl for performance
  m_dynamics_world->setInternalTickCallback(_internal_pre_tick_callback, static_cast<void*>(this), true);
  m_dynamics_world->setInternalTickCallback(_internal_tick_callback, static_cast<void*>(this), false);
  m_dynamics_world->setSynchronizeAllMotionStates(true);
}

PhysicsWorld::~PhysicsWorld()
{
  for (int i = 0; i < m_dynamics_world->getCollisionObjectArray().size(); ++i)
    m_dynamics_world->removeCollisionObject(m_dynamics_world->getCollisionObjectArray()[i]);

  m_dynamics_world.reset(nullptr);
  m_constraint_solver.reset(nullptr);
  m_broadphase.reset(nullptr);
  m_collision_dispatcher.reset(nullptr);
  delete m_configuration->collisionConfiguration;
  m_configuration.reset(nullptr);
}

btDiscreteDynamicsWorld* PhysicsWorld::world() const
{
  return m_dynamics_world.get();
}

void PhysicsWorld::setGravity(const Vec3& gravity)
{
  m_dynamics_world->setGravity(Math::toBtVec3(gravity));
}

Vec3 PhysicsWorld::gravity() const
{
  return Math::fromBtVec3(m_dynamics_world->getGravity());
}

void PhysicsWorld::update(Real delta)
{
  m_dynamics_world->stepSimulation(delta);
}

void PhysicsWorld::updateCollisions()
{
  m_dynamics_world->performDiscreteCollisionDetection();
}

void PhysicsWorld::setConstraintSolverIterations(int num)
{
  m_dynamics_world->getSolverInfo().m_numIterations = Math::clamp(num, 1, kMaxSolverIterations);
}

Int32 PhysicsWorld::constraintSolverIterations() const
{
  return m_dynamics_world->getSolverInfo().m_numIterations;
}

void PhysicsWorld::setIsSplitImpulseModeEnabled(bool enable)
{
  m_dynamics_world->getSolverInfo().m_splitImpulse = enable;
}

bool PhysicsWorld::isSplitImpulseModeEnabled() const
{
  return m_dynamics_world->getSolverInfo().m_splitImpulse;
}

Array<RigidBody*> PhysicsWorld::collidingRigidBodies(const RigidBody* body)
{
  Array<RigidBody*> out;
  for (HashMap<Pair<RigidBody*, RigidBody*>, ManifoldPair>::iterator i = m_curr_collisions.begin();
      i != m_curr_collisions.end(); ++i) {
    if (i.key().first == body) {

      if (i.key().second) {
        out.push_back(i.key().second);
      }

    } else if (i.key().second == body) {

      if (i.key().first) {
        out.push_back(i.key().first);
      }

    }
  }
  return out;
}

void PhysicsWorld::addRigidBody(RigidBody* body)
{
  if (!m_rigid_bodies.contains(body)) {
    m_rigid_bodies.push_back(body);
    m_dynamics_world->addRigidBody(body->body(), body->collisionLayer(), body->collisionMask());
    body->setIsInWorld(true);
  }
}

void PhysicsWorld::removeRigidBody(RigidBody* body)
{
  if (m_rigid_bodies.contains(body)) {
    m_rigid_bodies.removeOne(body);
    m_dynamics_world->removeRigidBody(body->body());
    body->setIsInWorld(false);
  }
}

void PhysicsWorld::addCollisionShape(CollisionShape* shape)
{
  if (!m_collision_shapes.contains(shape)) {
    m_collision_shapes.push_back(shape);
  }
}

void PhysicsWorld::removeCollisionShape(CollisionShape* shape)
{
  if (!m_collision_shapes.contains(shape)) {
    m_collision_shapes.removeOne(shape);
  }
}

void PhysicsWorld::addConstraint(Constraint* constraint)
{
  if (!m_constraints.contains(constraint)) {
    m_constraints.push_back(constraint);
    m_dynamics_world->addConstraint(constraint->constraint());
  }
}

void PhysicsWorld::removeConstraint(Constraint* constraint)
{
  if (m_constraints.contains(constraint)) {
    m_constraints.removeOne(constraint);
    m_dynamics_world->removeConstraint(constraint->constraint());
  }
}

void PhysicsWorld::preStep(Real delta)
{

}

void PhysicsWorld::postStep(Real delta)
{
  m_curr_collisions.clear();

  int num_manifolds = m_collision_dispatcher->getNumManifolds();

  if (num_manifolds > 0) {
    for (int i = 0; i < num_manifolds; ++i) {
      btPersistentManifold* contact_manifold = m_collision_dispatcher->getManifoldByIndexInternal(i);

      // First check that there are actual contacts, as the manifold exists
      // also when objects are close but not touching
      if (!contact_manifold->getNumContacts())
        continue;

      const btCollisionObject* obj_0 = contact_manifold->getBody0();
      const btCollisionObject* obj_1 = contact_manifold->getBody1();

      auto* body0 = static_cast<RigidBody*>(obj_0->getUserPointer());
      auto* body1 = static_cast<RigidBody*>(obj_1->getUserPointer());

      if (!body0 || !body1)
        continue;

      // Skip collision event signaling if both objects are static,
      // or if collision event mode does not match
      if (body0->mass() == 0.0f && body1->mass() == 0.0f)
        continue;

      //qDebug() << "F_____________k : " << body0->gameObject()->name() << "and" << body1->gameObject()->name();

      Pair<RigidBody*, RigidBody*> body_pair;
      if (body0 < body1) {
        body_pair = qMakePair(body0, body1);
        m_curr_collisions[body_pair].m_manifold = contact_manifold;
      } else {
        body_pair = qMakePair(body1, body0);
        m_curr_collisions[body_pair].m_manifold = contact_manifold;
      }
    }

    m_prev_collisions = m_curr_collisions;
  }
}

PhysicsDebugDraw* PhysicsWorld::debugDrawer() const
{
  return m_debug_drawer;
}

void PhysicsWorld::setDebugDrawer(PhysicsDebugDraw* debugDrawer)
{
  m_debug_drawer = debugDrawer;
  m_dynamics_world->setDebugDrawer(m_debug_drawer);
}

