#include "GLWidget.hpp"
#include "MeshLoader.hpp"
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>
#include <QTimer>

GLWidget::GLWidget(QWidget *parent)
  : QOpenGLWidget(parent), m_scn_mgr{}, m_xRot(0), m_yRot(0), m_zRot(0), m_program(nullptr),
    m_projMatrixLoc{0}, m_mvMatrixLoc{0}, m_normalMatrixLoc{0}, m_lightPosLoc{0}, m_last_time{QTime::currentTime()},
    m_fps{0}, m_fps_label{nullptr}
{
  m_scn_mgr = memory::mkUnique<SceneManager>();
  m_timer = memory::mkUnique<QTimer>();
  //m_fps_label = new QLabel(this);
  //m_fps_label->setFrameStyle(QFrame::Panel | QFrame::Sunken);
  //m_fps_label->setAlignment(Qt::AlignBottom | Qt::AlignRight);
  //m_fps_label->setFixedWidth(60);
  //m_fps_label->move(25, 25);
//Here is how to change position:
  //m_fps_label->setGeometry(QRect(10,10,30,80));
  // Terrain
  //{
//    auto* terrain = new Terrain();
//    terrain->create(1023, 1023, Image(":/Textures/heightmap"), false, 100.0f, 100.0f);
//    m_scn_mgr->addGameObject(terrain);
//    SceneNode* terrainNode = m_scn_mgr->createSceneNode("TerrainNode");
//    terrainNode->setParent(m_scn_mgr->rootSceneNode());
//    terrainNode->attachObject(terrain);
//    terrainNode->setVisible(true);
//    terrainNode->scale(Vec3{0.5f, 0.5f, 0.5f}, ETransformSpace::Local);
//  }

  // Soleil
  Mesh* sphere = MeshLoader::loadMesh("./sphere.obj");
  qDebug() << sphere->count();
  Array<float>& data = sphere->dataArray();
  Real umax = -10.0;
  Real umin = 20.0;
  Real vmax = -10.0;
  Real vmin = 20.0;
  for (UInt32 i = 0; i < sphere->vertexCount(); ++i) {
    Vec3 n = Vec3(data[i*8], data[i*8+1], data[i*8+2]).normalized();
    data[i*8+6] = qAtan2(n.x(), n.z()) / (2 * M_PI) + 0.5;
    if (umax < data[i*8+6]) umax = data[i*8+6];
    if (umin > data[i*8+6]) umin = data[i*8+6];
    data[i*8+7] = n.y() * 0.5 + 0.5;
    if (vmax < data[i*8+7]) vmax = data[i*8+7];
    if (vmin > data[i*8+7]) vmin = data[i*8+7];
  }

  for (UInt32 i = 0; i < sphere->vertexCount(); ++i) {
    data[i*8+6] /= (umax - umin);
    data[i*8+7] /= (vmax - vmin);
  }

  {
    auto* milky = new GameObject("Milky", "Habibi");
    milky->attachMesh(sphere);
    m_scn_mgr->addGameObject(milky);
    auto* milkyNode = m_scn_mgr->createSceneNode("MilkyNode");
    milkyNode->setParent(m_scn_mgr->rootSceneNode());
    milkyNode->attachObject(milky);
    milkyNode->setVisible(true);
    milkyNode->setPosition(Vec3{0.0, 0.0, 0.0});
    milkyNode->scale(Vec3(5000.0, 5000.0, 5000.0), ETransformSpace::Local);

    auto* sun = new GameObject("Sun", "Habibi");
    sun->attachMesh(sphere);
    m_scn_mgr->addGameObject(sun);
    auto* sunNode = m_scn_mgr->createSceneNode("SunNode");
    sunNode->setParent(m_scn_mgr->rootSceneNode());
    sunNode->attachObject(sun);
    sunNode->setVisible(true);
    sunNode->setPosition(Vec3{0.0, 1000.0, 0.0});
    sunNode->scale(Vec3(500.0, 500.0, 500.0), ETransformSpace::Local);

    auto* earth = new GameObject("Earth", "Habibi");
    earth->attachMesh(sphere);
    m_scn_mgr->addGameObject(earth);
    auto* earthNode = m_scn_mgr->createSceneNode("EarthNode");
    earthNode->setParent(sunNode);
    earthNode->attachObject(earth);
    earthNode->setVisible(true);
    earthNode->setPosition(Vec3{1000, 1000.0, 0.0});
    earthNode->scale(Vec3(50, 50, 50), ETransformSpace::Local);

    auto* jupiter = new GameObject("Jupiter", "Habibi");
    jupiter->attachMesh(sphere);
    m_scn_mgr->addGameObject(jupiter);
    auto* jupiterNode = m_scn_mgr->createSceneNode("JupiterNode");
    jupiterNode->setParent(sunNode);
    jupiterNode->attachObject(jupiter);
    jupiterNode->setVisible(true);
    jupiterNode->setPosition(Vec3{1500, 1000.0, 0.0});
    jupiterNode->scale(Vec3(100, 100, 100), ETransformSpace::Local);

    auto* moon = new GameObject("Moon", "Habibi");
    moon->attachMesh(sphere);
    m_scn_mgr->addGameObject(moon);
    auto* moonNode = m_scn_mgr->createSceneNode("MoonNode");
    moonNode->setParent(earthNode);
    moonNode->attachObject(moon);
    moonNode->setVisible(true);
    moonNode->setPosition(Vec3{900, 1000.0, 0.0});
    moonNode->scale(Vec3(10, 10, 10), ETransformSpace::Local);
  }

  // Camera
  m_camera = m_scn_mgr->createCamera("MainCamera");
  m_camera->pos = Vec3(0.0f, 3000.0f, 2000.0f);

  m_vbos.resize(255);
  m_ebos.resize(255);

  m_timer->setInterval(100);
  connect(&*m_timer, &QTimer::timeout, this, &GLWidget::rotateView);
  m_timer->start();
  m_is_ctrl_pressed = false;

  auto* fpsTimer = new QTimer();
  connect(fpsTimer, &QTimer::timeout, this, &GLWidget::rotateView);
  fpsTimer->start(16);
}

GLWidget::~GLWidget()
{
    cleanup();
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(400, 400);
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

void GLWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_xRot) {
        m_xRot = angle;
        emit xRotationChanged(angle);
        update();
    }
}

void GLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_yRot) {
        m_yRot = angle;
        emit yRotationChanged(angle);
        update();
    }
}

void GLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_zRot) {
        m_zRot = angle;
        emit zRotationChanged(angle);
        update();
    }
}

void GLWidget::rotateView()
{
  m_zRot += 2.0f;
  emit yRotationChanged(m_zRot);
  update();
}

void GLWidget::cleanup()
{
    if (m_program == nullptr) return;
    makeCurrent();
    m_grass_texture->destroy();
    m_grass_texture.reset();
    m_rock_texture->destroy();
  m_rock_texture.reset();
    m_snow_rock_texture->destroy();
  m_snow_rock_texture.reset();
    m_ocean_texture->destroy();
  m_ocean_texture.reset();
    m_earth_texture->destroy();
  m_earth_texture.reset();
    m_sun_texture->destroy();
  m_sun_texture.reset();
    m_jupiter_texture->destroy();
  m_jupiter_texture.reset();
    m_moon_texture->destroy();
  m_moon_texture.reset();
  m_milky_texture->destroy();
  m_milky_texture.reset();
    for (auto& vbo : m_vbos) vbo.destroy();
    for (auto& ebo : m_ebos) ebo->destroy();
    delete m_program;
    m_program = nullptr;
    doneCurrent();
}

void GLWidget::initializeGL()
{
  initializeOpenGLFunctions();
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &GLWidget::cleanup);
    glClearColor(0.2f, 0.3f, 0.4f, 1.0f);

    m_program = new QOpenGLShaderProgram;
    // fonction qui contient glCreateShader, glShaderSource, glCompileShader, glCreateProgram, glAttachShader
    m_program->addShaderFromSourceFile(OpenGLShader::Vertex, ":/Shaders/BasicVert");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Shaders/BasicFrag");
    m_program->bindAttributeLocation("inPosition", 0);
    m_program->bindAttributeLocation("inNormal", 1);
    m_program->bindAttributeLocation("inTexCoord", 2);
    m_program->link();
    m_program->bind();
    m_program->setUniformValue("texture0", 0);
    m_program->setUniformValue("texture1", 1);
    m_program->setUniformValue("texture2", 2);
    m_program->setUniformValue("texture3", 3);

    m_projMatrixLoc = m_program->uniformLocation("projMatrix");  // egale a glGetUniformLocation()
    m_mvMatrixLoc = m_program->uniformLocation("modelViewMatrix");
    m_normalMatrixLoc = m_program->uniformLocation("normalMatrix");
    m_lightPosLoc = m_program->uniformLocation("lightPos");

    m_vao.create();
    m_vao.bind();

    for (const auto& obj : m_scn_mgr->gameObjects()) {
      int idx = obj->vboIndex();
      m_vbos[idx].create();
      m_vbos[idx].bind();
      m_vbos[idx].setUsagePattern(OpenGLBuffer::StaticDraw);
      m_vbos[idx].allocate(obj->mesh()->constData(), obj->mesh()->count() * sizeof(float));
      m_ebos[idx] = new OpenGLBuffer(OpenGLBuffer::IndexBuffer);
      m_ebos[idx]->create();
      m_ebos[idx]->bind();
      m_ebos[idx]->setUsagePattern(OpenGLBuffer::StaticDraw);
      m_ebos[idx]->allocate(obj->mesh()->indices().data(), obj->mesh()->vertexCount() * sizeof(UInt32));
    }

    // Light position is fixed.
    m_program->setUniformValue(m_lightPosLoc, QVector3D(0, 0, 70));
    m_program->release();
    setupTextures();
  glEnable(GL_DEPTH_TEST);
  //glEnable(GL_CULL_FACE);
  //glCullFace(GL_BACK);
  glFrontFace(GL_CW);
}

void GLWidget::setupTextures()
{
  m_grass_texture = memory::mkUnique<QOpenGLTexture>(QImage(":/Textures/grass").mirrored());
  m_rock_texture = memory::mkUnique<QOpenGLTexture>(QImage(":/Textures/rock").mirrored());
  m_snow_rock_texture = memory::mkUnique<QOpenGLTexture>(QImage(":/Textures/snowrock").mirrored());
  m_ocean_texture = memory::mkUnique<QOpenGLTexture>(QImage(":/Textures/ocean").mirrored());
  m_earth_texture = memory::mkUnique<QOpenGLTexture>(QImage(":/Textures/earth").mirrored());
  m_sun_texture = memory::mkUnique<OpenGLTexture>(Image(":/Textures/sun").mirrored());
  m_moon_texture = memory::mkUnique<OpenGLTexture>(Image(":/Textures/moon").mirrored());
  m_jupiter_texture = memory::mkUnique<OpenGLTexture>(Image(":/Textures/jupiter").mirrored());
  m_milky_texture = memory::mkUnique<OpenGLTexture>(Image(":/Textures/milky").mirrored());
}

void GLWidget::paintGL()
{
  static int frames = 0;
  frames++;
  if (m_last_time.msecsTo(QTime::currentTime()) > 1000) {
    m_fps = frames;
    emit fpsChanged(m_fps);
    m_last_time = QTime::currentTime();
    frames = 0;
    //m_fps_label->setText("FPS : " + String::number(m_fps));
  }

  m_scn_mgr->rootSceneNode()->update(true);
  //auto objs = m_scn_mgr->toBeRenderedGameObjects();
  auto nodes = m_scn_mgr->toBeRenderedSceneNodes();
  //qDebug() << objs.size() << "objects will be rendered...";
  //qDebug() << nodes.size() << "nodes will be rendered...";

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  m_vao.bind();

  for (auto i = 0; i < nodes.size(); ++i) {
    const auto& obj = nodes[i]->attachedObject(0);
    int idx = obj->vboIndex();
    //qDebug() << "i:" << i;
    m_vbos[idx].bind();
    glEnableVertexAttribArray(0); glEnableVertexAttribArray(1); glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3*sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6*sizeof(float)));
    m_ebos[idx]->bind();

    m_model.setToIdentity();
    qDebug() << nodes[i]->scale();
    m_model.translate(nodes[i]->position());
    m_model.scale(nodes[i]->scale());
    m_model.rotate(90.0f, QVector3D(1.0f, 0.0f, 0.0f));
    m_model.rotate(180.0f - (m_xRot / 16.0f), 1, 0, 0);
    m_model.rotate(m_yRot / 16.0f, 0, 1, 0);
    m_model.rotate(m_zRot / 16.0f, 0, 0, 1);
    QMatrix3x3 normalMatrix = m_model.normalMatrix();

    m_program->bind();
    if (obj->name() == "Terrain") {
      m_grass_texture->bind(0);
      m_rock_texture->bind(1);
      m_snow_rock_texture->bind(2);
      m_ocean_texture->bind(3);
      m_program->setUniformValue("isTerrain", 1);
    } else {
      m_program->setUniformValue("isTerrain", 0);
      if (obj->name() == "Earth") {
        qDebug() << "Render Earth";
        m_earth_texture->bind(0);
      } else if (obj->name() == "Sun") {
        m_sun_texture->bind(0);
      } else if (obj->name() == "Jupiter") {
        m_jupiter_texture->bind(0);
      } else if (obj->name() == "Moon") {
        m_moon_texture->bind(0);
      } else if (obj->name() == "Milky") {
        m_milky_texture->bind(0);
      }
    }

    m_program->setUniformValue(m_projMatrixLoc, m_proj);
    m_program->setUniformValue(m_mvMatrixLoc,
                               m_camera->ViewMatrix() * m_model);
    m_program->setUniformValue(m_normalMatrixLoc, normalMatrix);

    glDrawElements(GL_TRIANGLES, obj->mesh()->indices().size(), GL_UNSIGNED_INT, (void*)nullptr);
  }

  m_program->release();
}

void GLWidget::resizeGL(int w, int h)
{
    m_proj.setToIdentity();
    m_proj.perspective(45.0f, GLfloat(w) / h, 0.01f, 10000.0f);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
  m_lastPos = event->pos();
}

void GLWidget::mouseReleaseEvent(QMouseEvent* event)
{
  // do nothing
  event->accept();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - m_lastPos.x();
    int dy = event->y() - m_lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(m_xRot + 8 * dy);
        setYRotation(m_yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(m_xRot + 8 * dy);
        setZRotation(m_zRot + 8 * dx);
    } else if (event->buttons() & Qt::MiddleButton) {
        m_camera->yaw += 8 * dx / 10.0f;
        m_camera->pitch -= 8 * dy / 10.0f;
    }

    m_lastPos = event->pos();
    //qDebug() << m_camera->pos.x() << " " << m_camera->pos.y() << " " << m_camera->pos.z();
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
  QPoint num_degrees = event->angleDelta() / 8;

  if (!m_is_ctrl_pressed) {

    m_camera->pitch += num_degrees.y() >= 0 ? 2.0f : -2.0f;

  } else {

    QVector3D dir = QVector3D::crossProduct(m_camera->up, m_camera->right);
    m_camera->pos += num_degrees.y() >= 0 ? 10.0f * dir.normalized() : -10.0f * dir.normalized();
  }

  //qDebug() << m_camera->pos.x() << " " << m_camera->pos.y() << " " << m_camera->pos.z();

  event->accept();
  repaint();
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
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

    case Qt::Key_Control: {
      m_is_ctrl_pressed = !m_is_ctrl_pressed;
    } break;
  }

  qDebug() << "KeyPress";
  //qDebug() << m_camera->pos.x() << " " << m_camera->pos.y() << " " << m_camera->pos.z();

  event->accept();
  repaint();
}

void GLWidget::keyReleaseEvent(QKeyEvent* event)
{
  switch (event->key()) {
    case Qt::Key_Control: {
      m_is_ctrl_pressed = false;
    } break;

    default:
      break;
  }

  qDebug() << "KeyRelease";

  event->accept();
  repaint();
}
