#include "Graphics/Public/SpotLight.hpp"

SpotLight::SpotLight(const String& name, GameObject* gameObject, const Vec3& color, Real intensity,
                     Real range, Real cutOffAngle, Attenuation attenuation)
  : Light(name, gameObject, color, intensity, ELightType::Spot),
    m_range{range}, m_cutoff_angle{cutOffAngle},
    m_attenuation{attenuation}
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

Attenuation SpotLight::attenuation() const
{
  return m_attenuation;
}

void SpotLight::setRange(Real range)
{
  m_range = range;
}

void SpotLight::setCutOffAngle(Real angle)
{
  m_cutoff_angle = angle;
}

void SpotLight::setAttenuation(Attenuation attenuation)
{
  m_attenuation = attenuation;
}
