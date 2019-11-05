#ifndef MOTEURDEJEUX_SRC_ENGINERENDERER_HPP
#define MOTEURDEJEUX_SRC_ENGINERENDERER_HPP

#include "Types.hpp"
#include <QtGui/QOpenGLFunctions>
#include <QKeyEvent>
#include <QQuickWindow>

class EngineRenderer : public QObject, protected QOpenGLFunctions {
  Q_OBJECT
public:
  explicit EngineRenderer(QObject* parent = nullptr);
  ~EngineRenderer() override;

  void initializeGL();

  void keyPressEvent(QKeyEvent* event);
  void setWindow(QQuickWindow* window) { m_window = window; }

public slots:
  void render();
  void resizeGL(Int32 w, Int32 h);

protected:
  UniquePtr<SceneManager> m_scn_mgr;

  OpenGLVAO m_vao;
  Array<OpenGLBuffer> m_vbos;
  OpenGLProgram m_program;

  Camera* m_camera;
  int m_proj_mat_loc;
  int m_model_view_mat_loc;
  int m_normal_mat_loc;
  int m_light_pos_loc;
  Mat4 m_proj;
  Mat4 m_model;

  std::unique_ptr<QOpenGLTexture> m_grass_texture;
  std::unique_ptr<QOpenGLTexture> m_rock_texture;
  std::unique_ptr<QOpenGLTexture> m_snow_rock_texture;
  std::unique_ptr<QOpenGLTexture> m_ocean_texture;

  QQuickWindow* m_window;

  //OpenGLBuffer m_vbo;
  //OpenGLBuffer m_ebo;
  //OpenGLTexture* m_texture;
  //QOpenGLShaderProgram m_program;
  //QMatrix4x4 m_projection;
};


#endif //MOTEURDEJEUX_SRC_ENGINERENDERER_HPP
