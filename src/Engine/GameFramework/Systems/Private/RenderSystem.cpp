#include <GameFramework/Systems/RenderSystem.hpp>

RenderSystem::RenderSystem(String name, Engine* engine)
  : System(std::move(name), engine)
{ }
