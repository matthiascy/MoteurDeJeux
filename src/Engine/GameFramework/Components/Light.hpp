#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_LIGHT_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_LIGHT_HPP

#include <Core/Core.hpp>
#include <GameFramework/Forward.hpp>
#include <GameFramework/Component.hpp>

class Light : public Component {
protected:
  Vec3 m_color;
  Real m_intensity;

public:
  Light(String name, GameObject* gameObject, const Vec3& color, Real intensity);
  ~Light() override = default;

  const Vec3& color() const;
  Real intensity() const;
};

#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_LIGHT_HPP */
