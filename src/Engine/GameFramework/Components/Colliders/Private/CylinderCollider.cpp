#include "GameFramework/Components/Colliders/CylinderCollider.hpp"

CylinderCollider::CylinderCollider(const String &name, GameObject *gameObject, Real radius, Real height, Real mass)
  : Collider<CylinderCollider>(name, gameObject, new btCylinderShape(btVector3{height, radius, radius}), mass)
{ }
