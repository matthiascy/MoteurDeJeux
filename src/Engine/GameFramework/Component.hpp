#ifndef MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENT_HPP
#define MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENT_HPP

#include <Engine/GameFramework/Forward.hpp>
#include <Engine/Core/HandleType.hpp>
#include <Engine/Core/Object.hpp>

enum class EComponentType {
  None = 0x00,
  BoxCollider = 0x01,
  MeshRenderer = 0x02,
};

/**
 * Base class for everything attached to Gameobjects.
 */
class Component : public Object {
private:
  /* The game object this component is attached to. */
  GameObject* m_game_object;
  /* The tag of this game object. */
  StringView  m_tag;
  /* The Transform attached to this game object. */
  Transform* m_transform;

public:
  Component() = delete;
  Component(String name, GameObject* gameObject);
  ~Component() override = default;
};

#endif  /* !MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENT_HPP */
