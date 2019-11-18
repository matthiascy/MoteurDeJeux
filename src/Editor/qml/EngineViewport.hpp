#ifndef MOTEURDEJEUX_SRC_QML_ENGINEVIEWPORT_HPP
#define MOTEURDEJEUX_SRC_QML_ENGINEVIEWPORT_HPP

#include "Types.hpp"
#include <QTime>
#include <QTimer>
#include <QQuickItem>
#include <QBasicTimer>

class EngineViewport : public QQuickItem {
  Q_OBJECT
  Q_PROPERTY(UInt32 fps READ fps WRITE setFps NOTIFY fpsChanged)
  Q_PROPERTY(Real angularSpeed READ angularSpeed WRITE setAngularSpeed NOTIFY angularSpeedChanged)
  Q_PROPERTY(Vec3 rotateAxis READ rotateAxis WRITE setRotateAxis NOTIFY rotateAxisChanged)

public:
  EngineViewport();
  ~EngineViewport() override;

  Int32 fps() const;
  void setFps(Int32 fps);
  Real angularSpeed() const;
  void setAngularSpeed(Real angularSpeed);
  Vec3 rotateAxis() const;
  void setRotateAxis(const Vec3& rotateAxis);

// =========
signals:
  void fpsChanged();
  void angularSpeedChanged();
  void rotateAxisChanged();

  void xRotationChanged(int angle);
  void yRotationChanged(int angle);
  void zRotationChanged(int angle);

public slots:
  void sync();
  void cleanup();
  void onMousePress(Int32 x, Int32 y);
  void onMouseRelease(Int32 x, Int32 y);
  void keyPressEvent(QKeyEvent* event) override;

protected:
  void timerEvent(QTimerEvent* event) override;

protected:
  UInt32 m_fps;
  QTime m_last_time;
  QTime m_last_time2;
  QTimer m_fps_timer;
  EngineRenderer* m_renderer;
  Vec2 m_mouse_start_pos;
  Vec3 m_rotate_axis;
  Real m_angular_speed;
  Quat m_rotation;
  Vec3 m_camera_pos;
};


#endif //MOTEURDEJEUX_SRC_QML_ENGINEVIEWPORT_HPP
