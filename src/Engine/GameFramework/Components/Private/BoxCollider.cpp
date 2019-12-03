#include <GameFramework/Components/BoxCollider.hpp>
#include <Core/Math/Math.hpp>

BoxCollider::BoxCollider(String name, GameObject* gameObject, const Vec3& size, Real mass)
  : Collider(std::move(name), gameObject, new btBoxShape(Math::toBTVec3(size)), mass)
{ }
