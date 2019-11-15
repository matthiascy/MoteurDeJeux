#include <Engine/GameFramework/Component.hpp>
#include <utility>

Component::Component(String name, GameObject* gameObject)
  : Object(std::move(name)), m_game_object{gameObject}
{
  m_tag = m_game_object->tag();
  m_transform = m_game_object->transform();
}
