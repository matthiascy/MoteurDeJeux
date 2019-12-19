#include <Physics/RigidBody.hpp>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <GameFramework/GameObject.hpp>
#include <Physics/Collider.hpp>

RigidBody::RigidBody(const String& name, GameObject* gameObject, PhysicsSystem* system)
  : Component(name, gameObject), m_mass{1.0f}, m_center_of_mass{0, 0, 0},
    m_is_kinematic{false}, m_is_trigger{false}, m_use_gravity{true},
    m_physics_system{system}, m_is_in_system{false}
{ }

void RigidBody::getWorldTransform(btTransform &worldTransform) const
{
  if (m_game_object) {
    m_last_position = m_game_object->transform()->worldPosition();
    m_last_rotation = m_game_object->transform()->worldRotation();
    worldTransform.setOrigin(Math::toBtVec3(m_last_position + m_last_rotation * m_mass_center));
    worldTransform.setRotation(Math::toBtQuat(m_last_rotation));
  }
}

void RigidBody::setWorldTransform(const btTransform &worldTransform)
{
  if (!m_body->isActive())
    return;

  Quat newWorldRotation = Math::fromBtQuat(worldTransform.getRotation());
  Vec3 newWorldPosition = Math::fromBtVec3(worldTransform.getOrigin()) - newWorldRotation * m_mass_center;
  RigidBody* parentRigidBody = nullptr;

  if (m_game_object) {
    auto* parent = m_game_object->transform()->parent();

    if (parent)
      parentRigidBody = parent->gameObject()->getComponent<RigidBody>();

    if (!parentRigidBody) {

      _apply_world_transform(newWorldPosition, newWorldRotation);

    } else {
      // TODO: parent
    }
  }
}

Real RigidBody::mass() const
{
  return 0;
}

void RigidBody::setMass(Real mass)
{
  m_body->set
}

Vec3 RigidBody::linearVelocity() const
{
  return Math::fromBtVec3(m_body->getLinearVelocity());
}

Vec3 RigidBody::angularVelocity() const
{
  return Math::fromBtVec3(m_body->getAngularVelocity());
}

Vec3 RigidBody::totalForce() const
{
  return Math::fromBtVec3(m_body->getTotalForce());
}

Vec3 RigidBody::totalTorque() const
{
  return Math::fromBtVec3(m_body->getTotalTorque());
}

void RigidBody::_apply_world_transform(const Vec3& position, const Quat& rotation)
{
  if (!m_game_object)
    return;

  m_game_object->transform()->setPosition(position, ESpace::World);
  m_game_object->transform()->setRotation(rotation, ESpace::World);
  m_last_position = m_game_object->transform()->worldPosition();
  m_last_rotation = m_game_object->transform()->worldRotation();
}

void RigidBody::addBodyToSystem()
{
  if (!m_physics_system)
    return;

  if (m_mass < 0.0f)
    m_mass = 0.0f;

  if (m_body) {

    removeBodyFromSystem();

  } else {

    // Calculate correct inertia
    btVector3 localInertia(0, 0, 0);
    if (m_game_object->hasCollider())
      m_collision_shape = m_game_object->getComponent<Collider>()->collisionShape();

    m_body = makeUnique<btRigidBody>(m_mass, this, m_collision_shape, localInertia);
    m_body->setUserPointer(this);

    // Check if CollisionShapes already exist in the game object and add them to the

  }
}

void RigidBody::removeBodyFromSystem()
{

}
