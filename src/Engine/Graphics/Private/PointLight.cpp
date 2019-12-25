#include "Graphics/Public/PointLight.hpp"

#include <utility>

PointLight::PointLight(const String& name, GameObject* gameObject, const Vec3& color, Real intensity,
                       Real range, EAttenuationType type, Real attenuation)
    : Light(name, gameObject, color, intensity, ELightType::Point),
      m_attenuation_type{type}, m_attenuation{attenuation}, m_range{range}
{ }

UInt64 PointLight::typeID() const
{
  return Component::family::type<PointLight>;
}

EAttenuationType PointLight::attenuationType() const
{
  return m_attenuation_type;
}

Real PointLight::attenuation() const
{
  return m_attenuation;
}

Real PointLight::range() const
{
  return m_range;
}
