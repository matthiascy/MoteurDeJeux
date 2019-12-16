#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_CYLINDER_COLLIDER_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_CYLINDER_COLLIDER_HPP

#include <GameFramework/Components/Collider.hpp>

class CylinderCollider : public Collider<CylinderCollider> {
public:
  CylinderCollider(const String& name, GameObject* gameObject, Real radius, Real height, Real mass);
  ~CylinderCollider() override = default;
};


#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_CYLINDER_COLLIDER_HPP */
