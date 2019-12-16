#include <GameFramework/Components/Colliders.hpp>
#include <GameFramework/Systems/PhysicsSystem.hpp>
#include <GameFramework/GameObject.hpp>
#include <GameFramework/Engine.hpp>
#include <GameFramework/Managers/SceneManager.hpp>
#include <GameFramework/Components/Collider.hpp>

/**
 * Add one rigid body to the bullet dynamics world to do the simulation.
 * @tparam T Collider
 * @tparam U BoxCollider, CapsuleCollider, ConeCollider, CylinderCollider ...
 */
template <template <typename> class T, typename U,
    typename = std::enable_if_t<std::is_base_of_v<Collider<U>, T<U>>>>
void _add_rigid_body_to_world(btDiscreteDynamicsWorld* world, T<U>* collider)
{
  auto* rigid_body = collider->rigidBody();
  if (world->getCollisionObjectArray().findLinearSearch(rigid_body) == world->getCollisionObjectArray().size()) {
    qDebug() << "Add one collider";
    world->addRigidBody(rigid_body);
  }
}

PhysicsSystem::PhysicsSystem(const String& name, Engine* engine, Object* parent, const Vec3& gravity)
  : System(name, engine, parent),
    m_config{nullptr}, m_dispatcher{nullptr},
    m_broad_phase{nullptr}, m_solver{nullptr},
    m_world{nullptr}, m_gravity{gravity}
{ }

void PhysicsSystem::init()
{
  qInfo() << "Initialization...";
  m_config = new btDefaultCollisionConfiguration();
  m_dispatcher = new btCollisionDispatcher(m_config);
  m_broad_phase = new btDbvtBroadphase();
  m_solver = new btSequentialImpulseConstraintSolver();
  m_world = new btDiscreteDynamicsWorld(m_dispatcher, m_broad_phase, m_solver, m_config);
  m_world->setGravity(Math::toBtVec3(m_gravity));
}

PhysicsSystem::~PhysicsSystem()
{
  for (int i = m_collision_objects.size() - 1; i >= 0; --i)
  {
    btCollisionObject* obj = m_collision_objects[i];
    btRigidBody* body = btRigidBody::upcast(obj);
    if (body && body->getMotionState())
    {
      delete body->getMotionState();
    }
    m_world->removeCollisionObject(obj);
    delete obj;
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
  m_world->stepSimulation(dt, 10);
  m_collision_objects = m_world->getCollisionObjectArray();

  for (int i = 0; i < m_collision_objects.size(); ++i) {
    btCollisionObject* obj = m_world->getCollisionObjectArray()[i];
    btRigidBody* body = btRigidBody::upcast(obj);
    btTransform trans;

    if (body && body->getMotionState()) {

      body->getMotionState()->getWorldTransform(trans);

    } else {

      trans = obj->getWorldTransform();

    }
    printf("world pos object %d = %f,%f,%f\n", i, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
  }
  // TODO: obtein game object directly from rigid body
  auto* scene = m_engine->sceneManager()->sceneAt(m_engine->sceneManager()->activatedScene());
  if (scene) {
    for (GameObject* object : scene->gameObjects()) {
      auto* rigid_body = object->rigidBody();
      if (rigid_body) {
        object->transform()->setPosition(Math::fromBtVec3(rigid_body->get))
      }
    }
  }
}

void PhysicsSystem::preUpdate(Real dt)
{
  // TODO better way to handle colliders type (used for type id)
  // Add the body to the dynamics world
  auto* scene = m_engine->sceneManager()->sceneAt(m_engine->sceneManager()->activatedScene());
  if (scene) {
    for (GameObject* object : scene->gameObjects()) {
      if (object->isSimulated() && !object->isStatic()) {
        if (object->hasComponent<BoxCollider>()) {
          _add_rigid_body_to_world(m_world, object->getComponent<Collider<BoxCollider>>());
        }
        if (object->hasComponent<CapsuleCollider>()) {
          _add_rigid_body_to_world(m_world, object->getComponent<Collider<CapsuleCollider>>());
        }
        if (object->hasComponent<ConeCollider>()) {
          _add_rigid_body_to_world(m_world, object->getComponent<Collider<ConeCollider>>());
        }
        if (object->hasComponent<CylinderCollider>()) {
          _add_rigid_body_to_world(m_world, object->getComponent<Collider<CylinderCollider>>());
        }
        if (object->hasComponent<SphereCollider>()) {
          _add_rigid_body_to_world(m_world, object->getComponent<Collider<SphereCollider>>());
        }
        if (object->hasComponent<StaticPlaneCollider>()) {
          _add_rigid_body_to_world(m_world, object->getComponent<Collider<StaticPlaneCollider>>());
        }
      }
    }
  }
}

void PhysicsSystem::fixedUpdate(Real dt)
{
}

void PhysicsSystem::postUpdate(Real dt)
{
}
