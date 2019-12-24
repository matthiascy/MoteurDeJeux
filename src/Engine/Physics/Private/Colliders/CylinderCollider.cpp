#include <Physics/Public/Colliders/CylinderCollider.hpp>

CylinderCollider::CylinderCollider(const String &name, GameObject *gameObject, PhysicsWorld* world, RigidBody* rigidBody,
                                   Real radius, Real height)
  : Collider(name, gameObject, world, rigidBody, CollisionShapeType::Cylinder,
             Math::Zero, Math::QuatIdentity, {radius, height, 0})
{ }
