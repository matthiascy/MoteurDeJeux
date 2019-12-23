#include <GameFramework/Systems/PhysicsSystem.hpp>
#include <GameFramework/GameObject.hpp>
#include <GameFramework/Engine.hpp>
#include <GameFramework/Managers/SceneManager.hpp>
#include <Physics/Public/RigidBody.hpp>
#include <Physics/Public/DebugDrawer.hpp>
#include <btBulletDynamicsCommon.h>

// TODO: better debug draw

PhysicsSystem::PhysicsSystem(const String& name, Engine* engine, Object* parent, const Vec3& gravity)
  : System(name, engine, parent),
    m_config{nullptr}, m_dispatcher{nullptr},
    m_broad_phase{nullptr}, m_solver{nullptr},
    m_world{nullptr}, m_gravity{gravity}
{ }

void PhysicsSystem::init()
{
  qInfo() << "Initialization...";
  m_config      = new btDefaultCollisionConfiguration();
  m_dispatcher  = new btCollisionDispatcher(m_config);
  m_broad_phase = new btDbvtBroadphase();
  m_solver      = new btSequentialImpulseConstraintSolver();
  m_world       = new btDiscreteDynamicsWorld(m_dispatcher, m_broad_phase, m_solver, m_config);
  m_debug_drawer = makeUnique<DebugDrawer>(m_engine->renderSystem());
  m_world->setGravity(Math::toBtVec3(m_gravity));
  m_world->setDebugDrawer(m_debug_drawer.get());
}

PhysicsSystem::~PhysicsSystem()
{
  for (int i = 0; i < m_world->getCollisionObjectArray().size(); ++i) {
    m_world->removeCollisionObject(m_world->getCollisionObjectArray()[i]);
  }

  delete m_world;
  delete m_solver;
  delete m_broad_phase;
  delete m_dispatcher;
  delete m_config;
}

void PhysicsSystem::setGravity(const Vec3& gravity)
{
  m_gravity = gravity;
  m_world->setGravity(Math::toBtVec3(gravity));
}

inline Vec3 PhysicsSystem::gravity() const
{
  return m_gravity;
}

void PhysicsSystem::update(Real dt)
{
}

void PhysicsSystem::preUpdate(Real dt)
{
  // TODO better way to handle colliders type (used for type id)
  // Add the body to the dynamics world
  auto* scene = m_engine->sceneManager()->sceneAt(m_engine->sceneManager()->activatedScene());
  if (scene) {
    for (GameObject* object : scene->gameObjects()) {
      if (object->isSimulated() && !object->isStatic()) {
        auto* rigid_body = object->getComponent<RigidBody>();
        if (rigid_body) {
          if (m_world->getCollisionObjectArray().findLinearSearch(rigid_body->body()) == m_world->getCollisionObjectArray().size()) {
            m_world->addRigidBody(rigid_body->body());
          }
        }
      }
    }
  }
}

void PhysicsSystem::fixedUpdate(Real dt)
{
  m_world->stepSimulation(dt, 10);
  m_collision_objects = m_world->getCollisionObjectArray();

  for (int i = 0; i < m_collision_objects.size(); ++i) {
    btTransform trans;
    auto* rigidBody = static_cast<RigidBody*>(btRigidBody::upcast(m_world->getCollisionObjectArray()[i])->getUserPointer());
    rigidBody->getWorldTransform(trans);
    //qDebug("world pos object %d = %f,%f,%f\n", i, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
    //Array<RigidBody*> bodies;
    //rigidBody->collidingBodies(bodies);
    //qDebug() << bodies.size();
    //m_world->debugDrawObject(trans, rigidBody->compoundShape(), {1.0, 0.5, 0.8});
  }
  m_world->debugDrawWorld();
}

void PhysicsSystem::postUpdate(Real dt)
{
  //m_debug_drawer->clearLines();
  m_debug_drawer->clear();
}

btDiscreteDynamicsWorld* PhysicsSystem::physicsWorld() const
{
  return m_world;
}

DebugDrawer* PhysicsSystem::debugDrawer() const
{
  return m_debug_drawer.get();
}
