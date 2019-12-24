#ifndef MOTEUR_DE_JEUX_SRC_PHYSICS_PUBLIC_COLLIDER_HPP
#define MOTEUR_DE_JEUX_SRC_PHYSICS_PUBLIC_COLLIDER_HPP

#include <Core/Public/Math/Math.hpp>
#include <Core/Public/Memory.hpp>
#include <GameFramework/Component.hpp>
#include "Forward.hpp"

class Collider : public Component {
protected:
  UniquePtr<CollisionShape> m_collision_shape;
  RigidBody* m_rigid_body;

public:
  //using OnCollisionFunction = std::function<void(RigidBody* other)>;

public:
  Collider(const String& name, GameObject* gameObject, PhysicsWorld* world, RigidBody* rigidBody, CollisionShapeType shapeType,
           const Vec3& position, const Quat& rotation, const Vec3& size);
  ~Collider() override;

  virtual CollisionShape* collisionShape();
  virtual RigidBody* rigidBody();

  virtual void setRigidBody(RigidBody* rigidBody);

  //void onCollision(RigidBody* other);

private:
  //OnCollisionFunction m_on_collision_fn;
};

#endif  /* !MOTEUR_DE_JEUX_SRC_PHYSICS_PUBLIC_COLLIDER_HPP */
