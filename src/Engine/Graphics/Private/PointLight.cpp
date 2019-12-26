#include "Graphics/Public/PointLight.hpp"

#include <utility>

PointLight::PointLight(const String& name, GameObject* gameObject, const Vec3& color, Real intensity,
                       Real range, Attenuation attenuation)
    : Light(name, gameObject, color, intensity, ELightType::Point),
      m_attenuation{attenuation}, m_range{range}
{ }

UInt64 PointLight::typeID() const
{
  return Component::family::type<PointLight>;
}

Attenuation PointLight::attenuation() const
{
  return m_attenuation;
}

Real PointLight::range() const
{
  return m_range;
}

void PointLight::setAttenuation(Attenuation attenuation)
{
  m_attenuation = attenuation;
}

void PointLight::setRange(Real range)
{
  m_range = range;
}
