#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_SCENE_MANAGER_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_SCENE_MANAGER_HPP

#include <GameFramework/Public/Scene.hpp>

class Engine;

struct SceneHandle {
  UInt32 idx;
};

class SceneManager : public Object {
public:
  SceneManager() = delete;
  explicit SceneManager(const String& name, Engine* engine, Object* parent = nullptr);
  ~SceneManager() override;

  SceneHandle createScene(const String& name);

  SceneHandle activatedScene();

  Scene* sceneAt(SceneHandle handle);

  SceneHandle loadScene(const String& path);

  void setActiveScene(SceneHandle handle);

protected:
  Array<Scene*> m_scenes;
  Engine* m_engine;
};


#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_SCENE_MANAGER_HPP */
