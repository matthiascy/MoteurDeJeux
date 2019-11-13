#include <LinearMath/btDefaultMotionState.h>
#include "Collider.hpp"

Collider::Collider(btCollisionShape* shape, Real mass)
{
  m_shape = shape;
  bool is_dynamic = (mass != 0);

  btVector3 local_inertial(0, 0, 0);
  if (is_dynamic)
    m_shape->calculateLocalInertia(mass, local_inertial);

  m_motion_state = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
  btRigidBody::btRigidBodyConstructionInfo rb_info(mass, m_motion_state, m_shape, local_inertial);
  m_rigid_body = new btRigidBody(rb_info);
}

Collider::~Collider()
{
  delete m_motion_state;
  delete m_rigid_body;
  delete m_shape;
}
