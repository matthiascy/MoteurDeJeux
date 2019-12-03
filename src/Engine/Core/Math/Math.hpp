#ifndef MOTEUR_DE_JEUX_SRC_CORE_MATH_MATH_HPP
#define MOTEUR_DE_JEUX_SRC_CORE_MATH_MATH_HPP

#include "MathTypes.hpp"

struct Math {

  constexpr static const Vec3 Zero = Vec3(0.0, 0.0, 0.0);

  constexpr static const Vec3 AxisX = Vec3(1.0, 0.0, 0.0);

  constexpr static const Vec3 AxisY = Vec3(0.0, 1.0, 0.0);

  constexpr static const Vec3 AxisZ = Vec3(0.0, 0.0, 1.0);

  constexpr static const Vec3 Up = Vec3(0.0, 1.0, 0.0);

  constexpr static const Vec3 Down = Vec3(0.0, -1.0, 0.0);

  constexpr static const Vec3 Left = Vec3(-1.0, 0.0, 0.0);

  constexpr static const Vec3 Right = Vec3(1.0, 0.0, 0.0);

  constexpr static const Vec3 Forward = Vec3(-1.0, 0.0, 0.0);

  constexpr static const Vec3 Backward = Vec3(1.0, 0.0, 0.0);

  inline static Vec3 mkVec3(Real a) {
    return Vec3(a, a, a);
  }

  inline static btVector3 toBTVec3(const Vec3& v) {
    return {v.x(), v.y(), v.z()};
  }

  inline static Vec3 fromBTVec3(const btVector3 &v) {
    return {v.x(), v.y(), v.z()};
  }

  inline static btQuaternion toBTQuat(const Quat& q) {
    return {q.x(), q.y(), q.z(), q.scalar()};
  }

  inline static Quat fromBTQuat(const btQuaternion& q) {
    return {q.w(), q.x(), q.y(), q.z()};
  }

  static const Mat4 Mat4Identity;

  static const Quat QuatIdentity;

  static Mat4 ortho(Real left, Real right, Real bottom, Real top, Real znear, Real zfar);

  static Mat4 perspective(Real fov, Real aspect, Real znear, Real zfar);
};

#endif  /* !MOTEUR_DE_JEUX_SRC_CORE_MATH_MATH_HPP */
