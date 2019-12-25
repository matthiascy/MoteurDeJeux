#include <Graphics/Public/Light.hpp>

Light::Light(const String& name, GameObject* gameObject, const Vec3& color, Real intensity, ELightType type)
  : Component(name, gameObject), m_color{color}, m_intensity{intensity}, m_type{type}
{ }

const Vec3& Light::color() const
{
  return m_color;
}

Real Light::intensity() const
{
  return m_intensity;
}

ELightType Light::lightType() const
{
  return m_type;
}

void Light::setColor(const Vec3& color)
{
  m_color = color;
}

void Light::setIntensity(Real intensity)
{
  m_intensity = intensity;
}
