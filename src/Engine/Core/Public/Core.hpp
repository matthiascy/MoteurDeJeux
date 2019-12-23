#ifndef MOTEUR_DE_JEUX_SRC_CORE_CORE_HPP
#define MOTEUR_DE_JEUX_SRC_CORE_CORE_HPP

#include <QMetaType>
#include "Core/Public/Math/Math.hpp"
#include "Memory.hpp"
#include "Handle.hpp"
#include "Container.hpp"
#include "Handle.hpp"

#include <QtCore/QElapsedTimer>

class Object;

#include <QPixmap>

/* =================== Memory =================== */
#include <QSharedPointer>

template <typename T>
using SharedPtr = QSharedPointer<T>;

template <typename T>
using UniquePtr = std::unique_ptr<T>;

#include <QtCore/QUuid>

typedef QUuid Uuid;

#include <QtGui/QImage>

typedef QImage Image;

struct VertexP {
  Vec3 position;
};

struct VertexPT {
  Vec3 position;
  Vec2 texCoord;
};

typedef QColor Color;

#include <random>

enum class ELightType {
  Point,
  Directional,
  Spot,
  Count,
};


enum class InputState {
  Invalid,
  Registered,
  Unregistered,
  Triggered,
  Pressed,
  Released
};

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

#include "Object.hpp"

#include "Utils.hpp"

#endif  /* !MOTEUR_DE_JEUX_SRC_CORE_CORE_HPP */
