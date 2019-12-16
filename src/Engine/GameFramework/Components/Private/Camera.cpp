#include <GameFramework/Components/Camera.hpp>
#include <GameFramework/GameObject.hpp>

Camera::Camera(const String& name, GameObject *gameObject, Real aspect, Real zNear, Real zFar)
  : Component(name, gameObject),
    m_aspect{aspect}, m_z_near{zNear}, m_z_far{zFar}
{ }

Mat4 Camera::viewMatrix() const
{
  return Component::m_game_object->transform()->worldMatrix().inverted();
}

Mat4 Camera::projectionMatrix() const
{
  return Math::Mat4Identity;
}

Real Camera::aspect() const
{
  return m_aspect;
}

void Camera::setAspect(Real aspect)
{
  m_aspect = aspect;
}

Real Camera::zNear() const
{
  return m_z_near;
}

void Camera::setZNear(Real z)
{
  m_z_near = z;
}

Real Camera::zFar() const
{
  return m_z_far;
}

void Camera::setZFar(Real z)
{
  m_z_far = z;
}
