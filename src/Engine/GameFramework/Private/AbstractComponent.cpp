#include <GameFramework/AbstractComponent.hpp>
#include <GameFramework/GameObject.hpp>

AbstractComponent::AbstractComponent(const String& name, GameObject* gameObject)
  :m_game_object{gameObject}, m_is_active{true}
{
  m_tag = m_game_object->tag();
  m_transform = m_game_object->transform();
}
