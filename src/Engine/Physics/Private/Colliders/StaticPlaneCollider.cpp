#include <Physics/Public/Colliders/StaticPlaneCollider.hpp>
#include <BulletCollision/CollisionShapes/btStaticPlaneShape.h>

StaticPlaneCollider::StaticPlaneCollider(const String &name, GameObject *gameObject, PhysicsWorld* world, RigidBody* rigidBody,
                                         const Vec3 &normal, Real distance)
  : Collider(name, gameObject, world, rigidBody, CollisionShapeType::StaticPlane,
             Math::Zero, Math::QuatIdentity, normal)
{ }
