#include <Engine/GameFramework/System.hpp>

System::System(String name, Scene* scene)
  : Object(std::move(name)), m_scene{scene}
{ }

