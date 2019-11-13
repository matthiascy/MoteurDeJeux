#ifndef MOTEUR_DE_JEUX_SRC_PHYSICS_COLLIDER_HPP
#define MOTEUR_DE_JEUX_SRC_PHYSICS_COLLIDER_HPP

#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include "../Types.hpp"

class Collider {
private:
  btCollisionShape* m_shape;
  btRigidBody* m_rigid_body;
  btMotionState* m_motion_state;
public:
  Collider(btCollisionShape* shape, Real mass);
  virtual ~Collider();
  virtual btCollisionShape* collisionShape();
  virtual btRigidBody* rigidBody();
  virtual btMotionState* motionState();
};


#endif  /* !MOTEUR_DE_JEUX_SRC_PHYSICS_COLLIDER_HPP */
