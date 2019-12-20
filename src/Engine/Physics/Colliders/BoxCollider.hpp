#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_BOX_COLLIDER_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_BOX_COLLIDER_HPP

#include "Physics/Collider.hpp"

class BoxCollider : public Collider {
public:
  BoxCollider(const String& name, GameObject* gameObject, const Vec3& size);
  ~BoxCollider() override = default;

  [[nodiscard]]
  UInt64 typeID() const override {
    return Component::family::type<BoxCollider>;
  }
};


#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_BOX_COLLIDER_HPP */
