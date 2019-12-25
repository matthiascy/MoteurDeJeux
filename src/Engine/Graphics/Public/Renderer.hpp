#ifndef MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_RENDERER_HPP
#define MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_RENDERER_HPP

#include <GameFramework/Public/ECS/Component.hpp>

class Renderer : public Component {
private:
public:
  Renderer(const String& name, GameObject* gameObject);
  ~Renderer() override = default;
};

#endif  /* !MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_RENDERER_HPP */
