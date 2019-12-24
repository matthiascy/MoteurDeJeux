#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_ENGINE_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_ENGINE_HPP

#include <Core/Public/Core.hpp>
#include "Forward.hpp"

class Engine {
private:
  UniquePtr<AssetManager>     m_asset_manager;
  UniquePtr<ComponentManager> m_component_manager;
  UniquePtr<SceneManager>     m_scene_manager;

  UniquePtr<RenderSystem>     m_render_sys;
  UniquePtr<PhysicsSystem>    m_physics_sys;
  UniquePtr<InputSystem>      m_input_sys;
  UniquePtr<BehaviorSystem>   m_behavior_sys;
  GameApp*                    m_app;

public:

  [[nodiscard]]
  bool isKeyPressed(Qt::Key key) const;

  [[nodiscard]]
  bool isKeyTriggered(Qt::Key key) const;

public:
  explicit Engine(GameApp* app);
  ~Engine();

  bool init();

  [[nodiscard]]
  AssetManager* assetManager() const;

  [[nodiscard]]
  ComponentManager* componentManager() const;

  [[nodiscard]]
  SceneManager* sceneManager() const;

  [[nodiscard]]
  RenderSystem* renderSystem() const;

  [[nodiscard]]
  PhysicsSystem* physicsSystem() const;

  [[nodiscard]]
  InputSystem* inputSystem() const;

  [[nodiscard]]
  BehaviorSystem* behaviorSystem() const;
};

#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_ENGINE_HPP */
