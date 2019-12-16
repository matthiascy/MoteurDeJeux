#include <GameFramework/Components/Colliders/StaticPlaneCollider.hpp>
#include <BulletCollision/CollisionShapes/btStaticPlaneShape.h>

StaticPlaneCollider::StaticPlaneCollider(const String &name, GameObject *gameObject, const Vec3 &normal, Real distance)
  : Collider(name, gameObject, new btStaticPlaneShape(Math::toBtVec3(normal), distance), 0)
{ }
