#ifndef HOMELAND_HOMELAND_APP_HPP
#define HOMELAND_HOMELAND_APP_HPP

#include <Engine/GameFramework/Public/GameApp.hpp>
#include <Engine/GameFramework/Public/Managers/AssetManager.hpp>

class Scene;
class GameObject;

class HomelandApp : public GameApp {
private:
  Scene* m_main_scene { };
  HashMap<String, ModelHandle> m_assets { };

public:
  HomelandApp(int argc, char** argv);

  ~HomelandApp() override = default;

  void init();

private:
  bool _init_main_scene();

  void _load_game_asset();

  void _init_camera();

  void _init_lights();

  void _init_terrain();
};


#endif  /* !MOTEUR_DE_JEUX_SRC_HOMELAND_HOMELAND_APP_HPP */
