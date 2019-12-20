#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_DIRECTIONAL_LIGHT_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_DIRECTIONAL_LIGHT_HPP

#include <Graphics/Components/Light.hpp>

class DirectionalLight : Light {
public:
  DirectionalLight(String name, GameObject* gameObject, const Vec3& color, Real intensity);
};

#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_DIRECTIONAL_LIGHT_HPP */