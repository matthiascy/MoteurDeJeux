#include <Core/Core.hpp>
#include <GameFramework/Engine.hpp>
#include <GameFramework/GameApp.hpp>
#include <Editor/EditorMainWindow.hpp>
#include <Graphics/OpenGLWindow.hpp>

Engine::Engine(GameApp* app)
  : m_app{app}
{
  // TODO: asset manager to system
  qDebug() << "Engine instance creation.";
  m_asset_manager = makeUnique<AssetManager>(m_app->name() + "AssetManager");
  m_render_sys = makeUnique<RenderSystem>(m_app->name() + "RenderSystem", this);
  //m_physics_sys = makeUnique<PhysicsSystem>(m_app->name() + "PhysicsSystem", this);
  //m_input_sys = makeUnique<InputSystem>(m_app->name() + "InputSystem", this);

  if (m_app->isEditorEnabled()) {
    m_window = makeUnique<EditorMainWindow>();
  } else {
    m_window = makeUnique<OpenGLWindow>();
  }
}

Engine::~Engine()
{
  m_asset_manager.reset(nullptr);
  //m_render_sys.reset();
  //m_physics_sys.reset();
  //m_input_sys.reset();
  m_window.reset(nullptr);
}

AbstractWindow* Engine::window()
{
  return m_window.get();
}

AssetManager* Engine::assetManager() const
{
  return m_asset_manager.get();
}

bool Engine::init()
{
  m_render_sys->init();
  return true;
}

RenderSystem *Engine::renderSystem() const
{
  return m_render_sys.get();
}

