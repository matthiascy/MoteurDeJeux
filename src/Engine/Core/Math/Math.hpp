#ifndef MOTEUR_DE_JEUX_SRC_CORE_MATH_MATH_HPP
#define MOTEUR_DE_JEUX_SRC_CORE_MATH_MATH_HPP

#include "MathTypes.hpp"

namespace vec3 {
  static const Vec3 Zero = Vec3(0.0, 0.0, 0.0);
  inline Vec3 mkVec3(Real a) { return Vec3(a, a, a); }
  static const Vec3 AxisX = Vec3(1.0, 0.0, 0.0);
  static const Vec3 AxisY = Vec3(0.0, 1.0, 0.0);
  static const Vec3 AxisZ = Vec3(0.0, 0.0, 1.0);
  // Directions
  static const Vec3 Up = Vec3(0.0, 1.0, 0.0);
  static const Vec3 Down = Vec3(0.0, -1.0, 0.0);
  static const Vec3 Left = Vec3(-1.0, 0.0, 0.0);
  static const Vec3 Right = Vec3(1.0, 0.0, 0.0);
  static const Vec3 Forward = Vec3(-1.0, 0.0, 0.0);
  static const Vec3 Backward = Vec3(1.0, 0.0, 0.0);

  inline btVector3 toBTVec3(const Vec3& v)
  {
    return {v.x(), v.y(), v.z()};
  }

  inline Vec3 fromBTVec3(const btVector3& v)
  {
    return {v.x(), v.y(), v.z()};
  }
}

namespace quat {
  static const Quat Identity = Quat(1.0f, 0.0f, 0.0f, 0.0f);

  inline btQuaternion toBTQuat(const Quat& q)
  {
    return {q.x(), q.y(), q.z(), q.scalar()};
  }

  inline Quat fromBTQuat(const btQuaternion& q)
  {
    return {q.w(), q.x(), q.y(), q.z()};
  }
}

#endif  /* !MOTEUR_DE_JEUX_SRC_CORE_MATH_MATH_HPP */
