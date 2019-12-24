#include <Physics/Public/Colliders/SphereCollider.hpp>


SphereCollider::SphereCollider(const String &name, GameObject *gameObject, PhysicsWorld* world, RigidBody* rigidBody,
                               Real radius)
  : Collider(name, gameObject, world, rigidBody, CollisionShapeType::Sphere,
             Math::Zero, Math::QuatIdentity, {radius, 0, 0})
{ }
