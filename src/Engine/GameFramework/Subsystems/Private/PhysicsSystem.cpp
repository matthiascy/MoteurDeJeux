#include "Engine/GameFramework/Subsystems/PhysicsSystem.hpp"
#include "Engine/GameFramework/GameObject.hpp"

PhysicsSystem::PhysicsSystem(const Vec3& gravity)
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

inline const Vec3& PhysicsSystem::gravity() const
{
  return m_world->getGravity();
}

void PhysicsSystem::update(Real dt)
{
  m_world->stepSimulation(dt, 10);
  m_collision_objects = m_world->getCollisionObjectArray();
}
