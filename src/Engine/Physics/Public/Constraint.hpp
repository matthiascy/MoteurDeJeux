#ifndef MOTEUR_DE_JEUX_SRC_PHYSICS_CONSTRAINT_HPP
#define MOTEUR_DE_JEUX_SRC_PHYSICS_CONSTRAINT_HPP

#include <Core/Public/Core.hpp>
#include "BulletForward.hpp"
#include "Forward.hpp"

enum class ConstraintType {
  Point,
  Hinge,
  Slider,
  ConeTwist
};

class Constraint {
private:
  PhysicsSystem* m_physics_system;
  RigidBody*     m_own_body;
  RigidBody*     m_other_body;
  UniquePtr<btTypedConstraint> m_constraint;
  ConstraintType m_constraint_type;
  Vec3           m_position;
  Quat           m_rotation;
  Vec3           m_other_position;
  Quat           m_other_rotation;
  Vec3           m_cached_world_scale; /**< Cached world scale for determining if the constraint position needs update. */
  Vec2           m_height_limit;
  Vec3           m_low_limit;
  Real           m_erp; /**< Error reduction parameter. */
  Real           m_cfm; /**< Constraint force mixing parameter. */
};


#endif  /* !MOTEUR_DE_JEUX_SRC_PHYSICS_CONSTRAINT_HPP */
