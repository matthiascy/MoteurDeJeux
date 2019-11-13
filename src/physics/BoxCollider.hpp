#ifndef MOTEUR_DE_JEUX_SRC_PHYSICS_BOX_COLLIDER_HPP
#define MOTEUR_DE_JEUX_SRC_PHYSICS_BOX_COLLIDER_HPP

#include "Collider.hpp"

class BoxCollider : public Collider {
public:
  BoxCollider(const Vec3& size, float mass);
};


#endif  /* !MOTEUR_DE_JEUX_SRC_PHYSICS_BOX_COLLIDER_HPP */
