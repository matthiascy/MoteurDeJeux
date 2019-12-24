#include <GameFramework/Systems/PhysicsSystem.hpp>
#include <GameFramework/GameObject.hpp>
#include <GameFramework/Engine.hpp>
#include <GameFramework/Managers/SceneManager.hpp>
#include <Physics/Public/RigidBody.hpp>
#include <Physics/Public/BulletTypes.hpp>
#include <Physics/Public/PhysicsDebugDraw.hpp>
#include <Physics/Public/PhysicsWorld.hpp>

// TODO: better debug draw

PhysicsSystem::PhysicsSystem(const String& name, Engine* engine, Object* parent, const Vec3& gravity)
  : System(name, engine, parent),
    m_world{nullptr}, m_gravity{gravity}, m_is_debug_draw_enabled{false}
{ }

void PhysicsSystem::init()
{
  qInfo() << "Initialization...";
  m_debug_drawer = makeUnique<PhysicsDebugDraw>(m_engine->renderSystem());
  m_world = makeUnique<PhysicsWorld>();
  m_world->setDebugDrawer(m_debug_drawer.get());
}

PhysicsSystem::~PhysicsSystem()
{
  m_world.reset(nullptr);
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
          if (m_world->world()->getCollisionObjectArray().findLinearSearch(rigid_body->body()) == m_world->world()->getCollisionObjectArray().size()) {
            m_world->addRigidBody(rigid_body);
          }
        }
      }
    }
  }
}

void PhysicsSystem::fixedUpdate(Real dt)
{
  m_world->update(dt);
  /*
  btCollisionObjectArray m_collision_objects = m_world->world()->getCollisionObjectArray();
  for (int i = 0; i < m_collision_objects.size(); ++i) {
  btTransform trans;
  auto* rigidBody = static_cast<RigidBody*>(btRigidBody::upcast(m_world->world()->getCollisionObjectArray()[i])->getUserPointer());
  rigidBody->getWorldTransform(trans);
  qDebug("world pos object %d = %f,%f,%f\n", i, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
  }
   */
  if (m_is_debug_draw_enabled)
    m_world->world()->debugDrawWorld();
}

void PhysicsSystem::postUpdate(Real dt)
{
  //m_debug_drawer->clearLines();
  m_world->debugDrawer()->clear();
}

PhysicsWorld* PhysicsSystem::physicsWorld() const
{
  return m_world.get();
}

PhysicsDebugDraw* PhysicsSystem::debugDrawer() const
{
  return m_world->debugDrawer();
}

bool PhysicsSystem::isDebugDrawEnabled() const
{
  return m_is_debug_draw_enabled;
}

void PhysicsSystem::setIsDebugDrawEnabled(bool enabled)
{
  m_is_debug_draw_enabled = enabled;
}
