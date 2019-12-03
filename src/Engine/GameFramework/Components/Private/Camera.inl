#ifndef MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_CAMERA_HPP
  #error "Camera.inl must be included from Camera.hpp"
#endif

template <typename T>
Camera<T>::Camera(const String& name, GameObject *gameObject, Real aspect, Real zNear, Real zFar)
  : Component<T>(name, gameObject),
    m_aspect{aspect}, m_z_near{zNear}, m_z_far{zFar}
{ }

template <typename T>
Mat4 Camera<T>::viewMatrix() const
{
  // TODO::implementation::lookAt
  return Math::Mat4Identity;
}

template <typename T>
Mat4 Camera<T>::projectionMatrix() const
{
  return Math::Mat4Identity;
}

template <typename T>
Real Camera<T>::aspect() const
{
  return m_aspect;
}

template <typename T>
void Camera<T>::setAspect(Real aspect)
{
  m_aspect = aspect;
}

template <typename T>
Real Camera<T>::zNear() const
{
  return m_z_near;
}

template <typename T>
void Camera<T>::setZNear(Real z)
{
  m_z_near = z;
}

template <typename T>
Real Camera<T>::zFar() const
{
  return m_z_far;
}

template <typename T>
void Camera<T>::setZFar(Real z)
{
  m_z_far = z;
}
