#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_ENGINE_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_ENGINE_HPP

//#include <Assets/AssetManager.hpp>
//#include <GameFramework/Subsystems/RendererSystem.hpp>
//#include <GameFramework/Subsystems/PhysicsSystem.hpp>
//#include <GameFramework/Subsystems/InputSystem.hpp>

class GameApp;

class Engine {
private:
  //UniquePtr<AssetManager>   m_asset_manager;
  //UniquePtr<RendererSystem> m_renderer_sys;
  //UniquePtr<PhysicsSystem>  m_physics_sys;
  //UniquePtr<InputSystem>    m_input_sys;
  UniquePtr<QObject> m_window;
  GameApp*          m_app;

public:
  explicit Engine(GameApp* app);
  ~Engine();

  //void update();
  //void start();

  QObject* window();
};

#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_ENGINE_HPP */
