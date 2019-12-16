#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_COLLIDERS_PLANE_COLLIDER_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_COLLIDERS_PLANE_COLLIDER_HPP

#include <GameFramework/Components/Collider.hpp>

class StaticPlaneCollider : public Collider<StaticPlaneCollider> {
public:
  StaticPlaneCollider(const String& name, GameObject* gameObject, const Vec3& normal, Real distance);
  ~StaticPlaneCollider() override = default;
};


#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_COLLIDERS_PLANE_COLLIDER_HPP */
