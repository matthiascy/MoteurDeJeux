#ifndef VIEWPORT_HPP
#define VIEWPORT_HPP

#include <QOpenGLWidget>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QOpenGLExtraFunctions>
#include <QOpenGLFunctions_4_0_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
//#include <Framework/Camera.hpp>
#include "EditorCamera.hpp"
//#include <Framework/Vertex.hpp>
#include "Vertex.hpp"
//#include <Framework/Graphics/OglContext.hpp>

class Viewport : public QOpenGLWidget {

  Q_OBJECT

  struct MouseState {
    bool r_btn_pressed;
    bool l_btn_pressed;
    bool m_btn_pressed;
    bool moving;
    QVector2D click_pos;
    QVector2D prev_pos;
    QVector2D curr_pos;
  };

  struct KeyState {
    bool alt_pressed;
  };

  enum ToolType {
    Select, Move, Scale, Rotate, Placement
  };

public:
  //explicit Viewport(OglContext* ctx);
  explicit Viewport();
  ~Viewport() override;

protected:
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void mouseDoubleClickEvent(QMouseEvent* event) override;

  void wheelEvent(QWheelEvent* event) override;

  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;

  void timerEvent(QTimerEvent* event) override;

  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

public:
  void SetClearColor(const QColor& color);
  void PrintContextInformation();
  void Toogle(ToolType type);

private:
  //OglContext* _ctx;
  QColor clear_color;
  QMatrix4x4 proj_mat;
  QMatrix4x4 view_mat;
  QMatrix4x4 model_mat;
  EditorCamera _camera;
  MouseState _mouseState;
  KeyState _keyState;
  float _gridStep = 1.0;
  unsigned _gridSize = 16;
  QOpenGLFunctions_4_0_Core* _fn;

private:
  QVBoxLayout* layout;
  QMenuBar* menubar;
  QMenu* menu_view;
  QMenu* menu_shading;
  QMenu* menu_lighting;

private:
  void render_viewcube();
  void render_indicator();
  void render_grid();
  void init_viewcube_indicator();
  void init_ground_grid(int n);
  void draw_selection_area(const QVector2D& p0, const QVector2D& p1);

  std::vector<gtd::Vertex> vertices;
  std::vector<unsigned int> indices;
  unsigned int vaos[2];
  unsigned int vbos[2];
  unsigned int ebos[2];
  unsigned int texs[2];
  //unsigned int vao, vbo, ebo,
  unsigned int viewcube_texture;

  QOpenGLShaderProgram program;
  QOpenGLShaderProgram _gridProgram;

  void init();
  void setupMenu();

public:
  void SelectCheck(bool checked_p);
  void MoveCheck(bool checked_p);
  void RotateCheck(bool checked_p);
  void ScaleCheck(bool checked_p);
  void PlaceCheck(bool checked_p);
};

#endif // VIEWPORT_HPP
