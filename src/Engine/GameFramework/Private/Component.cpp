#include <GameFramework/Component.hpp>
#include <GameFramework/GameObject.hpp>

Component::Component(const String& name, GameObject* gameObject)
  : m_game_object{gameObject}, m_is_active{true}
{
  m_tag = m_game_object->tag();
  m_transform = m_game_object->transform();
}
