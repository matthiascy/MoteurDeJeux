#ifndef MOTEUR_DE_JEUX_SRC_PHYSICS_COLLIDER_HPP
#define MOTEUR_DE_JEUX_SRC_PHYSICS_COLLIDER_HPP

#include <GameFramework/GameFrameworkTypes.hpp>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <Core/Math/Math.hpp>
#include <Core/Memory.hpp>
#include <LinearMath/btDefaultMotionState.h>

template <typename T>
class Collider : public Component<T> {
private:
  UniquePtr<btCollisionShape> m_shape;
  UniquePtr<btRigidBody>      m_rigid_body;
  UniquePtr<btMotionState>    m_motion_state;

public:
  Collider(const String& name, GameObject* gameObject, btCollisionShape* shape, Real mass);
  ~Collider() override;

  virtual btCollisionShape* collisionShape();
  virtual btRigidBody* rigidBody();
  virtual btMotionState* motionState();
};

#include "Private/Collider.inl"

#endif  /* !MOTEUR_DE_JEUX_SRC_PHYSICS_COLLIDER_HPP */
