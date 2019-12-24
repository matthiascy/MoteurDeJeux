#include <Physics/Public/Colliders/CapsuleCollider.hpp>
#include <BulletCollision/CollisionShapes/btCapsuleShape.h>

CapsuleCollider::CapsuleCollider(const String& name, GameObject* gameObject, PhysicsWorld* world, RigidBody* rigidBody,
                                 Real radius, Real height)
  : Collider(name, gameObject, world, rigidBody, CollisionShapeType::Capsule,
             Math::Zero, Math::QuatIdentity, {radius, height, 0})
{ }
