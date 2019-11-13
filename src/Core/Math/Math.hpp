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
  static const Vec3 DirUp = Vec3(0.0, 1.0, 0.0);
  static const Vec3 DirDown = Vec3(0.0, -1.0, 0.0);
  static const Vec3 DirLeft = Vec3(-1.0, 0.0, 0.0);
  static const Vec3 DirRight = Vec3(1.0, 0.0, 0.0);
  static const Vec3 DirForward = Vec3(-1.0, 0.0, 0.0);
  static const Vec3 DirBackward = Vec3(1.0, 0.0, 0.0);
}

namespace quat {
  static const Quat Identity = Quat(1.0f, 0.0f, 0.0f, 0.0f);
}

#endif  /* !MOTEUR_DE_JEUX_SRC_CORE_MATH_MATH_HPP */
