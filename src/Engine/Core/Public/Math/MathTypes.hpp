#ifndef MOTEUR_DE_JEUX_SRC_CORE_PUBLIC_MATH_TYPES_HPP
#define MOTEUR_DE_JEUX_SRC_CORE_PUBLIC_MATH_TYPES_HPP

/* =================== Math types =================== */
#include <QtMath>
#include <QVector3D>
#include <QVector2D>
#include <QVector4D>
#include <QMatrix2x2>
#include <QMatrix3x3>
#include <QMatrix4x4>
#include <QQuaternion>
#include <LinearMath/btVector3.h>
#include <LinearMath/btQuaternion.h>
#include <assimp/vector3.h>
#include <assimp/quaternion.h>
#include <assimp/matrix3x3.h>
#include <assimp/matrix4x4.h>

typedef QVector2D  Vec2;
typedef QVector3D  Vec3;
typedef QVector4D  Vec4;
typedef QMatrix2x2 Mat2;
typedef QMatrix3x3 Mat3;
typedef QMatrix4x4 Mat4;
typedef QQuaternion Quat;
typedef float Real;
class Transform;
class BoundingBox;

/**
 * Transformation space.
 */
enum class ESpace {
  World, /**< Transformation happens in world space. */
  Local, /**< Transformation happens in local space. (relative to parent). */
};

using EulerAngles = Vec3;

struct BBox {
  Vec3 min;
  Vec3 max;
};

#include "BoundingBox.hpp"

#endif  /* !MOTEUR_DE_JEUX_SRC_CORE_PUBLIC_MATH_TYPES_HPP */
