#include "BoxCollider.hpp"

BoxCollider::BoxCollider(const Vec3& size, float max)
  : Collider(new btBoxShape(btVector3(size.x(), size.y(), size.z())), mass)
{ }
