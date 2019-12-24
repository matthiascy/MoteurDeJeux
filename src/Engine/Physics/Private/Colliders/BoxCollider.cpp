#include <Physics/Public/Colliders/BoxCollider.hpp>
#include <Physics/Public/CollisionShape.hpp>
#include <Physics/Public/BulletTypes.hpp>
#include <Core/Public/Math/Math.hpp>

BoxCollider::BoxCollider(const String& name, GameObject* gameObject,
                         PhysicsWorld* world, RigidBody* rigidBody, const Vec3& size)
  : Collider(name, gameObject, world, rigidBody, CollisionShapeType::Box,
             Math::Zero, Math::QuatIdentity, size)
{ }
