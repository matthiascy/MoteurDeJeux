#include <GameFramework/Components/PerspectiveCamera.hpp>

PerspectiveCamera::PerspectiveCamera(String name, GameObject* gameObject,
                                     float fov, float aspect, float zNear, float zFar)
  : Camera(std::move(name), gameObject, aspect, zNear, zFar),
    m_fov{fov}
{ }

Mat4 PerspectiveCamera::projectionMatrix() const
{
  return Math::perspective(m_fov, m_aspect, m_z_near, m_z_far);
}

Real PerspectiveCamera::fieldOfView() const
{
  return m_fov;
}

void PerspectiveCamera::setFieldOfView(Real fov)
{
  m_fov = fov;
}
