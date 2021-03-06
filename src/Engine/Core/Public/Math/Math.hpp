#ifndef MOTEUR_DE_JEUX_SRC_CORE_MATH_MATH_HPP
#define MOTEUR_DE_JEUX_SRC_CORE_MATH_MATH_HPP

#include "MathTypes.hpp"

struct Math {

  constexpr static const Vec3 Zero = Vec3(0.0, 0.0, 0.0);

  constexpr static const Vec3 One = Vec3(1.0, 1.0, 1.0);

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

  inline static btVector3 toBtVec3(const Vec3& v) {
    return {v.x(), v.y(), v.z()};
  }

  inline static aiVector3D toAiVector3D(const Vec3& v) {
    return {v.x(), v.y(), v.z()};
  }

  inline static Vec3 fromBtVec3(const btVector3& v) {
    return {v.x(), v.y(), v.z()};
  }

  inline static Vec3 fromAiVec3(const aiVector3D& v) {
    return {v.x, v.y, v.z};
  }

  inline static btQuaternion toBtQuat(const Quat& q) {
    return {q.x(), q.y(), q.z(), q.scalar()};
  }

  inline static Mat4 fromAiMat4(const aiMatrix4x4& m) {
    // Qt Matrix4x4 takes values as row-major order.
    Mat4 out{m.a1, m.a2, m.a3, m.a4,
             m.b1, m.b2, m.b3, m.b4,
             m.c1, m.c2, m.c3, m.c4,
             m.d1, m.d2, m.d3, m.d4};
    return out.transposed();
  }

  inline static Quat fromBtQuat(const btQuaternion& q) {
    return {q.w(), q.x(), q.y(), q.z()};
  }

  inline static Quat fromAiQuat(const aiQuaternion& q) {
    return {q.w, q.x, q.y, q.z};
  }

  static const Mat4 Mat4Identity;

  static const Quat QuatIdentity;

  static Mat4 ortho(Real left, Real right, Real bottom, Real top, Real znear, Real zfar);

  static Mat4 perspective(Real fov, Real aspect, Real znear, Real zfar);

  static Mat4 lookAtMatrix(const Vec3& eye, const Vec3& center, const Vec3& up);

  static Vec3 lerp(const Vec3& start, const Vec3& end, float t);

  static Mat4 mat4FromTRS(const Vec3& translation, const Quat& rotation, const Vec3& scale);

  static Mat4 mat4FromSRT(const Vec3& translation, const Quat& rotation, const Vec3& scale);

  static Mat4 mat4FromRTS(const Vec3& translation, const Quat& rotation, const Vec3& scale);

  static Quat lookAtQuaternion(const Vec3& eye, const Vec3& center, const Vec3& up);

  static Vec3 extractTranslation(const Mat4& mat);

  static Vec3 extractScale(const Mat4& mat);

  static Quat extractRotationQuat(const Mat4& mat);

  static Mat3 extractRotationMat3(const Mat4& mat);

  static Vec3 extractAxisX(const Mat4& mat);

  static Vec3 extractAxisY(const Mat4& mat);

  static Vec3 extractAxisZ(const Mat4& mat);

  static Real clamp(Real value, Real low, Real high);
};

#endif  /* !MOTEUR_DE_JEUX_SRC_CORE_MATH_MATH_HPP */
