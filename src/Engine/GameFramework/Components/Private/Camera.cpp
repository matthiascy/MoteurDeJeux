#include <GameFramework/Components/Camera.hpp>
//#include <GameFramework/Components/Transform.hpp>
#include <Core/Core.hpp>

Camera::Camera(String name, GameObject *gameObject, Real aspect, Real zNear, Real zFar)
  : Component(std::move(name), gameObject),
    m_aspect{aspect}, m_z_near{zNear}, m_z_far{zFar}
{ }

Mat4 Camera::viewMatrix() const
{
  // TODO::implementation::lookAt
  return Math::Mat4Identity;
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
