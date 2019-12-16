#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_CAPSULE_COLLIDER_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_CAPSULE_COLLIDER_HPP

#include <GameFramework/Components/Collider.hpp>

class CapsuleCollider : public Collider<CapsuleCollider> {
public:
  CapsuleCollider(const String& name, GameObject* gameObject, Real radius, Real height, Real mass);
  ~CapsuleCollider() override = default;
};

#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_CAPSULE_COLLIDER_HPP */
