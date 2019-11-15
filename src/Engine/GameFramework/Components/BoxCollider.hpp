#ifndef MOTEUR_DE_JEUX_SRC_PHYSICS_BOX_COLLIDER_HPP
#define MOTEUR_DE_JEUX_SRC_PHYSICS_BOX_COLLIDER_HPP

#include "Collider.hpp"

class BoxCollider : public Collider {
public:
  BoxCollider(String name, GameObject* gameObject, const Vec3& size, Real mass);
  ~BoxCollider() override = default;
};


#endif  /* !MOTEUR_DE_JEUX_SRC_PHYSICS_BOX_COLLIDER_HPP */
