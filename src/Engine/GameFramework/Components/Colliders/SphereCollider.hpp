#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_SPHERE_COLLIDER_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_SPHERE_COLLIDER_HPP

#include <GameFramework/Components/Collider.hpp>

class SphereCollider : public Collider<SphereCollider> {
public:
  SphereCollider(const String& name, GameObject* gameObject, Real radius, Real mass);
  ~SphereCollider() override = default;
};


#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_SPHERE_COLLIDER_HPP */
