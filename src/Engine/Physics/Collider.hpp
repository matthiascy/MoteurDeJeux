#ifndef MOTEUR_DE_JEUX_SRC_PHYSICS_COLLIDER_HPP
#define MOTEUR_DE_JEUX_SRC_PHYSICS_COLLIDER_HPP

#include <Core/Math/Math.hpp>
#include <Core/Memory.hpp>
#include <Physics/ColliderPrivate.hpp>
#include <GameFramework/Component.hpp>

class RigidBody;

class Collider : public Component {
private:
  UniquePtr<btCollisionShape> m_shape;
  RigidBody* m_rigid_body;

public:
  Collider(const String& name, GameObject* gameObject, btCollisionShape* shape, Real mass);
  ~Collider() override;

  virtual btCollisionShape* collisionShape();
  virtual RigidBody* rigidBody();
};

#endif  /* !MOTEUR_DE_JEUX_SRC_PHYSICS_COLLIDER_HPP */
