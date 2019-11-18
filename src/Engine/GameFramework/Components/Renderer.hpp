#ifndef MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_RENDERER_HPP
#define MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_RENDERER_HPP

#include <Engine/GameFramework/Component.hpp>

class Renderer : public Component {
private:

public:
  Renderer(String name, GameObject* gameObject);
};


#endif  /* !MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_RENDERER_HPP */
