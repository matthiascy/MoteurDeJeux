#ifndef GEMTD_VERTEX_HPP
#define GEMTD_VERTEX_HPP

#include <QtGlobal> // for Q_MOVABLE_TYPE
#include <QVector3D>
#include <QVector2D>

namespace gtd {

  struct Vertex {
  public:
    QVector3D pos;
    QVector2D texCoord;
    QVector3D color;

    static const int stride;
    static const int posOffset;
    static const int colorOffset;
    static const int texCoordOffset;

  public:
    inline Vertex() { }
    inline explicit Vertex(const QVector3D& p) : pos{p} { }
    inline Vertex(const QVector3D& p, const QVector3D& c) : pos{p}, color{c} { }
    inline Vertex(const QVector3D& p, const QVector2D& t, const QVector3D& c) : pos{p}, texCoord{t}, color{c} { }
  };

}  // !namespace gtd

  Q_DECLARE_TYPEINFO(gtd::Vertex, Q_MOVABLE_TYPE);

#endif  // !VERTEX_HPP
