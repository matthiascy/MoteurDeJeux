#ifndef MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_RIGID_BODY_HPP
#define MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_RIGID_BODY_HPP

#include <GameFramework/Public/ECS/Component.hpp>
#include <LinearMath/btMotionState.h>
#include <GameFramework/Public/Systems/PhysicsSystem.hpp>
#include "Forward.hpp"

// TODO: PhysicsWorld

class RigidBody : public Component, public btMotionState {
private:
  PhysicsWorld*              m_physics_world;
  Array<CollisionShape*>     m_collision_shapes;

  UniquePtr<btRigidBody>     m_body;
  UniquePtr<btCompoundShape> m_compound_collision_shape;
  //TODO: UniquePtr<btCompoundShape> m_compound_collision_shape_shifted;
  //TODO: Array<Constraint*>         m_constraints;
  Vec3 m_overridden_gravity { Math::Zero };/**< Overridden gravity, if equals zero, we use world gravity. */
  Vec3 m_center_of_mass     { Math::Zero };
  Real m_mass               { kDefaultMass };
  UInt32 m_collision_layer  { kDefaultCollisionLayer };
  UInt32 m_collision_mask   { kDefaultCollisionMask };

  mutable Vec3 m_last_position { Math::Zero };
  mutable Quat m_last_rotation { Math::QuatIdentity };

  bool m_is_kinematic           { false };
  bool m_is_trigger             { false };
  bool m_is_using_gravity       { true  };
  bool m_is_in_world            { false };

  constexpr static Real   kDefaultMass            { 0.0f };
  constexpr static Real   kDefaultFriction        { 0.5f };
  constexpr static Real   kDefaultRestitution     { 0.0f };
  constexpr static Real   kDefaultRollingFriction { 0.0f };
  constexpr static UInt32 kDefaultCollisionLayer  { 0x1  };
  constexpr static UInt32 kDefaultCollisionMask   { UINT_MAX };

public:
  RigidBody(const String& name, GameObject* gameObject, PhysicsWorld* world, Real mass);
  ~RigidBody() override;

  /**
   * @brief Return initial world transform to Bullet
   */
  void getWorldTransform(btTransform& worldTransform) const override;

  /**
   * @brief Update world transform from Bullet
   */
  void setWorldTransform(const btTransform& worldTransform) override;

  /**
   * @brief Set mass. Zero mass makes the body static.
   */
  void setMass(Real mass);

  [[nodiscard]]
  Real mass() const;

  Vec3 centerOfMass() const;

  Real friction() const;

  void setFriction(Real friction);

  /**
   * @brief Returns anisotropic friction.
   */
  Vec3 anisotropicFriction() const;

  void setAnisotropicFriction(const Vec3& friction);

  /**
   * @brief Returns rolling friction coefficient.
   */
  Real rollingFriction() const;

  void setRollingFriction(Real friction);

  /**
   * @brief Returns restitution coefficient.
   */
  Real restitution() const;

  void setRestitution(Real threshold);

  /**
   * @brief Returns contact processing threshold.
   */
  Real contactProcessingThreshold() const;

  void setContactProcessingThreshold(Real threshold);

  /**
   * @brief Returns continuous collision detection swept sphere radius.
   */
  Real ccdRadius() const;

  void setCcdRadius(Real radius);

  /**
   * @brief Returns continuous collision detection motion-per-simulation-step threshold.
   */
  Real ccdMotionThreshold() const;

  void setCcdMotionThreshold(Real threshold) const;

  [[nodiscard]]
  Vec3 gravity() const;

  void setGravity(const Vec3& gravity);

  bool isUsingGravity() const;

  void setIsUsingGravity(bool enable);

  [[nodiscard]]
  Vec3 totalForce() const;

  [[nodiscard]]
  Vec3 totalTorque() const;

  [[nodiscard]]
  Real linearDamping() const;

  void setLinearDamping(Real damping);

  [[nodiscard]]
  Real angularDamping() const;

  void setAngularDamping(float damping);

  /**
   * @brief Returns angular velocity deactivation threshold.
   */
  Real angularSleepingThreshold() const;

  void setAngularSleepingThreshold(Real threshold);

  /**
   * @brief Returns linear velocity deactivation threshold.
   */
  Real linearSleepingThreshold() const;

  void setLinearSleepingThreshold(Real threshold);

  /**
   * @brief Returns angular degrees of freedom.
   */
  Vec3 angularFactor() const;

  void setAngularFactor(const Vec3& factor);

  /**
   * @brief Returns linear degrees of freedom.
   */
  Vec3 linearFactor() const;

  void setLinearFactor(const Vec3& factor);

  [[nodiscard]]
  Vec3 angularVelocity() const;

  void setAngularVelocity(const Vec3& velocity);

  [[nodiscard]]
  Vec3 linearVelocity() const;

  void setLinearVelocity(const Vec3& velocity);

  /**
   * @brief Returns rigid body's position in world space.
   */
  Vec3 position() const;

  void setPosition(const Vec3& position);

  /**
   * @brief Returns rigid body's rotation in world space.
   */
  Quat rotation() const;

  void setRotation(const Quat& rotation);

  /**
   * Set rigid body position and rotation in world space as an atomic operation.
   * @param position [in] new position.
   * @param rotation [in] new rotation.
   */
  void setTransform(const Vec3& position, const Quat& rotation);

  /**
   * @brief Returns linear velocity at local point.
   */
  Vec3 linearVelocityAtPoint(const Vec3& position) const;

  btCompoundShape* compoundShape() const;

  btRigidBody* body() const;

  [[nodiscard]]
  bool isTrigger() const;

  void setIsTrigger(bool enable);

  [[nodiscard]]
  bool isKinematic() const;

  void setIsKinematic(bool enable);

  bool isActive() const override;

  UInt32 collisionLayer() const;

  UInt32 collisionMask() const;

  void collidingBodies(Array<RigidBody*>& result) const;

  // TODO: void addConstraint(Constraint* constraint);

  // TODO: void removeConstraint(Constraint* constraint);

  /**
   * @brief Activate rigid body if it was sleeping.
   */
  void activate();

  void resetForces();

  void applyTorqueImpulse(const Vec3& torque);

  /**
   * @brief Applies impulse at local position.
   * @param impulse  [in] impulse over 3 axes.
   * @param position [in] position in local space.
   */
  void applyImpulse(const Vec3& impulse, const Vec3& position);

  /**
   * @brief Applies impulse to center of mass.
   */
  void applyImpulse(const Vec3& impulse);

  void applyTorque(const Vec3& torque);

  void applyForce(const Vec3& force, const Vec3& position);

  void applyForce(const Vec3& force);

  void setCollisionLayerAndMask(UInt32 layer, UInt32 mask);

  void setCollisionMask(UInt32 mask);

  void setCollisionLayer(UInt32 layer);

  [[nodiscard]]
  bool isInWorld() const;

  void setIsInWorld(bool inOut);

  UInt64 typeID() const override {
    return family::type<RigidBody>;
  };

  void addCollisionShape(CollisionShape* shape);

  void removeCollisionShape(CollisionShape* shape);

  void updateCollisionShape(CollisionShape* shape);

private:
  void _apply_world_transform(const Vec3& position, const Quat& rotation);

  void _update_mass();

  void _update_gravity();

  void _add_body_to_world();

  void _remove_body_from_world();

  void _destroy_body();

};


#endif  /* !MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_RIGID_BODY_HPP */
