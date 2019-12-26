#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_PUBLIC_LIGHT_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_PUBLIC_LIGHT_HPP

#include <Core/Public/Core.hpp>
#include <GameFramework/Public/ECS/Component.hpp>
#include "Forward.hpp"

struct Attenuation {
  Real constant {};
  Real linear   {};
  Real exponent {};
};

class Light : public Component {
protected:
  ELightType m_type;
  Vec3 m_color;
  Real m_intensity;

public:
  Light(const String& name, GameObject* gameObject, const Vec3& color, Real intensity, ELightType type);
  ~Light() override = default;

  [[nodiscard]]
  UInt64 typeID() const override {
    return Component::family::type<Light>;
  }

  [[nodiscard]]
  const Vec3& color() const;

  void setColor(const Vec3& color);

  [[nodiscard]]
  Real intensity() const;

  void setIntensity(Real intensity);

  [[nodiscard]]
  ELightType lightType() const;

};

#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_PUBLIC_LIGHT_HPP */
