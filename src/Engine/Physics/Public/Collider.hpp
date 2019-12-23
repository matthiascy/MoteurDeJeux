#ifndef MOTEUR_DE_JEUX_SRC_PHYSICS_COLLIDER_HPP
#define MOTEUR_DE_JEUX_SRC_PHYSICS_COLLIDER_HPP

#include <Core/Public/Math/Math.hpp>
#include <Core/Public/Memory.hpp>
#include <GameFramework/Component.hpp>

class RigidBody;
class btCollisionShape;

class Collider : public Component {
private:
  UniquePtr<btCollisionShape> m_shape;
  RigidBody* m_rigid_body;

public:
  using OnCollisionFunction = std::function<void(RigidBody* other)>;

public:
  Collider(const String& name, GameObject* gameObject, btCollisionShape* shape);
  ~Collider() override;

  virtual btCollisionShape* collisionShape();
  virtual RigidBody* rigidBody();

  virtual void setRigidBody(RigidBody* rigidBody);

  void onCollision(RigidBody* other);

private:
  OnCollisionFunction m_on_collision_fn;
};

#endif  /* !MOTEUR_DE_JEUX_SRC_PHYSICS_COLLIDER_HPP */
