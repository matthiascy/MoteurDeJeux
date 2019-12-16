#include "GameFramework/Components/Colliders/SphereCollider.hpp"

SphereCollider::SphereCollider(const String &name, GameObject *gameObject, Real radius, Real mass)
  : Collider<SphereCollider>(name, gameObject, new btSphereShape(radius), mass)
{ }
