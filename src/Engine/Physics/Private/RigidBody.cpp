#include <Physics/Public/RigidBody.hpp>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <GameFramework/GameObject.hpp>
#include <Physics/Public/Collider.hpp>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

//RigidBody::RigidBody(const String& name, GameObject* gameObject, btDynamicsWorld* world, Real mass)
RigidBody::RigidBody(const String& name, GameObject* gameObject, PhysicsSystem* system, Real mass)
  : Component(name, gameObject),
    m_system{system}
    //m_system->physicsWorld(){world}
{
  m_mass = std::max(mass, 0.0f);
  m_compound_collision_shape = makeUnique<btCompoundShape>();
  _add_body_to_world();
}

RigidBody::~RigidBody()
{
  _destroy_body();
}

void RigidBody::getWorldTransform(btTransform& worldTransform) const
{
  if (m_game_object) {
    m_last_position = m_game_object->transform()->worldPosition();
    m_last_rotation = m_game_object->transform()->worldRotation();
    worldTransform.setOrigin(Math::toBtVec3(m_last_position + m_last_rotation * m_center_of_mass));
    worldTransform.setRotation(Math::toBtQuat(m_last_rotation));
  }
}

void RigidBody::setWorldTransform(const btTransform &worldTransform)
{
  if (!m_body->isActive())
    return;

  Quat newWorldRotation = Math::fromBtQuat(worldTransform.getRotation());
  Vec3 newWorldPosition = Math::fromBtVec3(worldTransform.getOrigin()) - newWorldRotation * m_center_of_mass;
  RigidBody* parentRigidBody = nullptr;

  if (m_game_object) {
    //auto* parent = m_game_object->transform()->parent();

    //if (parent)
      //parentRigidBody = parent->gameObject()->getComponent<RigidBody>();

    //if (!parentRigidBody) {

      _apply_world_transform(newWorldPosition, newWorldRotation);

    //} else {
      // TODO: parent
    //}
  }
}

Real RigidBody::mass() const
{
  return m_mass;
}

void RigidBody::setMass(Real mass)
{
  mass = std::max(mass, 0.0f);
  if (mass != m_mass) {
    m_mass = mass;
    _add_body_to_world();
  }
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
  if (!m_game_object || !m_system->physicsWorld())
    return;

  m_game_object->transform()->setPosition(position, ESpace::World);
  m_game_object->transform()->setRotation(rotation, ESpace::World);
  m_last_position = m_game_object->transform()->worldPosition();
  m_last_rotation = m_game_object->transform()->worldRotation();
}

void RigidBody::_add_body_to_world()
{
  if (!m_system->physicsWorld())
    return;

  if (m_mass < 0.0f)
    m_mass = 0.0f;

  if (m_body) {

    _remove_body_from_world();

  } else {

    // Calculate correct inertia
    btVector3 localInertia(0, 0, 0);

    // Check if collision shapes already exist in the game object and add them to the compound shape.
    // TODO: child shape offset
    if (m_game_object->hasCollider()) {
      Array<Collider*> shapes = m_game_object->getComponentsOfType<Collider>();

      btTransform transform;
      //transform.setRotation(Math::toBtQuat(gameObject()->transform()->worldRotation()));
      //transform.setOrigin(Math::toBtVec3(gameObject()->transform()->worldPosition()));
      transform.setRotation(btQuaternion::getIdentity());
      transform.setOrigin({0, 0, 0});

      for (auto& shape : shapes) {
        shape->setRigidBody(this);
        m_compound_collision_shape->addChildShape(transform, shape->collisionShape());
      }

      qDebug() << m_compound_collision_shape->getNumChildShapes();
    }

    if (m_mass > 0.0f)
      m_compound_collision_shape->calculateLocalInertia(m_mass, localInertia);

    m_body = makeUnique<btRigidBody>(m_mass, this, m_compound_collision_shape.get(), localInertia);
    m_body->setUserPointer(this);

    _update_mass();
  }
}

void RigidBody::_remove_body_from_world()
{
  if (m_system->physicsWorld() && m_body) {
    m_system->physicsWorld()->removeRigidBody(m_body.get());
    m_is_in_world = false;
  }
}

void RigidBody::_destroy_body()
{
  if (m_body) {
    // TODO: Release all constraints which refer to this body
    _remove_body_from_world();
    m_body.reset(nullptr);
  }
}

void RigidBody::_update_gravity()
{
  if (m_system->physicsWorld() && m_body) {
    int flags = m_body->getFlags();

    if (m_is_using_gravity && m_overridden_gravity == Math::Zero)
      flags &= ~BT_DISABLE_WORLD_GRAVITY;
    else
      flags |= BT_DISABLE_WORLD_GRAVITY;

    m_body->setFlags(flags);

    if (m_is_using_gravity) {

      if (m_overridden_gravity == Math::Zero)
        m_body->setGravity(m_system->physicsWorld()->getGravity());
      else
        m_body->setGravity(Math::toBtVec3(m_overridden_gravity));

    } else {

      m_body->setGravity(btVector3(0, 0, 0));

    }
  }
}

void RigidBody::_update_mass()
{
  if (!m_body || !m_is_mass_update_enabled)
    return;

  btTransform principal;
  principal.setRotation(btQuaternion::getIdentity());
  principal.setOrigin(btVector3(0, 0, 0));

  // Calculate center of mass shift from all the collision shapes
  auto shapes_num = m_compound_collision_shape->getNumChildShapes();
  if ( shapes_num > 0) {
    Array<Real> masses(m_compound_collision_shape->getNumChildShapes(), 1.0f);
    btVector3 inertia(0, 0, 0);
    m_compound_collision_shape->calculatePrincipalAxisTransform(&masses[0], principal, inertia);
  }

  btCollisionShape* old_collision_shape = m_body->getCollisionShape();
  m_body->setCollisionShape(m_compound_collision_shape.get());

  Vec3 old_position = position();
  m_center_of_mass = Math::fromBtVec3(principal.getOrigin());
  setPosition(old_position);

  // Calculate final inertia
  btVector3 localInertia(0, 0, 0);
  if (m_mass > 0)
    m_compound_collision_shape->calculateLocalInertia(m_mass, localInertia);

  m_body->setMassProps(m_mass, localInertia);
  m_body->updateInertiaTensor();

  if (m_is_in_world && m_body->getCollisionShape() != old_collision_shape && m_system->physicsWorld()) {
    m_system->physicsWorld()->removeRigidBody(m_body.get());
    m_system->physicsWorld()->addRigidBody(m_body.get(), m_collision_layer, m_collision_mask);
  }
}

void RigidBody::collidingBodies(Array<RigidBody*>& result) const
{
  if (m_system->physicsWorld()) {
    result.clear();
    qFatal("Not implemented.");
  }
}

bool RigidBody::isActive() const
{
  return m_body ? m_body->isActive() && m_is_active : false;
}

Real RigidBody::ccdMotionThreshold() const
{
  return m_body ? m_body->getCcdMotionThreshold() : 0.0f;
}

Real RigidBody::ccdRadius() const
{
  return m_body ? m_body->getCcdSweptSphereRadius() : 0.0f;
}

Real RigidBody::contactProcessingThreshold() const
{
  return m_body ? m_body->getContactProcessingThreshold() : 0.0f;
}

Real RigidBody::restitution() const
{
  return m_body ? m_body->getRestitution() : 0.0f;
}

Real RigidBody::rollingFriction() const
{
  return m_body ? m_body->getRollingFriction() : 0.0f;
}

Vec3 RigidBody::anisotropicFriction() const
{
  return m_body ? Math::fromBtVec3(m_body->getAnisotropicFriction()) : Math::Zero;
}

Real RigidBody::friction() const
{
  return m_body ? m_body->getFriction() : 0.0f;
}

Real RigidBody::angularDamping() const
{
  return m_body ? m_body->getAngularDamping() : 0.0f;
}

Real RigidBody::angularSleepingThreshold() const
{
  return m_body ? m_body->getAngularSleepingThreshold() : 0.0f;
}

Vec3 RigidBody::angularFactor() const
{
  return m_body ? Math::fromBtVec3(m_body->getAngularFactor()) : Math::Zero;
}

Real RigidBody::linearDamping() const
{
  return m_body ? m_body->getLinearDamping() : 0.0f;
}

Real RigidBody::linearSleepingThreshold() const
{
  return m_body ? m_body->getLinearSleepingThreshold() : 0.0f;
}

Vec3 RigidBody::linearFactor() const {
  return m_body ? Math::fromBtVec3(m_body->getLinearFactor()) : Math::Zero;
}

Vec3 RigidBody::linearVelocityAtPoint(const Vec3 &position) const
{
  return m_body ? Math::fromBtVec3(m_body->getVelocityInLocalPoint(Math::toBtVec3(position - m_center_of_mass))) : Math::Zero;
}

btCompoundShape *RigidBody::compoundShape() const
{
  return m_compound_collision_shape.get();
}

Quat RigidBody::rotation() const
{
  return m_body ? Math::fromBtQuat(m_body->getWorldTransform().getRotation()) : Math::QuatIdentity;
}

Vec3 RigidBody::position() const
{
  if (m_body) {

    auto transform = m_body->getWorldTransform();

    return Math::fromBtVec3(transform.getOrigin()) - Math::fromBtQuat(transform.getRotation()) * m_center_of_mass;

  } else {

    return Math::Zero;

  }
}

void RigidBody::enableMassUpdate()
{
  if (m_is_mass_update_enabled) {
    m_is_mass_update_enabled = true;
    _update_mass();
  }
}

void RigidBody::disableMassUpdate()
{
  m_is_mass_update_enabled = false;
}

void RigidBody::activate()
{
  if (m_body && m_mass > 0.0f)
    m_body->activate(true);
}

void RigidBody::resetForces()
{
  if (m_body)
    m_body->clearForces();
}

void RigidBody::applyImpulse(const Vec3& impulse)
{
  if (m_body && impulse != Math::Zero) {
    activate();
    m_body->applyCentralImpulse(Math::toBtVec3(impulse));
  }
}

void RigidBody::applyImpulse(const Vec3& impulse, const Vec3& position)
{
  if (m_body && impulse != Math::Zero) {
    activate();
    m_body->applyImpulse(Math::toBtVec3(impulse), Math::toBtVec3(position - m_center_of_mass));
  }
}

void RigidBody::applyTorqueImpulse(const Vec3& torque)
{
  if (m_body && torque != Math::Zero) {
    activate();
    m_body->applyTorqueImpulse(Math::toBtVec3(torque));
  }
}

void RigidBody::applyTorque(const Vec3& torque)
{
  if (m_body && torque != Math::Zero) {
    activate();
    m_body->applyTorque(Math::toBtVec3(torque));
  }
}

void RigidBody::applyForce(const Vec3& force, const Vec3& position)
{
  if (m_body && force != Math::Zero) {
    activate();
    m_body->applyForce(Math::toBtVec3(force), Math::toBtVec3(position - m_center_of_mass));
  }
}

void RigidBody::applyForce(const Vec3& force)
{
  if (m_body && force != Math::Zero) {
    activate();
    m_body->applyCentralForce(Math::toBtVec3(force));
  }
}

void RigidBody::setCollisionLayerAndMask(UInt32 layer, UInt32 mask)
{
  if (mask != m_collision_mask || mask != m_collision_layer) {
    m_collision_layer = layer;
    m_collision_mask = mask;
    _add_body_to_world();
  }
}

void RigidBody::setCollisionMask(UInt32 mask)
{
  if (mask != m_collision_mask) {
    m_collision_mask = mask;
    _add_body_to_world();
  }
}

void RigidBody::setCollisionLayer(UInt32 layer)
{
  if (layer != m_collision_layer) {
    m_collision_layer = layer;
    _add_body_to_world();
  }
}

void RigidBody::setIsTrigger(bool enable)
{
  if (enable != m_is_trigger) {
    m_is_trigger = enable;
    _add_body_to_world();
  }
}

void RigidBody::setIsKinematic(bool enable)
{
  if (enable != m_is_kinematic) {
    m_is_kinematic = enable;
    _add_body_to_world();
  }
}

void RigidBody::setGravity(const Vec3& gravity)
{
  if (gravity != m_overridden_gravity) {
    m_overridden_gravity = gravity;
    _update_gravity();
  }
}

void RigidBody::setIsUsingGravity(bool enable)
{
  if (enable != m_is_using_gravity) {
    m_is_using_gravity = enable;
    _update_gravity();
  }
}

void RigidBody::setCcdMotionThreshold(Real threshold) const
{
  if (m_body) {
    m_body->setCcdMotionThreshold(std::max(threshold, 0.0f));
  }
}

void RigidBody::setCcdRadius(Real radius)
{
  if (m_body) {
    m_body->setCcdSweptSphereRadius(std::max(radius, 0.0f));
  }
}

void RigidBody::setContactProcessingThreshold(Real threshold)
{
  if (m_body) {
    m_body->setContactProcessingThreshold(threshold);
  }
}

void RigidBody::setRestitution(Real threshold)
{
  if (m_body) {
    m_body->setRestitution(threshold);
  }
}

void RigidBody::setRollingFriction(Real friction)
{
  if (m_body) {
    m_body->setRollingFriction(friction);
  }
}

void RigidBody::setAnisotropicFriction(const Vec3& friction)
{
  if (m_body) {
    m_body->setAnisotropicFriction(Math::toBtVec3(friction));
  }
}

void RigidBody::setFriction(Real friction)
{
  if (m_body) {
    m_body->setFriction(friction);
  }
}

bool RigidBody::isUsingGravity() const
{
  return m_is_using_gravity;
}

void RigidBody::setLinearDamping(Real damping)
{
  if (m_body) {
    m_body->setDamping(damping, m_body->getAngularDamping());
  }
}

void RigidBody::setAngularDamping(float damping)
{
  if (m_body) {
    m_body->setDamping(m_body->getLinearDamping(), damping);
  }
}

void RigidBody::setAngularSleepingThreshold(Real threshold)
{
  if (m_body) {
    m_body->setSleepingThresholds(m_body->getLinearSleepingThreshold(), threshold);
  }
}

void RigidBody::setLinearSleepingThreshold(Real threshold) {
  if (m_body) {
    m_body->setSleepingThresholds(threshold, m_body->getAngularSleepingThreshold());
  }
}

void RigidBody::setAngularFactor(const Vec3& factor)
{
  if (m_body) {
    m_body->setAngularFactor(Math::toBtVec3(factor));
  }
}

void RigidBody::setLinearFactor(const Vec3& factor)
{
  if (m_body) {
    m_body->setLinearFactor(Math::toBtVec3(factor));
  }
}

void RigidBody::setAngularVelocity(const Vec3& velocity)
{
  if (m_body) {
    m_body->setAngularVelocity(Math::toBtVec3(velocity));
    if (velocity != Math::Zero)
      activate();
  }
}

void RigidBody::setLinearVelocity(const Vec3& velocity)
{
  if (m_body) {
    m_body->setLinearVelocity(Math::toBtVec3(velocity));
    if (velocity != Math::Zero)
      activate();
  }
}

bool RigidBody::isKinematic() const
{
  return m_is_kinematic;
}

void RigidBody::setTransform(const Vec3& position, const Quat& rotation)
{
  if (m_body) {
    btTransform& transform = m_body->getWorldTransform();
    transform.setRotation(Math::toBtQuat(rotation));
    transform.setOrigin(Math::toBtVec3(position + rotation * m_center_of_mass));

    // TODO: while simulating

    m_body->updateInertiaTensor();

    activate();
  }
}

void RigidBody::setRotation(const Quat& rotation)
{
  if (m_body) {
    Vec3 old_position = position();
    btTransform& transform = m_body->getWorldTransform();
    transform.setRotation(Math::toBtQuat(rotation));

    if (!(m_center_of_mass == Math::Zero))
      transform.setOrigin(Math::toBtVec3(old_position + rotation * m_center_of_mass));

    // TODO: while simulating

    m_body->updateInertiaTensor();

    activate();
  }
}

void RigidBody::setPosition(const Vec3& position)
{
  if (m_body) {
    btTransform& transform = m_body->getWorldTransform();
    transform.setOrigin(Math::toBtVec3(position + Math::fromBtQuat(transform.getRotation()) * m_center_of_mass));

    // TODO: while simulating

    activate();
  }
}

Vec3 RigidBody::gravity() const
{
  return Vec3();
}

btRigidBody* RigidBody::body() const
{
  return m_body.get();
}

bool RigidBody::isTrigger() const
{
  return m_is_trigger;
}

UInt32 RigidBody::collisionLayer() const
{
  return m_collision_layer;
}

UInt32 RigidBody::collisionMask() const
{
  return m_collision_mask;
}

Vec3 RigidBody::centerOfMass() const
{
  return m_center_of_mass;
}
