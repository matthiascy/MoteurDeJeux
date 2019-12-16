#ifndef MOTEUR_DE_JEUX_SRC_PHYSICS_COLLIDER_PRIVATE_HPP
#define MOTEUR_DE_JEUX_SRC_PHYSICS_COLLIDER_PRIVATE_HPP

#include <Core/Core.hpp>

class btCollisionShape;
class btRigidBody;
class btMotionState;

class ColliderPrivate {

  friend class Collider;

private:
  UniquePtr<btCollisionShape> m_shape;
  UniquePtr<btRigidBody>      m_rigid_body;
  UniquePtr<btMotionState>    m_motion_state;

public:
  ColliderPrivate(btCollisionShape* shape, Real mass, Transform* transform);
  ~ColliderPrivate();
};


#endif  /* !MOTEUR_DE_JEUX_SRC_PHYSICS_COLLIDER_PRIVATE_HPP */
