#ifndef TYPES_HPP
#define TYPES_HPP

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

/* ==================== Input ==================== */
enum class InputState {
  Invalid,
  Registered,
  Unregistered,
  Triggered,
  Pressed,
  Released
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
class Scene;
class InputSystem;

/* ==================== Graphics ==================== */
class OpenGLWindow;

#include <QImage>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>

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

typedef QColor Color;

/* ==================== Physics ==================== */
class Collider;

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