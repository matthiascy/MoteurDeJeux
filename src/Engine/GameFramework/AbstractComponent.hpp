#ifndef MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_ABSTRACT_COMPONENT_HPP
#define MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_ABSTRACT_COMPONENT_HPP

#include <Core/Core.hpp>
#include <GameFramework/Types.hpp>

class Transform;

class GameObject;

class AbstractComponent {

  friend class ComponentManager;

protected:
  /* The game object this component is attached to. */
  GameObject* m_game_object;
  /* The tag of this game object. */
  StringView  m_tag;
  /* The Transform attached to this game object. */
  Transform* m_transform;
  /* Is this component is enabled. */
  bool m_is_active;

public:
  AbstractComponent() = delete;
  AbstractComponent(const String& name, GameObject* gameObject);
  virtual ~AbstractComponent() = default;

  [[nodiscard]]
  inline bool isActive() const { return m_is_active; };

  inline void setActive(bool status) { m_is_active = status; }

  [[nodiscard]]
  virtual UInt64 typeID() const = 0;

  [[nodiscard]]
  inline GameObject* gameObject() const { return m_game_object; }
};


#endif  /* !MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_ABSTRACT_COMPONENT_HPP */
