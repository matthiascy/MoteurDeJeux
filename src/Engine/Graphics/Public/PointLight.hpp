#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_PUBLIC_POINT_LIGHT_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_PUBLIC_POINT_LIGHT_HPP

#include "Light.hpp"

class PointLight : public Light {
private:
  Attenuation m_attenuation;
  Real m_range;

public:
  PointLight(const String& name, GameObject* gameObject, const Vec3& color, Real intensity,
             Real range = 1.0f, Attenuation attenuation = {});

  ~PointLight() override = default;

  [[nodiscard]]
  UInt64 typeID() const override;

  [[nodiscard]]
  Attenuation attenuation() const;

  void setAttenuation(Attenuation attenuation);

  [[nodiscard]]
  Real range() const;

  void setRange(Real range);
};


#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_PUBLIC_POINT_LIGHT_HPP */
