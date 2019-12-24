#include <Physics/Public/Collider.hpp>
#include <Physics/Public/RigidBody.hpp>
#include <GameFramework/GameObject.hpp>
#include <Physics/Public/CollisionShape.hpp>


Collider::Collider(const String& name, GameObject* gameObject, PhysicsWorld* world, RigidBody* rigidBody,
                   CollisionShapeType shapeType, const Vec3& position, const Quat& rotation, const Vec3& size)
  : Component(name, gameObject)
{
  m_rigid_body = gameObject->getComponent<RigidBody>();
  m_collision_shape = makeUnique<CollisionShape>(world, rigidBody, shapeType, position, rotation, size);
}

Collider::~Collider()
{
  m_collision_shape.reset(nullptr);
}

CollisionShape* Collider::collisionShape()
{
  return m_collision_shape.get();
}

RigidBody* Collider::rigidBody()
{
  return m_rigid_body;
}

void Collider::setRigidBody(RigidBody* rigidBody)
{
  m_rigid_body = rigidBody;
}

