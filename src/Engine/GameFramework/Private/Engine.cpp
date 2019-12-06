#include <Core/Core.hpp>
#include <GameFramework/Engine.hpp>
#include <GameFramework/GameApp.hpp>
#include <Editor/EditorMainWindow.hpp>
#include <GameFramework/Managers/ComponentManager.hpp>

Engine::Engine(GameApp* app)
  : m_app{app}
{
  qInfo() << "Instance creation.";
  m_component_manager = makeUnique<ComponentManager>(m_app->name() + "ComponentManager");
  m_asset_manager = makeUnique<AssetManager>(m_app->name() + "AssetManager");
  m_scene_manager = makeUnique<SceneManager>(m_app->name() + "SceneManager", this);
  m_render_sys = makeUnique<RenderSystem>(m_app->name() + "RenderSystem", this);
  //m_physics_sys = makeUnique<PhysicsSystem>(m_app->name() + "PhysicsSystem", this);
  m_input_sys = makeUnique<InputSystem>(m_app->name() + "InputSystem", this);
  qInfo() << "Instance creation[Finished]";
}

Engine::~Engine()
{
  qDebug() << "Shutting down...";
  m_asset_manager.reset(nullptr);
  m_scene_manager.reset(nullptr);
  m_render_sys.reset(nullptr);
  //m_physics_sys.reset(nullptr);
  m_input_sys.reset(nullptr);
  qDebug() << "Shutting down...[Finished]";
}

AssetManager* Engine::assetManager() const
{
  return m_asset_manager.get();
}

SceneManager *Engine::sceneManager() const
{
  return m_scene_manager.get();
}

bool Engine::init()
{
  m_render_sys->init();
  m_input_sys->init();
  return true;
}

RenderSystem* Engine::renderSystem() const
{
  return m_render_sys.get();
}

PhysicsSystem* Engine::physicsSystem() const
{
  return nullptr;//m_physics_sys.get();
}

InputSystem* Engine::inputSystem() const
{
  return m_input_sys.get();
}

ComponentManager* Engine::componentManager() const {
  return m_component_manager.get();
}

