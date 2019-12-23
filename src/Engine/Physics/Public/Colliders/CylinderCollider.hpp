#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_CYLINDER_COLLIDER_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_CYLINDER_COLLIDER_HPP

#include <Physics/Public/Collider.hpp>

class CylinderCollider : public Collider {
public:
  CylinderCollider(const String& name, GameObject* gameObject, Real radius, Real height);
  ~CylinderCollider() override = default;

  [[nodiscard]]
  UInt64 typeID() const override {
    return family::type<CylinderCollider>;
  }
};


#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_CYLINDER_COLLIDER_HPP */
