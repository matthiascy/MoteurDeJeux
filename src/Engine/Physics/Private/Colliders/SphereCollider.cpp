#include <Physics/Public/Colliders/SphereCollider.hpp>
#include <BulletCollision/CollisionShapes/btSphereShape.h>


SphereCollider::SphereCollider(const String &name, GameObject *gameObject, Real radius)
  : Collider(name, gameObject, new btSphereShape(radius))
{ }
