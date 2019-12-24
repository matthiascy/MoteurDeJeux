#include "Physics/Public/Constraint.hpp"

btTypedConstraint* Constraint::constraint() const
{
  return m_constraint.get();
}
