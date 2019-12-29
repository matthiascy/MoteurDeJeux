#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_ANIMATOR_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_ANIMATOR_HPP

#include <GameFramework/Public/ECS/Component.hpp>

/**
 * Controls which animation to be read.
 */
class Animator : Component {
private:
  // TODO:

public:
  Animator(const String& name, GameObject* gameObject);
  ~Animator() override = default;
};


#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_ANIMATOR_HPP */
