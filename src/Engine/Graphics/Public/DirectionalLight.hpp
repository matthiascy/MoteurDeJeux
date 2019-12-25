#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_PUBLIC_DIRECTIONAL_LIGHT_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_PUBLIC_DIRECTIONAL_LIGHT_HPP

#include <Graphics/Public/Light.hpp>

class DirectionalLight : public Light {
public:
  DirectionalLight(const String& name, GameObject* gameObject, const Vec3& color, Real intensity);
  ~DirectionalLight() override = default;

  [[nodiscard]]
  UInt64 typeID() const override;
};

#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_PUBLIC_DIRECTIONAL_LIGHT_HPP */
