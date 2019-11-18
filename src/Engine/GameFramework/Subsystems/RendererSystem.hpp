#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_SUBSYSTEMS_RENDERER_SYSTEM_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_SUBSYSTEMS_RENDERER_SYSTEM_HPP

#include <GameFramework/System.hpp>

// TODO forward
class Scene;
class GameObject;

class RendererSystem : public System {
public:
  RendererSystem(String name, Scene* scene);
  ~RendererSystem() override { };


  void init() override { };

  void render(GameObject* gameObject);

};

#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_SUBSYSTEMS_RENDERER_SYSTEM_HPP */
