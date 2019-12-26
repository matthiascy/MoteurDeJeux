#include <QPainter>
#include <QModelIndex>
#include <QFileInfo>
#include <QImage>
#include <Core/Public/Core.hpp>
#include "AssetPreviewer.hpp"
//#include <Framework/Graphics/Model3D.hpp>
#include <iostream> // debug

const QStringList AssetPreviewer::image_file_ext = {
  "bmp", "gif", "jpg", "jpeg", "png", "pbm", "pgm", "ppm"
};

const QStringList AssetPreviewer::model_file_ext = {
  "obj", "fbx", "off", "dae", "ply", "stl", "3ds"
};

AssetPreviewer::AssetPreviewer()
  : _clearColor{112, 112, 112, 255},
    _camera{{0.0f, 5.0f, 8.0f}, {0.0f, 1.0f, 0.0f}, -90.0f, -45.0f},
    _mouse_state{false, false, false, {0.0f, 0.0f}, {0.0f, 0.0f}},
    _texture_program{QOpenGLContext::currentContext()},
    _model_program{QOpenGLContext::currentContext()},
    _num_meshes{},
    _model_vaos{nullptr},
    _model_vbos{nullptr},
    _model_ebos{nullptr},
    _fn{nullptr}
{
  _proj_mat.setToIdentity();
  _view_mat.setToIdentity();
  _model_mat.setToIdentity();
  _tip_text = tr("Drag with the left mouse button to rotate.");
  render = []{};
  _asset_type = AssetType::None;
  _ortho_width = 5.0f;

  connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
}

AssetPreviewer::~AssetPreviewer()
{
  makeCurrent();
  _fn->glDeleteVertexArrays(1, &vao);
  _fn->glDeleteBuffers(1, &vbo);
  _fn->glDeleteBuffers(1, &ebo);
  _fn->glDeleteTextures(1, &_tex_texture);
  doneCurrent();
}

void AssetPreviewer::initializeGL()
{
  _fn = QOpenGLContext::currentContext()->currentContext()->versionFunctions<QOpenGLFunctions_4_0_Core>();
  _fn->initializeOpenGLFunctions();

  if (_texture_program.create()) {
    std::cout << "AssetPreviewer: Texture Shader Created. [id: " << _texture_program.programId() << "]" << std::endl;

    if (!_texture_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Shaders/previewimage.vert"))
      close();

    if (!_texture_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Shaders/previewimage.frag"))
      close();

    if (!_texture_program.link())
      close();
  }

  if (_model_program.create()) {
    std::cout << "AssetPreviewer: Model Shader Created. [id: " << _model_program.programId() << "]" << std::endl;

    if (!_model_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Shaders/previewimage.vert"))
      close();

    if (!_model_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Shaders/previewimage.frag"))
      close();

    if (!_model_program.link())
      close();
  }

  _fn->glGenVertexArrays(1, &vao);
  _fn->glGenBuffers(1, &vbo);
  _fn->glGenBuffers(1, &ebo);
  _fn->glGenTextures(1, &_tex_texture);

  //-----------
  VertexLayoutPT vertices[] = {
    {{-0.2f,  0.2f, 0.0f}, {0.0f, 1.0f}},
    {{-0.2f, -0.2f, 0.0f}, {0.0f, 0.0f}},
    {{ 0.2f, -0.2f, 0.0f}, {1.0f, 0.0f}},
    {{ 0.2f,  0.2f, 0.0f}, {1.0f, 1.0f}}
  };

    // triangle strip
  uint indices[] = {
    0, 1, 3, 2
  };

  _fn->glBindVertexArray(vao);
  _fn->glBindBuffer(GL_ARRAY_BUFFER, vbo);
  _fn->glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(VertexLayoutPT), &vertices[0], GL_STATIC_DRAW);
  _fn->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  _fn->glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(uint), &indices[0], GL_STATIC_DRAW);
  _fn->glEnableVertexAttribArray(0);
  _fn->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexLayoutPT), nullptr);
  _fn->glEnableVertexAttribArray(1);
  _fn->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexLayoutPT), reinterpret_cast<void*>(offsetof(VertexLayoutPT, texCoord)));
  _fn->glBindVertexArray(0);

  _fn->glEnable(GL_DEPTH_TEST);
}

void AssetPreviewer::resizeGL(int w, int h)
{
  float ratio = static_cast<float>(w) / static_cast<float>(h ? h : 1.0f);
  const float znear = 0.1f;
  const float zfar  = 1024.0f;
  const float fov    = 45.0f;

  _proj_mat.setToIdentity();
  _proj_mat.perspective(fov, ratio, znear, zfar);
  _view_mat = _camera.ViewMatrix();
}

void AssetPreviewer::paintGL()
{
  _fn->glClearColor(_clearColor.redF(), _clearColor.greenF(), _clearColor.blueF(), _clearColor.alphaF());
  _fn->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  render();

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  {
    QFontMetrics metrics = QFontMetrics(font());
    int border = qMax(4, metrics.leading());

    QRect rect = metrics.boundingRect(0, 0, width() - 2*border, int(height()*0.125),
                                      Qt::AlignCenter | Qt::TextWordWrap, _tip_text);
    painter.fillRect(QRect(0, 0, width(), rect.height() + 2*border),
                     QColor(127, 127, 127, 127));
    painter.setPen(Qt::white);
    painter.fillRect(QRect(0, 0, width(), rect.height() + 2*border),
                     QColor(0, 0, 0, 127));
    painter.drawText((width() - rect.width())/2, border, rect.width(), rect.height(),
                     Qt::AlignCenter | Qt::TextWordWrap, _tip_text);
  }
  painter.end();
}

void AssetPreviewer::mousePressEvent(QMouseEvent* event)
{
  switch (event->button()) {
    case Qt::MouseButton::RightButton: {
      _mouse_state.r_btn_pressed = true;
      _mouse_state.prev_pos      = _mouse_state.curr_pos;
      _mouse_state.curr_pos      = QVector2D(event->localPos());
    } break;

    case Qt::MouseButton::LeftButton: {
      _mouse_state.l_btn_pressed = true;
      _mouse_state.prev_pos      = _mouse_state.curr_pos;
      _mouse_state.curr_pos      = QVector2D(event->localPos());
    } break;

    default:
      break;
  }
}

void AssetPreviewer::mouseReleaseEvent(QMouseEvent* event)
{
  switch (event->button()) {
    case Qt::MouseButton::RightButton: {
      _mouse_state.r_btn_pressed = false;
    } break;

    case Qt::MouseButton::LeftButton: {
      _mouse_state.l_btn_pressed = false;
    } break;

    default:
      break;
  }
}

void AssetPreviewer::mouseMoveEvent(QMouseEvent* event)
{
  _mouse_state.moving = true;
  _mouse_state.prev_pos = _mouse_state.curr_pos;
  _mouse_state.curr_pos = QVector2D(event->localPos());

  if (_mouse_state.l_btn_pressed && !_mouse_state.r_btn_pressed) {

  } else if (!_mouse_state.l_btn_pressed && _mouse_state.r_btn_pressed) {

  }
}

void AssetPreviewer::wheelEvent(QWheelEvent* event)
{
  QPoint num_degrees = event->angleDelta() / 8;

  _camera.pos += (num_degrees.y() >= 0 ? _camera.front : -_camera.front);

  event->accept();
}

void AssetPreviewer::SetStyle()
{
  this->setMinimumSize(240, 240);
  //this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  this->setStyleSheet("background: #414345; color: #EEEEEE;");
}

void AssetPreviewer::preview(const QFileInfo& file_info)
{
  if (file_info.isFile()) {

    QString ext = file_info.suffix();

    if (AssetPreviewer::image_file_ext.contains(ext, Qt::CaseInsensitive)) {

      preview_texture(file_info);

    } else if (AssetPreviewer::model_file_ext.contains(ext, Qt::CaseInsensitive)) {

      preview_model(file_info);

    } else {

      std::cout << "AssetPreviewer: " << file_info.fileName().toStdString() << " not viewable!" << std::endl;

    }
  }
}

void AssetPreviewer::preview_model(const QFileInfo &file)
{
  /*
  Model3D model(file.absoluteFilePath().toStdString().c_str());

  // prepare vaos, vbos, ebos
  if (_model_vaos)
      delete [] _model_vaos;
    if (_model_vbos)
      delete [] _model_vbos;
    if (_model_ebos)
      delete [] _model_ebos;

    _model_vaos = nullptr;
    _model_vbos = nullptr;
    _model_ebos = nullptr;

    _num_meshes = model.Meshes().size();

    _model_vaos = new uint [_num_meshes];
    _model_vbos = new uint [_num_meshes];
    _model_ebos = new uint [_num_meshes];

    _fn->glGenVertexArrays(_num_meshes, _model_vaos);
    _fn->glGenBuffers(_num_meshes, _model_vbos);
    _fn->glGenBuffers(_num_meshes, _model_ebos);


    for (uint i = 0; i < _num_meshes; ++i) {
      _fn->glBindVertexArray(_model_vaos[i]);
      _fn->glBindBuffer(GL_ARRAY_BUFFER, _model_vbos[i]);
      _fn->glBufferData(GL_ARRAY_BUFFER,
                        model.Meshes()[i].vertices.size() * sizeof(VertexLayoutP),
                        &model.Meshes()[i].vertices[0], GL_STATIC_DRAW);
      _fn->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _model_ebos[i]);
      _fn->glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                        model.Meshes()[i].indices.size() * sizeof(uint),
                        &model.Meshes()[i].indices[0], GL_STATIC_DRAW);
      _fn->glEnableVertexAttribArray(0);
      _fn->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexLayoutP), nullptr);
      _fn->glBindVertexArray(0);
    }

  std::cout << "Model preparation finished." << std::endl;

  render = [&]() -> void {
    _model_program.bind();

    for (uint i = 0; i < _num_meshes; ++i) {
      _fn->glBindVertexArray(_model_vaos[i]);
      _fn->glDrawElements(GL_TRIANGLES, model.Meshes()[i].NumIndices, GL_UNSIGNED_INT, nullptr);
      _fn->glBindVertexArray(0);
    }

    std::cout << "render model" << std::endl;
    _model_program.release();
  };

  update();
   */
}

void AssetPreviewer::preview_texture(const QFileInfo &file)
{
  _tip_text = file.fileName();

  QImage img(file.absoluteFilePath());
  img = img.mirrored();

  float ratio = static_cast<float>(img.height()) / static_cast<float>(img.width());
  float w = 2.0f;
  float h = w * ratio;

  _ortho_width = w > h ? w : h;

  std::cout << "w " << w << std::endl;
  std::cout << "h " << h << std::endl;

  // 0 -- 3
  // |    |
  // 1 -- 2

  VertexLayoutPT vertices[] = {
    {{-w/2.0f,  h/2.0f, 0.0f}, {0.0f, 1.0f}},
    {{-w/2.0f, -h/2.0f, 0.0f}, {0.0f, 0.0f}},
    {{ w/2.0f, -h/2.0f, 0.0f}, {1.0f, 0.0f}},
    {{ w/2.0f,  h/2.0f, 0.0f}, {1.0f, 1.0f}}
  };

  _fn->glActiveTexture(GL_TEXTURE0);
  _fn->glBindTexture(GL_TEXTURE_2D, _tex_texture);
  _fn->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  _fn->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  _fn->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  _fn->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  _fn->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width(), img.height(), 0, GL_RGBA,
               GL_UNSIGNED_BYTE, img.convertToFormat(QImage::Format_RGBA8888).bits());
  _fn->glGenerateMipmap(GL_TEXTURE_2D);

  _fn->glBindVertexArray(vao);

  _fn->glBindBuffer(GL_ARRAY_BUFFER, vbo);
  _fn->glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(VertexLayoutPT), &vertices[0], GL_STATIC_DRAW);

  _fn->glEnableVertexAttribArray(0);
  _fn->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexLayoutPT), nullptr);

  _fn->glBindVertexArray(0);
  _asset_type = AssetType::Texture;

  _camera.pos = {0.0f, 0.0f, 3.0f};
  _camera.pitch = 0.0f;
  //_camera{{0.0f, 5.0f, 8.0f}, {0.0f, 1.0f, 0.0f}, -90.0f, -45.0f},

  render = [&]() -> void {
    _texture_program.bind();
    _view_mat = _camera.ViewMatrix();
    _model_mat.setToIdentity();
    _texture_program.setUniformValue("mvp", _proj_mat * _view_mat * _model_mat);

    _fn->glBindVertexArray(vao);
    _fn->glBindTexture(GL_TEXTURE_2D, _tex_texture);
    _fn->glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, nullptr);
    _fn->glBindTexture(GL_TEXTURE_2D, 0);
    _fn->glBindVertexArray(0);

    _texture_program.release();
  };
}
