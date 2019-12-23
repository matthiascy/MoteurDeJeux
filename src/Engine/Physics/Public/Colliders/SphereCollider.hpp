#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_SPHERE_COLLIDER_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_SPHERE_COLLIDER_HPP

#include <Physics/Public/Collider.hpp>

class SphereCollider : public Collider {
public:
  SphereCollider(const String& name, GameObject* gameObject, Real radius);
  ~SphereCollider() override = default;

  [[nodiscard]]
  UInt64 typeID() const override {
    return family::type<SphereCollider>;
  }
};


#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_SPHERE_COLLIDER_HPP */
