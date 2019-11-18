#include <Engine/GameFramework/Components/Light.hpp>

Light::Light(String name, GameObject* gameObject, const Vec3& color, Real intensity)
  : Component(std::move(name), gameObject), m_color{color}, m_intensity{intensity}
{ }

const Vec3& Light::color() const
{
  return m_color;
}

const Vec3& Light::intensity() const
{
  return m_intensity;
}
