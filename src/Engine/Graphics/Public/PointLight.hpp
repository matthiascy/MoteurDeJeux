#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_PUBLIC_POINT_LIGHT_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_PUBLIC_POINT_LIGHT_HPP

#include "Light.hpp"

class PointLight : public Light {
private:
  EAttenuationType m_attenuation_type;
  Real m_attenuation;
  Real m_range;

public:
  PointLight(const String& name, GameObject* gameObject, const Vec3& color, Real intensity,
             Real range = 1.0f, EAttenuationType type = EAttenuationType::Constant, Real attenuation = 1.0f);

  ~PointLight() override = default;

  [[nodiscard]]
  UInt64 typeID() const override;

  [[nodiscard]]
  EAttenuationType attenuationType() const;

  [[nodiscard]]
  Real attenuation() const;

  [[nodiscard]]
  Real range() const;
};


#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_PUBLIC_POINT_LIGHT_HPP */
