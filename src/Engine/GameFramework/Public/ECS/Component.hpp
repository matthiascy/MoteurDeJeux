#ifndef MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENT_HPP
#define MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENT_HPP

#include <GameFramework/Public/Forward.hpp>
#include <Core/Public/Handle.hpp>
#include <Core/Public/Object.hpp>
#include <GameFramework/Public/Types.hpp>

// TODO: system requirement
//       1 => pass reference of system to component
//       2 => system deals with specifique component

class GameObject;

enum class EComponentType {
  None = 0x00,
  Collider = 0x01,
};

/**
 * Base class for everything attached to GameObjects.
 */
class Component {

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
  using family = Family<Component>;

  Component() = delete;
  Component(const String& name, GameObject* gameObject);
  virtual ~Component() = default;

  [[nodiscard]]
  virtual UInt64 typeID() const = 0;

  [[nodiscard]]
  virtual bool isActive() const { return m_is_active; };

  inline void setActive(bool status) { m_is_active = status; }

  [[nodiscard]]
  inline GameObject* gameObject() const { return m_game_object; }
};

//const UInt64 Component::m_type_id = Family<Component>::type<Component>;

#endif  /* !MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENT_HPP */
