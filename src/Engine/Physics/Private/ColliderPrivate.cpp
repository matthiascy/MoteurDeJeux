#include <Physics/ColliderPrivate.hpp>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <LinearMath/btDefaultMotionState.h>
#include <GameFramework/Components/Transform.hpp>

ColliderPrivate::ColliderPrivate(btCollisionShape* shape, Real mass, Transform* transform)
{
  m_shape.reset(shape);

  bool is_dynamic = (mass != 0);

  btVector3 local_inertial(0, 0, 0);
  if (is_dynamic)
    m_shape->calculateLocalInertia(mass, local_inertial);

  m_motion_state = makeUnique<btDefaultMotionState>(btTransform(Math::toBtQuat(transform->worldRotation()),
      Math::toBtVec3(transform->worldPosition())));
  m_rigid_body = makeUnique<btRigidBody>(mass, m_motion_state.get(), m_shape.get(), local_inertial);
}

ColliderPrivate::~ColliderPrivate()
{
  m_motion_state.reset(nullptr);
  m_rigid_body.reset(nullptr);
  m_motion_state.reset(nullptr);
}

