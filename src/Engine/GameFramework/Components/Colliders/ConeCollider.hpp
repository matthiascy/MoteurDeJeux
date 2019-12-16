#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_CONE_COLLIDER_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_CONE_COLLIDER_HPP

#include <GameFramework/Components/Collider.hpp>

class ConeCollider : public Collider<ConeCollider> {
public:
  ConeCollider(const String& name, GameObject* gameObject, Real radius, Real height, Real mass);
  ~ConeCollider() override = default;
};


#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_CONE_COLLIDER_HPP */
