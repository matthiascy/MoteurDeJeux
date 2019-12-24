#include <Physics/Public/Colliders/ConeCollider.hpp>


ConeCollider::ConeCollider(const String& name, GameObject* gameObject, PhysicsWorld* world, RigidBody* rigidBody,
                           Real radius, Real height)
  : Collider(name, gameObject, world, rigidBody, CollisionShapeType::Cone,
             Math::Zero, Math::QuatIdentity, {radius, height, 0})
{ }
