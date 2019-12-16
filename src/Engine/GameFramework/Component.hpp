#ifndef MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENT_HPP
#define MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENT_HPP

#include <GameFramework/Forward.hpp>
#include <Core/Handle.hpp>
#include <Core/Object.hpp>
#include <GameFramework/Types.hpp>

class GameObject;

enum class EComponentType {
  None = 0x00,
  BoxCollider = 0x01,
  MeshRenderer = 0x02,
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
  inline bool isActive() const { return m_is_active; };

  inline void setActive(bool status) { m_is_active = status; }

  [[nodiscard]]
  inline GameObject* gameObject() const { return m_game_object; }
};

#endif  /* !MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENT_HPP */
