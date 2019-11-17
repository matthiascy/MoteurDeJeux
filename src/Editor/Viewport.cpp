#include "Viewport.hpp"
#include <iostream>
#include <QPainter>
#include <QPainterPath>
#include <QSurfaceFormat>
#include <QOpenGLContext>
#include <QMouseEvent>

Viewport::Viewport()
  : //_ctx{ctx},
    clear_color{42, 42, 50},
    _camera{{0.0f, 5.0f, 8.0f}, {0.0f, 1.0f, 0.0f}, -90.0f, -28.0f},
    _fn{nullptr},
    layout{nullptr},
    menubar{nullptr},
    menu_view{nullptr},
    menu_shading{nullptr},
    menu_lighting{nullptr},
    program{QOpenGLContext::currentContext()},
    _gridProgram{QOpenGLContext::currentContext()}
{
  _mouseState.l_btn_pressed = false;
  _mouseState.r_btn_pressed = false;
  _mouseState.m_btn_pressed = false;
  _mouseState.moving = false;

  _keyState.alt_pressed = false;

  proj_mat.setToIdentity();
  view_mat.setToIdentity();
  model_mat.setToIdentity();

  vertices.reserve(1024);
  indices.reserve(1024*3);

  connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));

  setupMenu();
}

void Viewport::init()
{
  _fn = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_0_Core>();
  _fn->initializeOpenGLFunctions();

  _fn->glEnable(GL_DEPTH_TEST);
  /*
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CW);
  */

  PrintContextInformation();

  if (program.create()) {

    std::cout << "Viewport: OpenGL Shader Program Created.[id: " << program.programId() << "]" << std::endl;

    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Shaders/viewport.vert"))
      close();

    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Shaders/viewport.frag"))
      close();

    if (!program.link())
      close();
  }

  if (_gridProgram.create()) {
    std::cout << "Viewport: Grid Shader Created. [id: " << _gridProgram.programId() << "]" << std::endl;

    if (!_gridProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Shaders/basic.vert"))
      close();

    if (!_gridProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Shaders/basic.frag"))
      close();

    if (!_gridProgram.link())
      close();
  }

  _fn->glGenBuffers(2, vbos);
  _fn->glGenBuffers(2, ebos);
  _fn->glGenVertexArrays(2, vaos);
  _fn->glGenTextures(1, &viewcube_texture);

  init_viewcube_indicator();
  init_ground_grid(_gridSize);
}

void Viewport::setupMenu()
{
  // create menu bar
  layout = new QVBoxLayout(this);
  menubar = new QMenuBar();
  menu_view = menubar->addMenu(tr("View"));
  QAction* grids = menu_view->addAction("Grids");
  QAction* gimzos = menu_view->addAction("Gimzos");
  grids->setCheckable(true);
  grids->setChecked(false);
  gimzos->setCheckable(true);
  gimzos->setChecked(false);

  menu_shading = menubar->addMenu(tr("Shading"));
  menu_lighting = menubar->addMenu(tr("Lighting"));
  layout->setMenuBar(menubar);
  menubar->setStyleSheet(tr("color: #EFEFEF; background-color: #2E2F30; border-bottom-left-radius: 6px; border-bottom-right-radius: 6px;"));
  this->setFocusPolicy(Qt::FocusPolicy::ClickFocus);
}

Viewport::~Viewport()
{
  makeCurrent();
  _fn->glDeleteVertexArrays(2, vaos);
  _fn->glDeleteBuffers(2, vbos);
  _fn->glDeleteBuffers(2, ebos);
  doneCurrent();

  if (layout)
    delete layout;

  if (menubar)
    delete menubar;

  layout = nullptr;
  menubar = nullptr;
}

void Viewport::initializeGL()
{
  init();
}

void Viewport::resizeGL(int w, int h)
{
  float ratio = static_cast<float>(w) / static_cast<float>(h ? h : 1);
  const float zNear = 0.1f;
  const float zFar  = 1024.0f;
  const float fov   = 45.0f;

  proj_mat.setToIdentity();
  proj_mat.perspective(fov, ratio, zNear, zFar);

  view_mat = _camera.ViewMatrix();
  model_mat.setToIdentity();
}

void Viewport::paintGL()
{
   _fn->glClearColor(clear_color.redF(), clear_color.greenF(), clear_color.blueF(),
               clear_color.alphaF());
   _fn->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  render_viewcube();
  render_grid();

  if (_mouseState.moving && _mouseState.l_btn_pressed) {
    draw_selection_area(_mouseState.click_pos, _mouseState.curr_pos);
  }
}


void Viewport::SetClearColor(const QColor& color)
{
  clear_color = color;
}


void Viewport::mousePressEvent(QMouseEvent* event)
{
  switch (event->button()) {
    case Qt::MouseButton::RightButton: {
      _mouseState.r_btn_pressed = true;
      _mouseState.curr_pos = QVector2D(event->localPos());
      //_mouseState.prev_pos = QVector2D(event->localPos());
    } break;

    case Qt::MouseButton::LeftButton: {
      _mouseState.l_btn_pressed = true;
      _mouseState.click_pos = QVector2D(event->localPos());
      _mouseState.curr_pos = QVector2D(event->localPos());
      //_mouseState.prev_pos = QVector2D(event->localPos());
    } break;

    case Qt::MouseButton::MiddleButton: {
      _mouseState.m_btn_pressed = true;
      _mouseState.click_pos = QVector2D(event->localPos());
      //_mouseState.prev_pos = QVector2D(event->localPos());
      _mouseState.curr_pos = QVector2D(event->localPos());
    } break;

    default:
      break;
  }
}

void Viewport::mouseReleaseEvent(QMouseEvent* event)
{
  switch (event->button()) {
    case Qt::MouseButton::RightButton: {
      _mouseState.r_btn_pressed = false;
    } break;

    case Qt::MouseButton::LeftButton: {
      _mouseState.l_btn_pressed = false;
    } break;

    case Qt::MouseButton::MiddleButton: {
      _mouseState.m_btn_pressed = false;
    } break;

    default:
      break;
  };
}

void Viewport::mouseMoveEvent(QMouseEvent* event)
{
  _mouseState.moving = true;
  _mouseState.prev_pos = _mouseState.curr_pos;
  _mouseState.curr_pos = QVector2D(event->localPos());

  if (_mouseState.r_btn_pressed) {
    float dx = event->localPos().x() - _mouseState.prev_pos.x();
    float dy = event->localPos().y() - _mouseState.prev_pos.y();
    _mouseState.prev_pos = _mouseState.curr_pos;
    _mouseState.curr_pos = QVector2D(event->localPos());

    _camera.yaw   += dx * 0.4f;
    _camera.pitch -= dy * 0.4f;
  }
}

void Viewport::mouseDoubleClickEvent(QMouseEvent* event)
{ }

void Viewport::wheelEvent(QWheelEvent* event)
{
  QPoint numDegrees = event->angleDelta() / 8;

  _camera.pos += (numDegrees.y() >= 0 ? _camera.front : -_camera.front);

  event->accept();
}

void Viewport::keyPressEvent(QKeyEvent* event)
{
  switch (event->key()) {
    case Qt::Key_Alt: {
      _keyState.alt_pressed = true;
      this->cursor().setShape(Qt::SizeHorCursor);
    } break;

    default:
      break;
  };
}

void Viewport::keyReleaseEvent(QKeyEvent* event)
{ }

void Viewport::timerEvent(QTimerEvent* event)
{ }

/** Public functions */
void Viewport::PrintContextInformation()
{
  if (isValid()) {
  QString glType;
  QString glVersion;
  QString glProfile;

  // Get Version Information
  glType = (QOpenGLContext::currentContext()->isOpenGLES()) ? "OpenGL ES" : "OpenGL";
  glVersion = reinterpret_cast<const char*>(_fn->glGetString(GL_VERSION));

  // Get Profile Information
  #define CASE(c) case QSurfaceFormat::c: glProfile = #c; break
    switch (format().profile())
    {
      CASE(NoProfile);
      CASE(CoreProfile);
      CASE(CompatibilityProfile);
    }
  #undef CASE

  std::cout << glType.toStdString() << glVersion.toStdString()
            << "(" << glProfile.toStdString() << ")" << std::endl;
  }
}


/** Priate functions */
void Viewport::init_viewcube_indicator()
{
  // ViewCube data
  //      3----2
  //     /|   /|
  //    0----1 |
  //    | 7__|_6
  //    |/   |/
  //    4----5

  // top
  vertices.insert(vertices.end(), {
    // top
    {{-0.5f,  0.5f,  0.5f}, {0.0000f, 0.0f}, {}}, // 0 - v0
    {{ 0.5f,  0.5f,  0.5f}, {0.3333f, 0.0f}, {}}, // 1 - v1
    {{ 0.5f,  0.5f, -0.5f}, {0.3333f, 0.5f}, {}}, // 2 - v2
    {{-0.5f,  0.5f, -0.5f}, {0.0000f, 0.5f}, {}}, // 3 - v3
    // bot
    {{-0.5f, -0.5f,  0.5f}, {0.6666f, 0.5f}, {}}, // 4 - v4
    {{-0.5f, -0.5f, -0.5f}, {0.6666f, 0.0f}, {}}, // 7 - v5
    {{ 0.5f, -0.5f, -0.5f}, {1.0000f, 0.0f}, {}}, // 6 - v6
    {{ 0.5f, -0.5f,  0.5f}, {1.0000f, 0.5f}, {}},  // 5 - v7
    // front
    {{-0.5f,  0.5f,  0.5f}, {0.3333f, 1.0f}, {}}, // 0 - v8
    {{-0.5f, -0.5f,  0.5f}, {0.3333f, 0.5f}, {}}, // 4 - v9
    {{ 0.5f, -0.5f,  0.5f}, {0.6666f, 0.5f}, {}}, // 5 - v10
    {{ 0.5f,  0.5f,  0.5f}, {0.6666f, 1.0f}, {}}, // 1 - v11
    // back
    {{ 0.5f,  0.5f, -0.5f}, {0.3333f, 0.5f}, {}},  // 2 - v12
    {{ 0.5f, -0.5f, -0.5f}, {0.3333f, 0.0f}, {}},  // 6 - v13
    {{-0.5f, -0.5f, -0.5f}, {0.6666f, 0.0f}, {}},  // 7 - v14
    {{-0.5f,  0.5f, -0.5f}, {0.6666f, 0.5f}, {}},  // 3 - v15
    // left
    {{-0.5f,  0.5f, -0.5f}, {0.0000f, 1.0f}, {}},  // 3 - v16
    {{-0.5f, -0.5f, -0.5f}, {0.0000f, 0.5f}, {}},  // 7 - v17
    {{-0.5f, -0.5f,  0.5f}, {0.3333f, 0.5f}, {}},  // 4 - v18
    {{-0.5f,  0.5f,  0.5f}, {0.3333f, 1.0f}, {}},  // 0 - v19
    // right
    {{ 0.5f,  0.5f,  0.5f}, {0.6666f, 1.0f}, {}},  // 1 - v20
    {{ 0.5f, -0.5f,  0.5f}, {0.6666f, 0.5f}, {}},  // 5 - v21
    {{ 0.5f, -0.5f, -0.5f}, {1.0000f, 0.5f}, {}},  // 6 - v22
    {{ 0.5f,  0.5f, -0.5f}, {1.0000f, 1.0f}, {}}   // 2 - v23
  });

  unsigned int idx[] = { 0,   1,  2,  0,  2,  3,
                         4,   5,  6,  4,  6,  7,
                         8,   9, 10,  8, 10, 11,
                        12,  13, 14, 12, 14, 15,
                        16,  17, 18, 16, 18, 19,
                        20,  21, 22, 20, 22, 23};

  std::copy(std::begin(idx), std::end(idx), std::back_inserter(indices));

  // textures
   _fn->glActiveTexture(GL_TEXTURE0);
   _fn->glBindTexture(GL_TEXTURE_2D, viewcube_texture);
   _fn->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   _fn->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   _fn->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   _fn->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  QImage texImg(":/Assets/eponge.png");
  texImg = texImg.mirrored();
   _fn->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texImg.width(), texImg.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texImg.convertToFormat(QImage::Format_RGBA8888).bits());
   _fn->glGenerateMipmap(GL_TEXTURE_2D);

   _fn->glBindVertexArray(vaos[0]);

   _fn->glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
   _fn->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(gtd::Vertex), &vertices[0], GL_DYNAMIC_DRAW);

   _fn->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebos[0]);
   _fn->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_DYNAMIC_DRAW);

  // position attribute
   _fn->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, gtd::Vertex::stride, nullptr);
   _fn->glEnableVertexAttribArray(0);

  // color attribute
   _fn->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, gtd::Vertex::stride, reinterpret_cast<void*>(gtd::Vertex::colorOffset));
   _fn->glEnableVertexAttribArray(1);

  // texture attribute
   _fn->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, gtd::Vertex::stride, reinterpret_cast<void*>(gtd::Vertex::texCoordOffset));
   _fn->glEnableVertexAttribArray(2);

   _fn->glBindVertexArray(0);
}

void Viewport::init_ground_grid(int n)
{
  size_t vtop = vertices.size();
  size_t itop = indices.size();

  for (int i = 0; i <= n; ++i) {
    for (int j = 0; j <= n; ++j) {
      vertices.push_back({{i * _gridStep - _gridStep * 0.5f * n, 0.0f,
                           j * _gridStep - _gridStep * 0.5f * n}, {}, {0.16f, 0.60f, 0.55f}});
    }
  }

  /*
  for (unsigned i = 0; i < _gridSize; ++i) {
    for (unsigned j = 0; j < _gridSize; ++j) {
        indices.insert(indices.end(), {j + i * (_gridSize + 1), j + 1 + i * (_gridSize + 1),
                                       j + (i + 1) * (_gridSize + 1), j + 1 + (i + 1) * (_gridSize + 1)});
    }
  }
  */
  for (unsigned j = 0; j < _gridSize + 1; ++j) {
      indices.insert(indices.end(), {j, j + _gridSize * (_gridSize + 1)});
  }

  for (unsigned i = 0; i < _gridSize + 1; ++i) {
      indices.insert(indices.end(), {i * (_gridSize + 1), (i + 1) * (_gridSize + 1) - 1});
  }

   _fn->glBindVertexArray(vaos[1]);
   _fn->glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
   _fn->glBufferData(GL_ARRAY_BUFFER, (vertices.size() - vtop) * sizeof(gtd::Vertex), &vertices[vtop], GL_DYNAMIC_DRAW);

   _fn->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebos[1]);
   _fn->glBufferData(GL_ELEMENT_ARRAY_BUFFER, (indices.size() - itop) * sizeof(unsigned int), &indices[itop], GL_DYNAMIC_DRAW);

   _fn->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, gtd::Vertex::stride, nullptr);
   _fn->glEnableVertexAttribArray(0);

   _fn->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, gtd::Vertex::stride, reinterpret_cast<void*>(gtd::Vertex::colorOffset));
   _fn->glEnableVertexAttribArray(1);

   _fn->glBindVertexArray(0);
}

void Viewport::render_viewcube()
{
  // TODO: plane drawing
  model_mat.setToIdentity();
  view_mat = _camera.ViewMatrix();
  program.bind();
  program.setUniformValue("MVP", proj_mat * view_mat * model_mat);
  _fn->glBindTexture(GL_TEXTURE_2D, viewcube_texture);
  _fn->glBindVertexArray(vaos[0]);
  _fn->glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
  _fn->glBindTexture(GL_TEXTURE_2D, 0);
  _fn->glBindVertexArray(0);
  program.release();
}

void Viewport::render_grid()
{
  _gridProgram.bind();
  _gridProgram.setUniformValue("MVP", proj_mat * _camera.ViewMatrix() * model_mat);
  _gridProgram.setUniformValue("alpha", 0.8f);
   _fn->glBindVertexArray(vaos[1]);
   _fn->glDrawElements(GL_LINES, (_gridSize + 1) * 4, GL_UNSIGNED_INT, nullptr);
   _fn->glBindVertexArray(0);
  _gridProgram.release();
}

void Viewport::draw_selection_area(const QVector2D& p0, const QVector2D& p1)
{
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  QPainterPath path;
  QRect rect{static_cast<int>(p0.x()), static_cast<int>(p0.y()),
        static_cast<int>(p1.x() - p0.x()), static_cast<int>(p1.y() - p0.y())};
  path.addRoundedRect(rect, 10, 10);
  painter.fillPath(path, QColor(110, 198, 255, 100));
  QPen pen(QColor(0, 122, 193), 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
  painter.setPen(pen);
  painter.drawPath(path);
  painter.end();
}

void Viewport::SelectCheck(bool checked_p)
{
  std::cout << "selection checked? " << checked_p << std::endl;
}

void Viewport::MoveCheck(bool checked_p)
{

}

void Viewport::RotateCheck(bool checked_p)
{

}

void Viewport::ScaleCheck(bool checked_p)
{

}

void Viewport::PlaceCheck(bool checked_p)
{

}
