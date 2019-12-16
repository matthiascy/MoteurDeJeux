#include <GameFramework/Components/Colliders.hpp>
#include <GameFramework/Systems/PhysicsSystem.hpp>
#include <GameFramework/GameObject.hpp>
#include <GameFramework/Engine.hpp>
#include <GameFramework/Managers/SceneManager.hpp>
#include <GameFramework/Components/Collider.hpp>


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
}

void PhysicsSystem::preUpdate(Real dt)
{
  // TODO better way to handle colliders type (used for type id)
  // Add the body to the dynamics world
  auto* scene = m_engine->sceneManager()->sceneAt(m_engine->sceneManager()->activatedScene());
  if (scene) {
    for (GameObject* object : scene->gameObjects()) {
      if (object->isSimulated() && !object->isStatic()) {
        qDebug() << "has collider " << object->hasComponent<Collider>();
        if (object->hasCollider()) {
          auto colliders = object->getComponentsOfType<Collider>();
          qDebug() << colliders.size();
          for (auto* collider : colliders) {
            auto* rigid_body = collider->rigidBody();
            if (m_world->getCollisionObjectArray().findLinearSearch(rigid_body) ==
                m_world->getCollisionObjectArray().size()) {
              qDebug() << "Add one collider";
              m_world->addRigidBody(rigid_body);
            }
          }
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
