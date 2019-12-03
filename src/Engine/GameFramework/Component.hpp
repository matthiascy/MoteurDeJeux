#ifndef MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENT_HPP
#define MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENT_HPP

#include <GameFramework/Forward.hpp>
#include <Core/Handle.hpp>
#include <Core/Object.hpp>
#include <GameFramework/AbstractComponent.hpp>
#include "Types.hpp"
#include "TypeID.hpp"

class GameObject;

enum class EComponentType {
  None = 0x00,
  BoxCollider = 0x01,
  MeshRenderer = 0x02,
};

/**
 * Base class for everything attached to GameObjects.
 */
template <typename T>
class Component : public AbstractComponent {
protected:
  static const UInt64 m_type_id;

public:
  Component() = delete;
  Component(const String& name, GameObject* gameObject);
  ~Component() override = default;

  [[nodiscard]]
  static UInt64 componentTypeID() { return m_type_id; }

  [[nodiscard]]
  UInt64 typeID() const override;
};

template <typename T>
const UInt64 Component<T>::m_type_id = FamilyTypeID<Component>::template next<T>();

#include "Private/Component.inl"

#endif  /* !MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENT_HPP */
