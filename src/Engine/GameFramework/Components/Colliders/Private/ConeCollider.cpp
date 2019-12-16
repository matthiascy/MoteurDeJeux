#include <GameFramework/Components/Colliders/ConeCollider.hpp>
#include <BulletCollision/CollisionShapes/btConeShape.h>


ConeCollider::ConeCollider(const String &name, GameObject *gameObject, Real radius, Real height, Real mass)
  : Collider(name, gameObject, new btConeShape(radius, height), mass)
{ }
