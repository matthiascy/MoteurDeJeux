#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_CAPSULE_COLLIDER_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_CAPSULE_COLLIDER_HPP

#include <Physics/Public/Collider.hpp>

class CapsuleCollider : public Collider {
public:
  CapsuleCollider(const String& name, GameObject* gameObject, PhysicsWorld* world, RigidBody* rigidBody, Real radius, Real height);
  ~CapsuleCollider() override = default;

  [[nodiscard]]
  UInt64 typeID() const override {
    return Component::family::type<CapsuleCollider>;
  }
};

#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_CAPSULE_COLLIDER_HPP */
