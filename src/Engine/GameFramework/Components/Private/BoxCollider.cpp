#include <GameFramework/Components/BoxCollider.hpp>
#include <Core/Math/Math.hpp>

BoxCollider::BoxCollider(const String& name, GameObject* gameObject, const Vec3& size, Real mass)
  : Collider(name, gameObject, new btBoxShape(Math::toBTVec3(size)), mass)
{ }
