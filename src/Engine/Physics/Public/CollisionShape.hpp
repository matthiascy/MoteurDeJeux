#ifndef MOTEUR_DE_JEUX_SRC_PHYSICS_PUBLIC_COLLISION_SHAPE_HPP
#define MOTEUR_DE_JEUX_SRC_PHYSICS_PUBLIC_COLLISION_SHAPE_HPP

#include <Core/Public/Core.hpp>
#include <Thirdparty/bullet3/src/LinearMath/btTransform.h>
#include "Forward.hpp"


class CollisionShape {
private:
  UniquePtr<btCollisionShape> m_collision_shape;
  CollisionShapeType          m_collision_shape_type;
  PhysicsWorld*               m_physics_world;
  RigidBody*                  m_rigid_body;
  Vec3                        m_position;
  Quat                        m_rotation;
  Vec3                        m_size;

public:
  CollisionShape(PhysicsWorld* world, RigidBody* rigidBody, CollisionShapeType type,
                 const Vec3& position, const Quat& rotation, const Vec3& size);

  ~CollisionShape();

  [[nodiscard]]
  Vec3 size() const;
  void setSize(const Vec3& size);

  [[nodiscard]]
  Vec3 position() const;
  void setPosition(const Vec3& position);

  [[nodiscard]]
  Real margin() const;
  void setMargin(Real margin);

  [[nodiscard]]
  Quat rotation() const;
  void setRotation(const Quat& rotation);

  [[nodiscard]]
  btTransform transform() const;
  void setTransform(const Vec3& position, const Quat& rotation);

  [[nodiscard]]
  btCollisionShape* bulletCollisionShape() const;

  [[nodiscard]]
  PhysicsWorld* physicsWorld() const;

  [[nodiscard]]
  CollisionShapeType shapeType() const;

  [[nodiscard]]
  RigidBody* rigidBody() const;

  void setRigidBody(RigidBody* body);

private:
  void _create_collision_shape(CollisionShapeType shapeType);
  void _update_collision_shape();
};


#endif  /* !MOTEUR_DE_JEUX_SRC_PHYSICS_PUBLIC_COLLISION_SHAPE_HPP */
