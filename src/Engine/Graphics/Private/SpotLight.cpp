#include "Graphics/Public/SpotLight.hpp"

SpotLight::SpotLight(const String& name, GameObject* gameObject, const Vec3& color, Real intensity,
                     Real range, Real cutOffAngle, EAttenuationType type, Real attenuation)
  : Light(name, gameObject, color, intensity, ELightType::Spot),
    m_range{range}, m_cutoff_angle{cutOffAngle},
    m_attenuation_type{type}, m_attenuation{attenuation}
{ }

UInt64 SpotLight::typeID() const
{
  return Component::family::type<SpotLight>;
}

Real SpotLight::range() const
{
  return m_range;
}

Real SpotLight::cutOffAngle() const
{
  return m_cutoff_angle;
}

Real SpotLight::attenuation() const
{
  return m_attenuation;
}

EAttenuationType SpotLight::attenuationType() const
{
  return m_attenuation_type;
}
