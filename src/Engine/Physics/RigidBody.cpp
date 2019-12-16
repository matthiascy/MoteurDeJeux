#include <Physics/RigidBody.hpp>
#include <BulletDynamics/Dynamics/btRigidBody.h>

RigidBody::RigidBody(const String &name, GameObject *gameObject)
  : Component(name, gameObject)
{

}

Vec3 RigidBody::linearVelocity() const
{
  return Math::fromBtVec3(m_rigid_body->getLinearVelocity());
}

Vec3 RigidBody::angularVelocity() const
{
  return Math::fromBtVec3(m_rigid_body->getAngularVelocity());
}

Vec3 RigidBody::gravity() const
{
  return Math::fromBtVec3(m_rigid_body->getGravity());
}

void RigidBody::setGravity(const Vec3& gravity)
{
  m_rigid_body->setGravity(Math::toBtVec3(gravity));
}

Vec3 RigidBody::totalForce() const
{
  return Math::fromBtVec3(m_rigid_body->getTotalForce());
}

Vec3 RigidBody::totalTorque() const
{
  return Math::fromBtVec3(m_rigid_body->getTotalTorque());
}
