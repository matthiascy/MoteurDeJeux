#include <Physics/Public/Colliders/BoxCollider.hpp>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <Core/Public/Math/Math.hpp>

BoxCollider::BoxCollider(const String& name, GameObject* gameObject, const Vec3& size)
  : Collider(name, gameObject, new btBoxShape(Math::toBtVec3(size)))
{ }
