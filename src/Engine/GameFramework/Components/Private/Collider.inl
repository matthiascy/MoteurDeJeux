#ifndef MOTEUR_DE_JEUX_SRC_PHYSICS_COLLIDER_HPP
  #error "Collider.inl must be included from Collider.hpp"
#endif

template <typename T>
Collider<T>::Collider(const String& name, GameObject* gameObject, btCollisionShape* shape, Real mass)
  : Component<T>(name, gameObject)
{
  m_shape.reset(shape);

  bool is_dynamic = (mass != 0);

  btVector3 local_inertial(0, 0, 0);
  if (is_dynamic)
    m_shape->calculateLocalInertia(mass, local_inertial);

  auto* transform = gameObject->transform();
  m_motion_state = makeUnique<btDefaultMotionState>(btTransform(Math::toBTQuat(transform->worldRotation()), btVector3(0, 0, 0)));
  m_rigid_body = makeUnique<btRigidBody>(mass, m_motion_state.get(), m_shape.get(), local_inertial);
}

template <typename T>
Collider<T>::~Collider()
{
  m_motion_state.reset();
  m_rigid_body.reset();
}

template <typename T>
btCollisionShape* Collider<T>::collisionShape()
{
  return m_shape.get();
}

template <typename T>
btRigidBody* Collider<T>::rigidBody()
{
  return m_rigid_body.get();
}

template <typename T>
btMotionState* Collider<T>::motionState()
{
  return m_motion_state.get();
}

