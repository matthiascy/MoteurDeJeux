#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_ENGINE_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_ENGINE_HPP

//#include <Assets/AssetManager.hpp>
#include <GameFramework/Systems/RenderSystem.hpp>
#include <GameFramework/Systems/PhysicsSystem.hpp>
#include <GameFramework/Systems/InputSystem.hpp>
#include <Graphics/AbstractWindow.hpp>

#include <QtWidgets/QSplashScreen>

class GameApp;

class Engine {
private:
  //UniquePtr<AssetManager>   m_asset_manager;
  UniquePtr<RenderSystem>   m_render_sys;
  UniquePtr<PhysicsSystem>  m_physics_sys;
  UniquePtr<InputSystem>    m_input_sys;
  UniquePtr<AbstractWindow> m_window;
  GameApp*           m_app;
  //UniquePtr<QSplashScreen> m_splash;

public:
  explicit Engine(GameApp* app);
  ~Engine();

  bool initialize();

  //void update();
  //void start();

  AbstractWindow* window();
};

#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_ENGINE_HPP */
