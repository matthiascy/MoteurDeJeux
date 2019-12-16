#include "GameFramework/Components/Colliders/ConeCollider.hpp"

ConeCollider::ConeCollider(const String &name, GameObject *gameObject, Real radius, Real height, Real mass)
  : Collider<ConeCollider>(name, gameObject, new btConeShape(radius, height), mass)
{ }
