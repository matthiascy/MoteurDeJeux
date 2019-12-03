#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_SCENE_MANAGER_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_SCENE_MANAGER_HPP

#include "Scene.hpp"

struct SceneHandle {
  UInt32 idx;
};

class SceneManager : Object {
public:
  SceneManager() = delete;
  explicit SceneManager(String name);
  ~SceneManager() override;

  SceneHandle createScene(String name);

protected:
  Array<Scene*> m_scenes;
};


#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_SCENE_MANAGER_HPP */
