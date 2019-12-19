#ifndef MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_RIGID_BODY_HPP
#define MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_RIGID_BODY_HPP

#include <GameFramework/Component.hpp>
#include <LinearMath/btMotionState.h>
#include <GameFramework/Systems/PhysicsSystem.hpp>
#include "Forward.hpp"

class RigidBody : public Component, public btMotionState {
private:
  PhysicsSystem*             m_physics_system;
  UniquePtr<btRigidBody>     m_body;
  UniquePtr<btCompoundShape> m_compound_collision_shape;
  UniquePtr<btCompoundShape> m_compound_collision_shape_shifted;
  Array<Constraint*>         m_constraints;
  Vec3 m_overridden_gravity; /**< Overridden gravity, if equals zero, we use world gravity. */
  Vec3 m_center_of_mass;
  Real m_mass;
  UInt32 m_collision_layer;
  UInt32 m_collision_mask;

  mutable Vec3 m_last_position;
  mutable Quat m_last_rotation;

  bool m_is_kinematic;
  bool m_is_trigger;
  bool m_is_using_gravity;
  bool m_is_in_system;
  bool m_is_mass_update_enabled;

public:
  RigidBody(const String& name, GameObject* gameObject, PhysicsSystem* system);

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

  /**
   * @brief Returns anisotropic friction.
   */
  Vec3 anisotropicFriction() const;

  /**
   * @brief Returns rolling friction coefficient.
   */
  Real rollingFriction() const;

  /**
   * @brief Returns restitution coefficient.
   */
  Real restitution() const;

  /**
   * @brief Returns contact processing threshold.
   */
  Real contactProcessingThreshold() const;

  /**
   * @brief Returns continuous collision detection swept sphere radius.
   */
  Real ccdRadius() const;

  /**
   * @brief Returns continuous collision detection motion-per-simulation-step threshold.
   */
  Real ccdMotionThreshold() const;

  void setCcdMotionThreshold(Real threshold) const;

  [[nodiscard]]
  Vec3 gravity() const;

  void setGravity(const Vec3& cc);

  bool isUsingGravity() const;

  void setIsUsingGravity(bool enable);

  [[nodiscard]]
  Vec3 totalForce() const;

  [[nodiscard]]
  Vec3 totalTorque() const;

  [[nodiscard]]
  Real linearDamping() const;

  [[nodiscard]]
  Real angularDamping() const;

  /**
   * @brief Returns angular velocity deactivation threshold.
   */
  Real angularSleepingThreshold() const;

  /**
   * @brief Returns linear velocity deactivation threshold.
   */
  Real linearSleepingThreshold() const;

  /**
   * @brief Returns angular degrees of freedom.
   */
  Vec3 angularFactor() const;

  /**
   * @brief Returns linear degrees of freedom.
   */
  Vec3 linearFactor() const;

  [[nodiscard]]
  Vec3 angularVelocity() const;

  [[nodiscard]]
  Vec3 linearVelocity() const;

  /**
   * @brief Returns rigid body's position in world space.
   */
  Vec3 position() const;

  /**
   * @brief Returns rigid body's rotation in world space.
   */
  Quat rotation() const;

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

  bool isActive() const;

  UInt32 collisionLayer() const;

  UInt32 collisionMask() const;

  void collidingBodies(Array<RigidBody*>& result) const;

  void addConstraint(Constraint* constraint);

  void removeConstraint(Constraint* constraint);

  void destroyBody();

  void enableMassUpdate();

  void disableMassUpdate();

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

private:
  void _apply_world_transform(const Vec3& position, const Quat& rotation);

  void _update_mass();

  void _update_gravity();

  void _add_body_to_world();

  void _remove_body_from_system();

  void _re_add_body_to_world();

};


#endif  /* !MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_RIGID_BODY_HPP */
