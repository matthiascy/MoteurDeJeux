#include <GameFramework/Public/Systems.hpp>
#include "Physics/Public/PhysicsDebugDraw.hpp"

PhysicsDebugDraw::PhysicsDebugDraw(RenderSystem* renderSystem)
{
  m_render_system = renderSystem;
}

void PhysicsDebugDraw::clear()
{
  m_lines.clear();
  m_render_system->physicsDebugDrawInfo().size = 0;
  m_render_system->makeCurrent();
  auto* vbo = m_render_system->vboAt(m_render_system->physicsDebugDrawInfo().vboIdx);
  vbo->bind();
  vbo->allocate(m_lines.constData(), m_lines.size());
  m_render_system->doneCurrent();
}

void PhysicsDebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
  _push_back_vertex(m_lines, from, color);
  _push_back_vertex(m_lines, to, color);

  auto& debug_draw_info = m_render_system->physicsDebugDrawInfo();
  debug_draw_info.size += 2;
  m_render_system->makeCurrent();
  auto* vbo = m_render_system->vboAt(debug_draw_info.vboIdx);
  vbo->bind();
  vbo->allocate(m_lines.constData(), m_lines.size());
  m_render_system->doneCurrent();
}

void PhysicsDebugDraw::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB,
                                        btScalar distance, int lifeTime, const btVector3& color)
{
  // TODO: deal life time
  drawLine(PointOnB, PointOnB + normalOnB * distance, color);
}

void PhysicsDebugDraw::reportErrorWarning(const char *warningString)
{

}

void PhysicsDebugDraw::draw3dText(const btVector3& location, const char* textString)
{

}

void PhysicsDebugDraw::setDebugMode(int debugMode)
{
  m_debug_mode = debugMode;
}

int PhysicsDebugDraw::getDebugMode() const
{
  return m_debug_mode;
}

void PhysicsDebugDraw::toggleDebugFlag(int flag)
{
  if (m_debug_mode & flag)
    m_debug_mode = m_debug_mode & (~flag);
  else
    m_debug_mode |= flag;
}

void PhysicsDebugDraw::_push_back_vertex(Array<Real>& array, const btVector3& position, const btVector3& color)
{
  for (auto i = 0; i < 3; ++i) {
    array.push_back(position[i]);
  }

  for (auto i = 0; i < 3; ++i) {
    array.push_back(color[i]);
  }
}
