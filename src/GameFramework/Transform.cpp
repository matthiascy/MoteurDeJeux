#include "Transform.hpp"

Transform::Transform(const Vec3& position, const Quat& rotation, const Vec3& scale)
{
  m_position = position;
  m_rotation = rotation;
  m_scale = scale;
}

Transform& Transform::rotate(const Vec3& axis, Real angle)
{
  m_rotation = Quat::fromAxisAndAngle(axis, angle).normalized() * m_rotation;
  return *this;
}

Transform& Transform::rotate(const Quat& q)
{
  m_rotation = q.normalized() * m_rotation;
  return *this;
}

Transform& Transform::scale(Real s)
{
  m_scale = vec3::mkVec3(s);
  return *this;
}

Transform& Transform::scale(const Vec3& s)
{
  m_scale = s;
  return *this;
}

Transform& Transform::translate(const Vec3& t)
{
  m_position += m_rotation * t;
  return *this;
}

Transform& Transform::setPosition(const Vec3& position)
{
  m_position = position;
  return *this;
}

Transform& Transform::setScale(const Vec3& scale)
{
  m_scale = scale;
  return *this;
}

Transform& Transform::setRotation(const Quat& rotation)
{
  m_rotation = rotation;
  return *this;
}

const Vec3& Transform::position() const
{
  return m_position;
}

const Vec3& Transform::scale() const {
  return m_scale;
}

const Quat& Transform::rotation() const {
  return m_rotation;
}

Mat4 Transform::matrix() const
{
  Mat4 T, R, S;
  T.setToIdentity(); R.setToIdentity(); S.setToIdentity();
  T.translate(m_position);
  R.rotate(m_rotation);
  S.scale(m_scale);
  return R * T * S;
}
