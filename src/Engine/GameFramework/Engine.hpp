#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_ENGINE_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_ENGINE_HPP

#include <Engine/Graphics/Window.hpp>
//#include <Engine/Assets/AssetManager.hpp>
//#include <Engine/GameFramework/Subsystems/RendererSystem.hpp>
//#include <Engine/GameFramework/Subsystems/PhysicsSystem.hpp>
//#include <Engine/GameFramework/Subsystems/InputSystem.hpp>

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
