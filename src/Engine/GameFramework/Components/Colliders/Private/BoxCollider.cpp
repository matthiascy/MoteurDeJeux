#include <GameFramework/Components/Colliders/BoxCollider.hpp>
#include <Core/Math/Math.hpp>

BoxCollider::BoxCollider(const String& name, GameObject* gameObject, const Vec3& size, Real mass)
  : Collider<BoxCollider>(name, gameObject, new btBoxShape(Math::toBtVec3(size)), mass)
{ }
