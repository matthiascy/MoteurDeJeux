#include "GameFramework/Components/Behavior.hpp"

Behavior::Behavior(String name, GameObject* gameObject)
  : Component(std::move(name), gameObject)
{ }
