#include "GameFramework/Systems/PhysicsSystem.hpp"
#include "GameFramework/GameObject.hpp"

// TODO:
PhysicsSystem::PhysicsSystem(const String& name, Engine* engine, Object* parent, const Vec3& gravity)
  : System(name, engine, parent)
{
  m_config = new btDefaultCollisionConfiguration();
  m_dispatcher = new btCollisionDispatcher(m_config);
  m_cache = new btDbvtBroadphase();
  m_solver = new btSequentialImpulseConstraintSolver;
  m_world = new btDiscreteDynamicsWorld(m_dispatcher, m_cache, m_solver, m_config);
  m_world->setGravity(btVector3(gravity.x(), gravity.y(), gravity.z()));
}

PhysicsSystem::~PhysicsSystem()
{
  delete m_config;
  delete m_dispatcher;
  delete m_cache;
  delete m_solver;
  delete m_world;
}

void PhysicsSystem::setGravity(const Vec3& gravity)
{
  m_world->setGravity(btVector3(gravity.x(), gravity.y(), gravity.z()));
}

inline Vec3 PhysicsSystem::gravity() const
{
  return Math::fromBTVec3(m_world->getGravity());
}

void PhysicsSystem::update(Real dt)
{
  m_world->stepSimulation(dt, 10);
  m_collision_objects = m_world->getCollisionObjectArray();
}

void PhysicsSystem::init()
{
  qInfo() << "\tPhysics system initialization...";
  // TODO
}

void PhysicsSystem::preUpdate(Real dt)
{

}

void PhysicsSystem::fixedUpdate(Real dt)
{

}

void PhysicsSystem::postUpdate(Real dt)
{

}
