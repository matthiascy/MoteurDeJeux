#ifndef WINDOW_H
#define WINDOW_H

#include "../../Types.hpp"
#include <QWidget>
#include <QtGui/QtGui>
#include <Engine/Core/CoreTypes.hpp>
#include "OpenGLError.hpp"
#include "Window.hpp"


class GLWidget;
class MainWindow;

class OpenGLWindow : public QOpenGLWindow, protected QOpenGLFunctions {
    Q_OBJECT

public:
  OpenGLWindow();
  ~OpenGLWindow() override { };
  void initializeGL() override;
  void resizeGL(Int32 width, Int32 height) override;
  void paintGL() override {  };

protected slots:
  void teardownGL() { };
  void update() { };

protected:
  bool event(QEvent* event) override;
  void errorEventGL(OpenGLError* error);
  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override { };
  void mousePressEvent(QMouseEvent* event) override { };
  void mouseReleaseEvent(QMouseEvent* event) override { };
  void mouseMoveEvent(QMouseEvent* event) override { };
  void moveEvent(QMoveEvent* event) override { };
  void wheelEvent(QWheelEvent* event) override { };


private:
  QElapsedTimer m_frame_timer;
  UInt64        m_frame_count;
  OpenGLProgram* m_program;
  OpenGLVAO      m_vao;
  OpenGLBuffer   m_vbo;
  Color          m_clear_color;

  void print_version_information();
};

#endif