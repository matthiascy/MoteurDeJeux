#include <Engine/Core/Core.hpp>
#include <Engine/GameFramework/Engine.hpp>

Engine::Engine(GameApp* app)
{
  m_asset_manager = makeUnique<AssetManager>(name + "AssetManager");
  m_renderer_sys = makeUnique<RendererSystem>(name + "RendererSystem", this);
  m_physics_sys = makeUnique<PhysicsSystem>(name + "PhysicsSystem", this);
  m_input_sys = makeUnique<InputSystem>(name + "InputSystem", this);
}

Engine::~Engine()
{
}

