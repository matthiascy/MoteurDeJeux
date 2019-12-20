#include <Physics/Colliders/CapsuleCollider.hpp>
#include <BulletCollision/CollisionShapes/btCapsuleShape.h>

CapsuleCollider::CapsuleCollider(const String &name, GameObject *gameObject, Real radius, Real height)
  : Collider(name, gameObject, new btCapsuleShape(radius, height))
{ }
