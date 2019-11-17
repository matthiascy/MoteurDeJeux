#include <Engine/Core/Core.hpp>
#include <Engine/GameFramework/Engine.hpp>
#include <Engine/GameFramework/GameApp.hpp>
#include <Editor/EditorMainWindow.hpp>
#include <Engine/Graphics/OpenGLWindow.hpp>

Engine::Engine(GameApp* app)
  : m_app{app}
{
  //m_asset_manager = makeUnique<AssetManager>(name + "AssetManager");
  //m_renderer_sys = makeUnique<RendererSystem>(name + "RendererSystem", this);
  //m_physics_sys = makeUnique<PhysicsSystem>(name + "PhysicsSystem", this);
  //m_input_sys = makeUnique<InputSystem>(name + "InputSystem", this);
  if (m_app->isEditorEnabled()) {
    m_window = makeUnique<EditorMainWindow>();
  } else {
    m_window = makeUnique<OpenGLWindow>();
  }
}

Engine::~Engine()
{
  m_window.reset();
}

Window* Engine::window()
{
  return m_window.get();
}

