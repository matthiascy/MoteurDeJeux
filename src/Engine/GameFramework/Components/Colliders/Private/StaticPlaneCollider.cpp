#include "GameFramework/Components/Colliders/StaticPlaneCollider.hpp"

StaticPlaneCollider::StaticPlaneCollider(const String &name, GameObject *gameObject, const Vec3 &normal, Real distance)
  : Collider<StaticPlaneCollider>(name, gameObject, new btStaticPlaneShape(Math::toBtVec3(normal), distance), 0)
{ }
