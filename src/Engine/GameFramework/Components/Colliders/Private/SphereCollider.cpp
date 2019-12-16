#include <GameFramework/Components/Colliders/SphereCollider.hpp>
#include <BulletCollision/CollisionShapes/btSphereShape.h>


SphereCollider::SphereCollider(const String &name, GameObject *gameObject, Real radius, Real mass)
  : Collider(name, gameObject, new btSphereShape(radius), mass)
{ }
