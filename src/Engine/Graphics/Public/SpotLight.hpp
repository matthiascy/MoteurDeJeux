#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_PUBLIC_SPOT_LIGHT_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_PUBLIC_SPOT_LIGHT_HPP

#include "Light.hpp"

class SpotLight : public Light {
private:
  EAttenuationType m_attenuation_type;
  Real m_attenuation;
  Real m_cutoff_angle;
  Real m_range;

public:
  SpotLight(const String& name, GameObject* gameObject, const Vec3& color, Real intensity,
            Real range, Real cutOffAngle, EAttenuationType type, Real attenuation);

  ~SpotLight() override = default;

  [[nodiscard]]
  UInt64 typeID() const override;

  [[nodiscard]]
  Real range() const;

  [[nodiscard]]
  Real cutOffAngle() const;

  [[nodiscard]]
  Real attenuation() const;

  [[nodiscard]]
  EAttenuationType attenuationType() const;
};


#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_PUBLIC_SPOT_LIGHT_HPP */
