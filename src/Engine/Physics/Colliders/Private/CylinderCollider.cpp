#include <Physics/Colliders/CylinderCollider.hpp>
#include <BulletCollision/CollisionShapes/btCylinderShape.h>

CylinderCollider::CylinderCollider(const String &name, GameObject *gameObject, Real radius, Real height, Real mass)
  : Collider(name, gameObject, new btCylinderShape(btVector3{height, radius, radius}), mass)
{ }
