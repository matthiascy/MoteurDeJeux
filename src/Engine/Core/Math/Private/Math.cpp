#include <Core/Math/Math.hpp>

inline Vec3 Math::mkVec3(Real a)
{
  return Vec3(a, a, a);
}

inline btVector3 Math::toBTVec3(const Vec3 &v)
{
  return {v.x(), v.y(), v.z()};
}

inline Vec3 Math::fromBTVec3(const btVector3 &v)
{
  return {v.x(), v.y(), v.z()};
}

inline btQuaternion Math::toBTQuat(const Quat &q)
{
  return {q.x(), q.y(), q.z(), q.scalar()};
}

inline Quat Math::fromBTQuat(const btQuaternion &q)
{
  return {q.w(), q.x(), q.y(), q.z()};
}

inline Mat4 Math::ortho(Real left, Real right, Real bottom, Real top, Real znear, Real zfar)
{
  Mat4 out;
  out.ortho(left, right, bottom, top, znear, zfar);
  return out;
}

inline static Mat4 perspective(Real fov, Real aspect, Real znear, Real zfar)
{
  Mat4 out;
  out.perspective(fov, aspect, znear, zfar);
  return out;
}

const Mat4 Math::Mat4Identity = Mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

const Quat Math::QuatIdentity = Quat(1.0f, 0.0f, 0.0f, 0.0f);

