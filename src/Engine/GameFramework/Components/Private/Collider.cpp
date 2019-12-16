#include <GameFramework/Components/Collider.hpp>
#include <GameFramework/GameObject.hpp>

Collider::Collider(const String& name, GameObject* gameObject, btCollisionShape* shape, Real mass)
  : Component(name, gameObject)
{
  m_shape.reset(shape);

  bool is_dynamic = (mass != 0);

  btVector3 local_inertial(0, 0, 0);
  if (is_dynamic)
    m_shape->calculateLocalInertia(mass, local_inertial);

  m_motion_state = makeUnique<btDefaultMotionState>(btTransform(Math::toBtQuat(gameObject->transform()->worldRotation()),
                                                                Math::toBtVec3(gameObject->transform()->worldPosition())));
  m_rigid_body = makeUnique<btRigidBody>(mass, m_motion_state.get(), m_shape.get(), local_inertial);
}

Collider::~Collider()
{
  m_motion_state.reset(nullptr);
  m_rigid_body.reset(nullptr);
  m_motion_state.reset(nullptr);
}

btCollisionShape* Collider::collisionShape()
{
  return m_shape.get();
}

btRigidBody* Collider::rigidBody()
{
  return m_rigid_body.get();
}

btMotionState* Collider::motionState()
{
  return m_motion_state.get();
}

