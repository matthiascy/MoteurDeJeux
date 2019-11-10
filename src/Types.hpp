#ifndef TYPES_HPP
#define TYPES_HPP

/* =================== Basic types =================== */
#include <QObject>
#include <cstdint>
typedef std::int8_t   Int8;
typedef std::int32_t  Int32;
typedef std::int64_t  Int64;

typedef std::uint8_t  UInt8;
typedef std::uint32_t UInt32;
typedef std::uint64_t UInt64;

/* =================== Math types =================== */
#include <QtMath>
#include <QVector3D>
#include <QVector2D>
#include <QVector4D>
#include <QMatrix2x2>
#include <QMatrix3x3>
#include <QMatrix4x4>
#include <QQuaternion>

typedef QVector2D   Vec2;
typedef QVector3D   Vec3;
typedef QVector4D   Vec4;
typedef QMatrix2x2 Mat2;
typedef QMatrix3x3 Mat3;
typedef QMatrix4x4 Mat4;
typedef QQuaternion Quat;
typedef qreal Real;

struct BBox {
  Vec3 min;
  Vec3 max;
  Vec3 size;
};

namespace vec3 {
  static const Vec3 Zero = Vec3(0.0, 0.0, 0.0);
  inline Vec3 mkVec3(Real a) { return Vec3(a, a, a); }
  static const Vec3 AxisX = Vec3(1.0, 0.0, 0.0);
  static const Vec3 AxisY = Vec3(0.0, 1.0, 0.0);
  static const Vec3 AxisZ = Vec3(0.0, 0.0, 1.0);
}

namespace quat {
  static const Quat Identity = Quat(1.0f, 0.0f, 0.0f, 0.0f);
}

/* =================== Containers types =================== */
#include <QVector>
#include <QString>
#include <QHash>

template <typename Key, typename Value>
using HashMap = QHash<Key, Value>;

template <typename T>
using Array = QVector<T>;

typedef QString     String;
typedef QStringView StringView;

/* =================== Memory =================== */
#include <QSharedPointer>

template <typename T>
using SharedPtr = QSharedPointer<T>;

template <typename T>
using UniquePtr = std::unique_ptr<T>;

namespace memory {
  template <typename T, typename... Args>
  inline auto mkUnique(Args&&... args) -> decltype(std::make_unique<T>(std::forward<Args>(args)...)) {
    return std::make_unique<T>(std::forward<Args>(args)...);
  }

  template <typename T, typename... Args>
  inline auto mkShared(Args&&... args) -> decltype(std::make_shared<T>(std::forward<Args>(args)...)) {
    return std::make_shared<T>(std::forward<Args>(args)...);
  }
}

/* =================== GL related =================== */
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>

typedef QOpenGLBuffer            OpenGLBuffer;
typedef QOpenGLVertexArrayObject OpenGLVAO;
typedef QOpenGLTexture           OpenGLTexture;
typedef QOpenGLShaderProgram     OpenGLProgram;
typedef QOpenGLShader            OpenGLShader;

/* ==================== Enums ==================== */
enum class ETransformSpace {
  Local,   // Transform is relative to the local space
  Parent,  // Transform is relative to the parent space
  World,   // Transform is relative to the world space
};

enum class ELightType {
  Point,
  Directional,
  Spot,
  Count,
};

enum class EResourceType {
  Texture,
  Audio,
  Mesh,
};

/* ==================== Forward declarations ==================== */
class Camera;
class Light;
class Node;
class NameGenerator;
class ResourceManager;
class Mesh;
class GameObject;
class SceneNode;
class SceneManager;

struct ResourceHandle {
  UInt64 idx;
  EResourceType type;
};

#include <QImage>

typedef QImage Image;

struct MaterialInfo {
  String name;
  Vec3 ambient;
  Vec3 diffuse;
  Vec3 specular;
  float shininess;
};

struct LightInfo {
  Vec4 position;
  Vec3 intensity;
};

class EngineRenderer;

struct VertexP {
  Vec3 position;
};

struct VertexPT {
  Vec3 position;
  Vec2 texCoord;
};

#include <random>

// Meta types declaration
Q_DECLARE_METATYPE(Int8)
Q_DECLARE_METATYPE(Int32)
Q_DECLARE_METATYPE(Int64)
Q_DECLARE_METATYPE(UInt8)
Q_DECLARE_METATYPE(UInt32)
Q_DECLARE_METATYPE(UInt64)
Q_DECLARE_METATYPE(Real)
Q_DECLARE_METATYPE(Vec2)
Q_DECLARE_METATYPE(Vec3)
Q_DECLARE_METATYPE(Vec4)
Q_DECLARE_METATYPE(Quat)

#endif // TYPES_HPP