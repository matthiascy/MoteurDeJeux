#ifndef MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENT_HPP
  #error "Component.inl must be included from Component.hpp"
#endif


template <typename T>
Component<T>::Component(const String& name, GameObject* gameObject)
  : AbstractComponent(name, gameObject)
{ }

template <typename T>
UInt64 Component<T>::typeID() const
{
  return m_type_id;
}
