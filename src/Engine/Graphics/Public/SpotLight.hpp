#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_PUBLIC_SPOT_LIGHT_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_PUBLIC_SPOT_LIGHT_HPP

#include "Light.hpp"

class SpotLight : public Light {
private:
  Attenuation m_attenuation;
  Real m_cutoff_angle;
  Real m_range;

public:
  SpotLight(const String& name, GameObject* gameObject, const Vec3& color, Real intensity,
            Real range, Real cutOffAngle, Attenuation attenuation);

  ~SpotLight() override = default;

  [[nodiscard]]
  UInt64 typeID() const override;

  [[nodiscard]]
  Real range() const;

  void setRange(Real range);

  [[nodiscard]]
  Real cutOffAngle() const;

  void setCutOffAngle(Real angle);

  [[nodiscard]]
  Attenuation attenuation() const;

  void setAttenuation(Attenuation attenuation);
};


#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_PUBLIC_SPOT_LIGHT_HPP */
