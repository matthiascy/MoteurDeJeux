#include "Physics/PhysicsDebugDrawSystem.hpp"

void PhysicsDebugDrawSystem::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{

}

void PhysicsDebugDrawSystem::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB,
                                              btScalar distance, int lifeTime, const btVector3& color)
{

}

void PhysicsDebugDrawSystem::reportErrorWarning(const char *warningString)
{

}

void PhysicsDebugDrawSystem::draw3dText(const btVector3& location, const char *textString)
{

}

void PhysicsDebugDrawSystem::setDebugMode(int debugMode)
{

}

int PhysicsDebugDrawSystem::getDebugMode() const
{
  return 0;
}

void PhysicsDebugDrawSystem::attachToRenderSystem(RenderSystem* renderSystem)
{
  m_render_system = renderSystem;
}
