#include "GameFramework/Components/Colliders/CapsuleCollider.hpp"

CapsuleCollider::CapsuleCollider(const String &name, GameObject *gameObject, Real radius, Real height, Real mass)
  : Collider<CapsuleCollider>(name, gameObject, new btCapsuleShape(radius, height), mass)
{ }
