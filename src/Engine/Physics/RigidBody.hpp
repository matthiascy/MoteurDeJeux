#ifndef MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_RIGID_BODY_HPP
#define MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_RIGID_BODY_HPP

#include <GameFramework/Component.hpp>
#include <LinearMath/btMotionState.h>

class btRigidBody;
class RigidBody : public Component, public btMotionState {
private:
  Real m_mass;
  btRigidBody* m_rigid_body;

public:
  RigidBody(const String& name, GameObject *gameObject);

  /**
   * @brief Return initial world transform to Bullet
   */
  void getWorldTransform(btTransform& worldTransform) const;

  /**
   * @brief Update world transform from Bullet
   */
  void setWorldTransform(const btTransform& worldTransform);

  /**
   * @brief Set mass. Zero mass makes the body static.
   */
  void setMass();
  Real mass() const { return m_mass; };

  void setMass() const;

  [[nodiscard]]
  Vec3 linearVelocity() const;

  [[nodiscard]]
  Vec3 angularVelocity() const;

  [[nodiscard]]
  Vec3 gravity() const;

  void setGravity(const Vec3& gravity);

  [[nodiscard]]
  Vec3 totalForce() const;

  [[nodiscard]]
  Vec3 totalTorque() const;

  [[nodiscard]]
  Real linearDamping() const;

  [[nodiscard]]
  Real angularDamping() const;
};


#endif  /* !MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_RIGID_BODY_HPP */
