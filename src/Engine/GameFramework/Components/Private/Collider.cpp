#include <GameFramework/Components/Collider.hpp>
#include <GameFramework/GameObject.hpp>

Collider::Collider(const String& name, GameObject* gameObject, btCollisionShape* shape, Real mass)
  : Component(name, gameObject)
{
  m_private = makeUnique<ColliderPrivate>(shape, mass, gameObject->transform());
}

Collider::~Collider()
{
  m_private.reset();
}

btCollisionShape* Collider::collisionShape()
{
  return m_private->m_shape.get();
}

btRigidBody* Collider::rigidBody()
{
  return m_private->m_rigid_body.get();
}

btMotionState* Collider::motionState()
{
  return m_private->m_motion_state.get();
}

