#include <GameFramework/Components/OrthogonalCamera.hpp>

OrthogonalCamera::OrthogonalCamera(const String& name, GameObject* gameObject,
                                   Real aspect, Real scale, Real zNear, Real zFar)
  : Camera(name, gameObject, aspect, zNear, zFar),
    m_scale{scale}
{ }

Mat4 OrthogonalCamera::projectionMatrix() const
{
  return Math::ortho(-m_aspect * m_scale, m_aspect * m_scale,
                     -m_scale, m_scale, m_z_near, m_z_far);
}
