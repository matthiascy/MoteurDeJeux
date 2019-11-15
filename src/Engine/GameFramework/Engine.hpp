#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_ENGINE_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_ENGINE_HPP

#include <Engine/Graphics/Window.hpp>

class GameApp;

class Engine {
private:
  UniquePtr<AssetManager>   m_asset_manager;
  UniquePtr<RendererSystem> m_renderer_sys;
  UniquePtr<PhysicsSystem>  m_physics_sys;
  UniquePtr<InputSystem>    m_input_sys;
  UniquePtr<Window>         m_window;

public:
  Engine(GameApp* app);
  ~Engine();

  void update();
  void start();

  Window* window();
};

#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_ENGINE_HPP */
