#include "OpenGLWindow.hpp"
//#include "Graphics/OpenGLError.hpp"
#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QPushButton>
#include <QDesktopWidget>
#include <QApplication>
#include <QMessageBox>

OpenGLWindow::OpenGLWindow()
    : QOpenGLWindow(), AbstractWindow(), m_clear_color{60, 60, 69, 255}
{

}

void OpenGLWindow::initializeGL()
{
  initializeOpenGLFunctions();
  connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &OpenGLWindow::teardownGL, Qt::DirectConnection);
  print_version_information();

  glEnable(GL_CULL_FACE);
  glClearColor(m_clear_color.redF(), m_clear_color.greenF(), m_clear_color.blueF(), m_clear_color.alphaF());
  {
    // Shader program creation
    m_program = new OpenGLProgram();
    m_program->addShaderFromSourceFile(OpenGLShader::Vertex, ":/Shaders/");
    m_program->addShaderFromSourceFile(OpenGLShader::Fragment, ":/Shaders/");
    m_program->link();
    m_program->bind();

    // Buffer creation
    m_vbo.create();
    m_vbo.bind();
    m_vbo.setUsagePattern(OpenGLBuffer::StaticDraw);
    //m_vbo.allocate()

    // Vertex Array Object Creation
    m_vao.create();
    m_vao.bind();
    m_program->enableAttributeArray(0);
    m_program->enableAttributeArray(1);
    //...set attribute buffer

    m_vao.release();
    m_vbo.release();
    m_program->release();
  }
}

void OpenGLWindow::resizeGL(Int32 width, Int32 height)
{

}

void OpenGLWindow::print_version_information()
{
  String gl_type, gl_version, gl_profile;
  gl_type = context()->isOpenGLES() ? "OpenGL ES" : "OpenGL Desktop";
  gl_version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
  switch (format().profile()) {
    case QSurfaceFormat::NoProfile: {
      gl_profile = "NoProfile";
    } break;

    case QSurfaceFormat::CoreProfile: {
      gl_profile = "CoreProfile";
    } break;

    case QSurfaceFormat::CompatibilityProfile: {
      gl_profile = "CompatibleProfile";
    } break;
  }

  qDebug() << qPrintable(gl_type) << qPrintable(gl_version) << "(" << qPrintable(gl_profile) << ")";
}

/* ---------- Events ---------- */
bool OpenGLWindow::event(QEvent* event)
{
  /*
  if (event->type() == OpenGLError::type()) {
    errorEventGL(dynamic_cast<OpenGLError*>(event));
    return true;
  }
   */
  return QPaintDeviceWindow::event(event);
}

//void OpenGLWindow::errorEventGL(OpenGLError* error)
//{
//  qFatal("%s::%s => Returned an error!", error->callerName(), error->functionName());
//}

void OpenGLWindow::keyPressEvent(QKeyEvent* event)
{
  if (event->isAutoRepeat()) {
    event->ignore();
  } else {
    //Input::registerKeyPress(event->key());
  }
}
