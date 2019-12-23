#include <Physics/Public/Collider.hpp>
#include <Physics/Public/RigidBody.hpp>
#include <GameFramework/GameObject.hpp>

Collider::Collider(const String& name, GameObject* gameObject, btCollisionShape* shape)
  : Component(name, gameObject)
{
  m_shape.reset(shape);

  m_rigid_body = gameObject->getComponent<RigidBody>();

  /*
  bool is_dynamic = (mass != 0);

  btVector3 local_inertial(0, 0, 0);
  if (is_dynamic)
    m_shape->calculateLocalInertia(mass, local_inertial);

  m_motion_state = makeUnique<btDefaultMotionState>(btTransform(Math::toBtQuat(gameObject->transform()->worldRotation()),
                                                                Math::toBtVec3(gameObject->transform()->worldPosition())));
  m_rigid_body = makeUnique<btRigidBody>(mass, m_motion_state.get(), m_shape.get(), local_inertial);
   */
}

Collider::~Collider()
{
  m_shape.reset(nullptr);
}

btCollisionShape* Collider::collisionShape()
{
  return m_shape.get();
}

RigidBody* Collider::rigidBody()
{
  return m_rigid_body;
}

void Collider::setRigidBody(RigidBody* rigidBody)
{
  m_rigid_body = rigidBody;
}

