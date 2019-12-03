#ifndef ASSET_PREVIEWER_HPP
#define ASSET_PREVIEWER_HPP

#include <QColor>
#include <vector>
#include <QMatrix4x4>
#include <QStringList>
#include <QPaintEvent>
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_4_0_Core>
//#include <Framework/Vertex.hpp>
//#include <Framework/Camera.hpp>
#include "Vertex.hpp"
#include "EditorCamera.hpp"

//#include <Framework/Graphics/Types.hpp>
//#include <Framework/Graphics/Model3D.hpp>

class QString;
class QFileInfo;
class QWheelEvent;

class AssetPreviewer : public QOpenGLWidget {

  Q_OBJECT

  struct MouseState {
    bool r_btn_pressed;
    bool l_btn_pressed;
    bool moving;
    QVector2D prev_pos;
    QVector2D curr_pos;
  };

  enum AssetType {
    Texture, Model, None
  };

public:
  AssetPreviewer();
  ~AssetPreviewer() override;

protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

  /**
   *  MouseRight button - smooth zoom
   *  MouseLeft  button - arcball camera
   *  MouseWheel        - zoom
   */
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void wheelEvent(QWheelEvent* event) override;

public slots:
  void preview(const QFileInfo& file_info);

public:
  void SetStyle();

private:
  QColor _clearColor;
  QMatrix4x4 _proj_mat;
  QMatrix4x4 _view_mat;
  QMatrix4x4 _model_mat;
  EditorCamera _camera;
  MouseState _mouse_state;

  uint vao, vbo, ebo;

  // shader for rendering image files.
  QOpenGLShaderProgram _texture_program;
  uint _tex_texture;

  // shader for rendering model files.
  QOpenGLShaderProgram _model_program;

  // QImage doesn't support TIFF and TGA
  static const QStringList image_file_ext;
  static const QStringList model_file_ext;

  QString _tip_text;

  std::function<void()> render;

  AssetType _asset_type;

  float _ortho_width;

  uint  _num_meshes;
  uint* _model_vaos;
  uint* _model_vbos;
  uint* _model_ebos;

  QOpenGLFunctions_4_0_Core* _fn;

private:
  void preview_model(const QFileInfo& file);
  void preview_texture(const QFileInfo& file);
};


#endif // ASSET_PREVIEWER_HPP
