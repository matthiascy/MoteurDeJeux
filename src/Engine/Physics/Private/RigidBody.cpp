#include <Physics/Public/RigidBody.hpp>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <GameFramework/Public/ECS/GameObject.hpp>
#include <Physics/Public/Collider.hpp>
#include <Physics/Public/CollisionShape.hpp>
#include <Physics/Public/PhysicsWorld.hpp>
#include <Physics/Public/BulletTypes.hpp>

RigidBody::RigidBody(const String& name, GameObject* gameObject, PhysicsWorld* world, Real mass)
  : Component(name, gameObject), m_physics_world{world}
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
  if (!m_game_object || !m_physics_world)
    return;

  m_game_object->transform()->setPosition(position, ESpace::World);
  m_game_object->transform()->setRotation(rotation, ESpace::World);
  m_last_position = m_game_object->transform()->worldPosition();
  m_last_rotation = m_game_object->transform()->worldRotation();
}

void RigidBody::_add_body_to_world()
{
  if (!m_physics_world)
    return;

  if (m_mass < 0.0f)
    m_mass = 0.0f;

  if (m_body) {

    _remove_body_from_world();

  } else {

    btVector3 localInertia(0, 0, 0);
    m_body = makeUnique<btRigidBody>(m_mass, this, m_compound_collision_shape.get(), localInertia);
    m_body->setUserPointer(this);

    // Check if collision shapes already exist in the game object and add them to the compound shape.
    if (m_game_object->hasCollider()) {
      Array<Collider*> colliders = m_game_object->getComponentsOfType<Collider>();

      for (auto& collider : colliders) {
        collider->setRigidBody(this);
        m_compound_collision_shape->addChildShape(collider->collisionShape()->transform(),
                                                  collider->collisionShape()->bulletCollisionShape());
      }
    }

    _update_mass();
    _update_gravity();
  }
}

void RigidBody::_remove_body_from_world()
{
  if (m_physics_world && m_body) {
    m_physics_world->removeRigidBody(this);
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
  if (m_physics_world && m_body) {
    int flags = m_body->getFlags();

    if (m_is_using_gravity && m_overridden_gravity == Math::Zero)
      flags &= ~BT_DISABLE_WORLD_GRAVITY;
    else
      flags |= BT_DISABLE_WORLD_GRAVITY;

    m_body->setFlags(flags);

    if (m_is_using_gravity) {

      if (m_overridden_gravity == Math::Zero)
        m_body->setGravity(m_physics_world->world()->getGravity());
      else
        m_body->setGravity(Math::toBtVec3(m_overridden_gravity));

    } else {

      m_body->setGravity(btVector3(0, 0, 0));

    }
  }
}

void RigidBody::updateCollisionShape(CollisionShape* shape)
{
  if (!m_body)
    return;

  for (int i = 0; i < m_compound_collision_shape->getNumChildShapes(); ++i) {
    if (m_compound_collision_shape->getChildShape(i) == shape->bulletCollisionShape()) {
      qDebug() << "Update collision shape";
      m_compound_collision_shape->updateChildTransform(i, shape->transform());
    }
  }

  _update_mass();
  _update_gravity();
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

void RigidBody::removeCollisionShape(CollisionShape* shape)
{
  if (m_collision_shapes.contains(shape)) {
    m_collision_shapes.removeOne(shape);
    m_compound_collision_shape->removeChildShape(shape->bulletCollisionShape());
    _update_mass();
    _update_gravity();
  }
}

void RigidBody::addCollisionShape(CollisionShape* shape)
{
  if (!m_collision_shapes.contains(shape)) {
    m_collision_shapes.append(shape);
    m_compound_collision_shape->addChildShape(shape->transform(), shape->bulletCollisionShape());
    _update_mass();
    _update_gravity();
  }
}

void RigidBody::_update_mass()
{
  if (!m_body)
    return;

  btTransform principal;
  principal.setRotation(btQuaternion::getIdentity());
  principal.setOrigin(btVector3(0.0f, 0.0f, 0.0f));

  // Calculate center of mass shift from all the collision shapes
  auto numShapes = (unsigned)m_compound_collision_shape->getNumChildShapes();
  if (numShapes)
  {
    Array<float> masses(numShapes, 1.0f);
    btVector3 inertia(0.0f, 0.0f, 0.0f);
    m_compound_collision_shape->calculatePrincipalAxisTransform(&masses[0], principal, inertia);
  }

  btCollisionShape* oldCollisionShape = m_body->getCollisionShape();
  m_body->setCollisionShape(m_compound_collision_shape.get());

  /*
  // If we have one shape and this is a triangle mesh, we use a custom material callback in order to adjust internal edges
  if (!useCompound && body_->getCollisionShape()->getShapeType() == SCALED_TRIANGLE_MESH_SHAPE_PROXYTYPE &&
      physicsWorld_->GetInternalEdge())
    body_->setCollisionFlags(body_->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
  else
   */
  m_body->setCollisionFlags(m_body->getCollisionFlags() & ~btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

  // Reapply rigid body position with new center of mass shift
  Vec3 oldPosition = position();
  m_center_of_mass = Math::fromBtVec3(principal.getOrigin());
  setPosition(oldPosition);

  // Calculate final inertia
  btVector3 localInertia(0.0f, 0.0f, 0.0f);
  if (m_mass > 0.0f)
    m_compound_collision_shape->calculateLocalInertia(m_mass, localInertia);
  m_body->setMassProps(m_mass, localInertia);
  m_body->updateInertiaTensor();

  /*
  // Reapply constraint positions for new center of mass shift
  if (node_)
  {
    for (PODVector<Constraint*>::Iterator i = constraints_.Begin(); i != constraints_.End(); ++i)
      (*i)->ApplyFrames();
  }
   */

  // Re add body to world to reset Bullet collision cache if collision shape was changed
  if (m_is_in_world && m_body->getCollisionShape() != oldCollisionShape && m_physics_world)
  {
    m_physics_world->removeRigidBody(this);
    m_physics_world->addRigidBody(this);
  }
}

bool RigidBody::isInWorld() const
{
  return m_is_in_world;
}

void RigidBody::setIsInWorld(bool inOut)
{
  m_is_in_world = inOut;
}
