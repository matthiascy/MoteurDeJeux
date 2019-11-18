#include <Core/Core.hpp>
#include <GameFramework/Engine.hpp>
#include <GameFramework/GameApp.hpp>
#include <Editor/EditorMainWindow.hpp>
#include <Graphics/OpenGLWindow.hpp>

Engine::Engine(GameApp* app)
  : m_app{app}
{
  //m_asset_manager = makeUnique<AssetManager>(name + "AssetManager");
  //m_renderer_sys = makeUnique<RendererSystem>(name + "RendererSystem", this);
  //m_physics_sys = makeUnique<PhysicsSystem>(name + "PhysicsSystem", this);
  //m_input_sys = makeUnique<InputSystem>(name + "InputSystem", this);
  if (m_app->isEditorEnabled()) {
    //m_window = makeUnique<QMainWindow>();
    m_window = makeUnique<EditorMainWindow>();
  } else {
    //m_window = makeUnique<OpenGLWindow>();
    m_window = makeUnique<QOpenGLWindow>();
  }
}

Engine::~Engine()
{
  m_window.reset();
}

QObject* Engine::window()
{
  return m_window.get();
}

