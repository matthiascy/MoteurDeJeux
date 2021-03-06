#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_BOX_COLLIDER_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_BOX_COLLIDER_HPP

#include "Physics/Public/Collider.hpp"

class BoxCollider : public Collider {
public:
  BoxCollider(const String& name, GameObject* gameObject, PhysicsWorld* world, RigidBody* rigidBody, const Vec3& size);
  ~BoxCollider() override = default;

  [[nodiscard]]
  UInt64 typeID() const override {
    return Component::family::type<BoxCollider>;
  }
};


#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_BOX_COLLIDER_HPP */
