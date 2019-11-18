#include <GameFramework/Subsystems/RendererSystem.hpp>

RendererSystem::RendererSystem(String name, Scene* scene)
  : System(std::move(name), scene)
{ }
