#include <Engine/GameFramework/Components/Collider.hpp>
#include <Engine/Core/Math/Math.hpp>
#include <Engine/Core/Memory.hpp>
#include <LinearMath/btDefaultMotionState.h>

Collider::Collider(String name, GameObject* gameObject, btCollisionShape* shape, Real mass)
  : Component(std::move(name), gameObject)
{
  m_shape.reset(shape);

  bool is_dynamic = (mass != 0);

  btVector3 local_inertial(0, 0, 0);
  if (is_dynamic)
    m_shape->calculateLocalInertia(mass, local_inertial);

  auto* transform = gameObject->transform();
  m_motion_state = makeUnique<btDefaultMotionState>(btTransform(quat::toBTQuat(transform->rotation()), btVector3(0, 0, 0)));
  m_rigid_body = makeUnique<btRigidBody>(mass, m_motion_state.get(), m_shape.get(), local_inertial);
}

Collider::~Collider()
{
  m_motion_state.reset();
  m_rigid_body.reset();
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

