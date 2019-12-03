#include <GameFramework/Components/Renderer.hpp>

Renderer::Renderer(String name, GameObject *gameObject)
  : Component(std::move(name), gameObject)
{ }
