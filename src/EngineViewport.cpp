#include "EngineViewport.hpp"
#include "EngineRenderer.hpp"
#include <QQuickWindow>

EngineViewport::EngineViewport()
    : m_renderer{nullptr},
      m_angular_speed(0),
      m_fps(0),
      m_last_time(QTime::currentTime()),
      m_last_time2(QTime::currentTime())
{
  m_fps_timer.setInterval(static_cast<int>(1.0/60.0) * 1000.0);
  connect(&m_fps_timer, &QTimer::timeout, this, [this](){
    QQuickItem::update();
    window()->update();
    //static UInt32 frameCount = 0;
    //frameCount++;
    //if (m_last_time.msecsTo(QTime::currentTime()) > 1000) {
//    setFps(frameCount);
//    m_last_time = QTime::currentTime();
//    frameCount = 0;
//  }
    qDebug() << "elapsed: " << m_last_time2.msecsTo(QTime::currentTime());
    m_last_time2 = QTime::currentTime();
  });


  connect(this, &QQuickItem::windowChanged, this, [this](QQuickWindow* window){
    if (window) {
      connect(window, &QQuickWindow::beforeSynchronizing, this, &EngineViewport::sync,
              Qt::DirectConnection);
      connect(window, &QQuickWindow::sceneGraphInvalidated, this, &EngineViewport::cleanup,
              Qt::DirectConnection);
      window->setClearBeforeRendering(false);
    }
    else return;
  });
}

EngineViewport::~EngineViewport() = default;

void EngineViewport::sync()
{
  if (!m_renderer) {
    m_renderer = new EngineRenderer();
    m_renderer->initializeGL();
    m_renderer->resizeGL(window()->width(), window()->height());
    connect(window(), &QQuickWindow::beforeRendering, this, [this](){
      m_renderer->render();
    }, Qt::DirectConnection);

    connect(window(), &QQuickWindow::afterRendering, this, [this](){
      //渲染后调用，计算fps
      static int frame = 0;
      frame++;
      if (m_last_time.msecsTo(QTime::currentTime()) > 1000)
      {
        setFps(frame);
        m_last_time = QTime::currentTime();
        frame = 0;
      }
    }, Qt::DirectConnection);

    connect(window(), &QQuickWindow::widthChanged, this, [this](){
      m_renderer->resizeGL(window()->width(), window()->height());
    });

    connect(window(), &QQuickWindow::heightChanged, this, [this](){
      m_renderer->resizeGL(window()->width(), window()->height());
    });
  }
  m_renderer->setWindow(window());
}

void EngineViewport::cleanup()
{
  qDebug() << "EngineViewport cleanup...";
  if (m_renderer) {
    delete m_renderer;
    m_renderer = nullptr;
  }
}

void EngineViewport::timerEvent(QTimerEvent *event)
{
  Q_UNUSED(event)
  setAngularSpeed(m_angular_speed * 0.99);

  if (angularSpeed() <= 0.01)
    setAngularSpeed(0.0);
  else
  {
    m_rotation = QQuaternion::fromAxisAndAngle(m_rotate_axis, m_angular_speed) * m_rotation;
    window()->update();
  }
}

void EngineViewport::onMousePress(int x, int y)
{
  m_mouse_start_pos = QVector2D(x, y);
}

void EngineViewport::onMouseRelease(int x, int y)
{
  QVector2D diff = QVector2D(x, y) - m_mouse_start_pos;
  QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();
  qreal acc = diff.length() / 100.0;
  setRotateAxis((rotateAxis() * angularSpeed() + n * acc).normalized());
  setAngularSpeed(angularSpeed() + acc);
}

Int32 EngineViewport::fps() const
{
  return m_fps;
}

void EngineViewport::setFps(Int32 fps)
{
  if (fps == m_fps) return;
  m_fps = fps;
  emit fpsChanged();
}

Real EngineViewport::angularSpeed() const {
  return m_angular_speed;
}

void EngineViewport::setAngularSpeed(Real angularSpeed)
{
  if (angularSpeed == m_angular_speed) return;
  m_angular_speed = angularSpeed;
  emit angularSpeedChanged();
}

Vec3 EngineViewport::rotateAxis() const
{
  return m_rotate_axis;
}

void EngineViewport::setRotateAxis(const Vec3 & rotateAxis)
{
  if (rotateAxis == m_rotate_axis) return;
  m_rotate_axis = rotateAxis;
  emit rotateAxisChanged();
}

void EngineViewport::keyPressEvent(QKeyEvent* event)
{
  /*
  switch (event->key()) {
    case Qt::Key_Z: {
      m_camera->pos += QVector3D(0.0f, 0.0f, -20.0f);
    } break;

    case Qt::Key_S: {
      m_camera->pos += QVector3D(0.0f, 0.0f, 20.0f);
    } break;

    case Qt::Key_Q: {
      m_camera->pos += QVector3D(-20.0f, 0.0f, 0.0f);
    } break;

    case Qt::Key_D: {
      m_camera->pos += QVector3D(20.0f ,0.0f, 0.0f);
    } break;
  }
   */
  m_renderer->keyPressEvent(event);
}
