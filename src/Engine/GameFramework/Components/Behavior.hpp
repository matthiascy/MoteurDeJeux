#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_BEHAVIOR_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_BEHAVIOR_HPP

#include <Engine/GameFramework/Component.hpp>

class InputState;

class Behavior : Component {
public:
  Behavior(String name, GameObject* gameObject);
  virtual void update(Real deltaTime, InputState input) = 0;
};


#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_BEHAVIOR_HPP */