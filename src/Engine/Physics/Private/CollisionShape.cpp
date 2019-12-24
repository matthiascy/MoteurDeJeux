#include "Physics/Public/CollisionShape.hpp"
#include "Physics/Public/PhysicsWorld.hpp"
#include "Physics/Public/RigidBody.hpp"
#include "Physics/Public/BulletTypes.hpp"

CollisionShape::CollisionShape(PhysicsWorld* world, RigidBody* rigidBody, CollisionShapeType type,
                               const Vec3& position, const Quat& rotation, const Vec3& size)
  : m_collision_shape{nullptr}, m_collision_shape_type{type},
    m_physics_world{world}, m_rigid_body{rigidBody},
    m_position{position}, m_rotation{rotation}, m_size{size}
{
  _create_collision_shape(type);
}

CollisionShape::~CollisionShape()
{
  // Remove collision shape from rigid body's compound shape
  if (m_rigid_body)
    m_rigid_body->removeCollisionShape(this);

  m_collision_shape.reset(nullptr);

  if (m_physics_world)
    m_physics_world->removeCollisionShape(this);
}

void CollisionShape::_create_collision_shape(CollisionShapeType shapeType)
{
  switch (m_collision_shape_type) {
    case CollisionShapeType::Box: {
      m_collision_shape = makeUnique<btBoxShape>(btVector3{0.5, 0.5, 0.5});
    } break;

    case CollisionShapeType::Sphere: {
      m_collision_shape = makeUnique<btSphereShape>(1);
    } break;

    case CollisionShapeType::StaticPlane: {
      m_collision_shape = makeUnique<btStaticPlaneShape>(btVector3{0, 1, 0}, 1);
    } break;

    case CollisionShapeType::Capsule: {
      m_collision_shape = makeUnique<btCapsuleShape>(0.5, 1);
    } break;

    case CollisionShapeType::Cone: {
      m_collision_shape = makeUnique<btConeShape>(0.5, 1);
    } break;

    default:
      break;
  }

  _update_collision_shape();

  if (m_collision_shape) {
    m_collision_shape->setUserPointer(this);
  }
}

CollisionShapeType CollisionShape::shapeType() const
{
  return m_collision_shape_type;
}

PhysicsWorld* CollisionShape::physicsWorld() const
{
  return m_physics_world;
}

btCollisionShape* CollisionShape::bulletCollisionShape() const
{
  return m_collision_shape.get();
}

void CollisionShape::setTransform(const Vec3& position, const Quat& rotation)
{
  m_position = position;
  m_rotation = rotation;
  m_rigid_body->updateCollisionShape(this);
}

btTransform CollisionShape::transform() const
{
  btTransform out;
  out.setRotation(Math::toBtQuat(m_rotation));
  out.setOrigin(Math::toBtVec3(m_position));
  return out;
}

Quat CollisionShape::rotation() const
{
  return m_rotation;
}

void CollisionShape::setRotation(const Quat& rotation)
{
  m_rotation = rotation;
  m_rigid_body->updateCollisionShape(this);
}

Real CollisionShape::margin() const
{
  return m_collision_shape->getMargin();
}

void CollisionShape::setMargin(Real margin)
{
  if (margin != m_collision_shape->getMargin()) {
    m_collision_shape->setMargin(margin);
  }
}

Vec3 CollisionShape::position() const
{
  return m_position;
}

void CollisionShape::setPosition(const Vec3& position)
{
  if (m_position != position) {
    m_position = position;
    m_rigid_body->updateCollisionShape(this);
  }
}

void CollisionShape::setSize(const Vec3& size)
{
  if (m_size != size) {
    m_size = size;
    _update_collision_shape();
    m_rigid_body->updateCollisionShape(this);
  }
}

Vec3 CollisionShape::size() const
{
  return m_size;
}

RigidBody* CollisionShape::rigidBody() const
{
  return m_rigid_body;
}

void CollisionShape::setRigidBody(RigidBody* body)
{
  if (body) {
    if (m_rigid_body) {
      // Remove collision shape from old rigid body
      m_rigid_body->removeCollisionShape(this);
      // Add it to new rigid body
      m_rigid_body = body;
      m_rigid_body->addCollisionShape(this);
    } else {
      m_rigid_body = body;
      m_rigid_body->addCollisionShape(this);
    }
  }
}

void CollisionShape::_update_collision_shape()
{
  switch (m_collision_shape_type) {
    case CollisionShapeType::Box: {
      m_collision_shape->setLocalScaling(Math::toBtVec3(m_size));
    } break;

    case CollisionShapeType::Sphere: {
      m_collision_shape->setLocalScaling({m_size.x(), m_size.y(), m_size.z()});
    } break;

    case CollisionShapeType::StaticPlane: {
      m_collision_shape->setLocalScaling(Math::toBtVec3(m_size));
    } break;

    case CollisionShapeType::Capsule: {
      m_collision_shape->setLocalScaling({m_size.x(), m_size.y(), m_size.x()});
    } break;

    case CollisionShapeType::Cone: {
      m_collision_shape->setLocalScaling({m_size.x(), m_size.y(), m_size.x()});
    } break;

    default:
      break;
  }
}
