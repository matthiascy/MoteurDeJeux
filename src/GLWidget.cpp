#include "GLWidget.hpp"
#include "MeshLoader.hpp"
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>
#include <QTimer>
#include <QLabel>

GLWidget::GLWidget(QWidget *parent)
  : QOpenGLWidget(parent), m_program(nullptr),
    m_projMatrixLoc{0}, m_mvMatrixLoc{0}, m_normalMatrixLoc{0}, m_lightPosLoc{0}, m_last_time{QTime::currentTime()},
    m_fps{0}, m_fps_label{nullptr}
{
  m_scene = memory::mkUnique<Scene>();
  m_physics_system = memory::mkUnique<PhysicsSystem>();
  m_timer = new QTimer;
  m_fps_label = new QLabel(this);
  m_fps_label->setFrameStyle(QFrame::Panel | QFrame::Sunken);
  m_fps_label->setAlignment(Qt::AlignBottom | Qt::AlignRight);
  m_fps_label->setFixedWidth(60);
  m_fps_label->move(25, 25);
  m_fps_label->setStyleSheet("QLabel { background-color : white; color : black; }");

  auto& terrain = m_scene->createTerrain(1023, 1023, Image(":/Textures/heightmap"), false, 100.0f);
  terrain->setParent(m_scene->root())->setVisible(true);
  terrain->transform().scale(0.5).translate(Vec3{0, -1000, 0});
  
  /*
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
    Real u = qAtan2(n.x(), n.z()) / (2 * M_PI) + 0.5;
    Real v = n.y() * 0.5 + 0.5;
    data[i*8+6] = u;
    data[i*8+7] = v;
    if (umax < u) umax = u;
    if (umin > u) umin = u;
    if (vmax < v) vmax = v;
    if (vmin > v) vmin = v;
  }

  qDebug() << "u max:" << umax << "u min:" << umin;
  qDebug() << "v max:" << vmax << "v min:" << vmin;

  for (UInt32 i = 0; i < sphere->vertexCount(); ++i) {
    data[i*8+6] = (data[i*8+6] - umin) / (umax - umin);
    data[i*8+7] = (data[i*8+7] - vmin) / (vmax - vmin);
  }

  {
    auto* milky = new GameObject("Milky", "Habibi");
    milky->attachMesh(sphere);
    m_scene->addGameObject(milky);
    auto* milkyNode = m_scene->createSceneNode("MilkyNode");
    milkyNode->setParent(m_scene->rootSceneNode());
    milkyNode->attachObject(milky);
    milkyNode->setVisible(true);
    milkyNode->setPosition(Vec3{0.0, 500.0, 0.0});
    milkyNode->scale(vec3::mkVec3(40000), ETransformSpace::Local);
    milkyNode->update();

    auto* sun = new GameObject("Sun", "Habibi");
    sun->attachMesh(sphere);
    m_scene->addGameObject(sun);
    auto* sunNode = m_scene->createSceneNode("SunNode");
    sunNode->setParent(m_scene->rootSceneNode());
    sunNode->attachObject(sun);
    sunNode->setVisible(true);
    //sunNode->setPosition(Vec3{0.0, 1000.0, 0.0});
    sunNode->setPosition(Vec3{0.0, 500.0, 0.0});
    sunNode->scale(vec3::mkVec3(1000), ETransformSpace::Local);

    auto* mercury = new GameObject("Mercury", "Habibi");
    mercury->attachMesh(sphere);
    m_scene->addGameObject(mercury);
    auto* mercuryNode = m_scene->createSceneNode("MercuryNode");
    mercuryNode->setInheritedTransformation(false, true, false);
    mercuryNode->setParent(sunNode);
    mercuryNode->attachObject(mercury);
    mercuryNode->setVisible(true);
    mercuryNode->setPosition(Vec3{1.5, 0.0, 0.0});
    mercuryNode->scale(vec3::mkVec3(0.2), ETransformSpace::Local);

    auto* venus = new GameObject("Venus", "Habibi");
    venus->attachMesh(sphere);
    m_scene->addGameObject(venus);
    auto* venusNode = m_scene->createSceneNode("VenusNode");
    venusNode->setParent(sunNode);
    venusNode->attachObject(venus);
    venusNode->setVisible(true);
    venusNode->setPosition(Vec3{2.2, 0, 0.0});
    venusNode->scale(vec3::mkVec3(0.3), ETransformSpace::Local);

    auto* earth = new GameObject("Earth", "Habibi");
    earth->attachMesh(sphere);
    m_scene->addGameObject(earth);
    auto* earthNode = m_scene->createSceneNode("EarthNode");
    earthNode->setInheritedTransformation(false, true, false);
    //earthNode->setParent(earthOrbitNode);
    earthNode->setParent(sunNode);
    earthNode->attachObject(earth);
    earthNode->setVisible(true);
    earthNode->setPosition(Vec3{3.4, 0.0, 0.0});
    earthNode->scale(vec3::mkVec3(0.35), ETransformSpace::Local);

    auto* mars = new GameObject("Mars", "Habibi");
    mars->attachMesh(sphere);
    m_scene->addGameObject(mars);
    auto* marsNode = m_scene->createSceneNode("MarsNode");
    marsNode->setInheritedTransformation(false, true, false);
    marsNode->setParent(sunNode);
    marsNode->attachObject(mars);
    marsNode->setVisible(true);
    marsNode->setPosition(Vec3{5, 0.0, 0.0});
    marsNode->scale(vec3::mkVec3(0.15), ETransformSpace::Local);

    auto* jupiter = new GameObject("Jupiter", "Habibi");
    jupiter->attachMesh(sphere);
    m_scene->addGameObject(jupiter);
    auto* jupiterNode = m_scene->createSceneNode("JupiterNode");
    jupiterNode->setParent(sunNode);
    jupiterNode->attachObject(jupiter);
    jupiterNode->setVisible(true);
    jupiterNode->setPosition(Vec3{7, 0.0, 0.0});
    jupiterNode->scale(vec3::mkVec3(0.7), ETransformSpace::Local);

    auto* saturn = new GameObject("Saturn", "Habibi");
    saturn->attachMesh(sphere);
    m_scene->addGameObject(saturn);
    auto* saturnNode = m_scene->createSceneNode("SaturnNode");
    saturnNode->setParent(sunNode);
    saturnNode->attachObject(saturn);
    saturnNode->setVisible(true);
    saturnNode->setPosition(Vec3{9.6, 0.0, 0.0});
    saturnNode->scale(vec3::mkVec3(0.6), ETransformSpace::Local);

    auto* uranus = new GameObject("Uranus", "Habibi");
    uranus->attachMesh(sphere);
    m_scene->addGameObject(uranus);
    auto* uranusNode = m_scene->createSceneNode("UranusNode");
    uranusNode->setParent(sunNode);
    uranusNode->attachObject(uranus);
    uranusNode->setVisible(true);
    uranusNode->setPosition(Vec3{11.2, 0.0, 0.0});
    uranusNode->scale(vec3::mkVec3(0.5), ETransformSpace::Local);

    auto* neptune = new GameObject("Neptune", "Habibi");
    neptune->attachMesh(sphere);
    m_scene->addGameObject(neptune);
    auto* neptuneNode = m_scene->createSceneNode("NeptuneNode");
    neptuneNode->setParent(sunNode);
    neptuneNode->attachObject(neptune);
    neptuneNode->setVisible(true);
    neptuneNode->setPosition(Vec3{13.7, 0.0, 0.0});
    neptuneNode->scale(vec3::mkVec3(0.45), ETransformSpace::Local);

    auto* moon = new GameObject("Moon", "Habibi");
    moon->attachMesh(sphere);
    m_scene->addGameObject(moon);
    auto* moonNode = m_scene->createSceneNode("MoonNode");
    moonNode->setInheritedTransformation(false, true, false);
    moonNode->setParent(earthNode);
    moonNode->attachObject(moon);
    moonNode->setVisible(true);
    moonNode->setPosition(Vec3{1.5, 0.0, 0.0});
    moonNode->scale(vec3::mkVec3(0.15), ETransformSpace::Local);

    m_solar_system.resize(20);
    m_solar_system[0] = sunNode;
    m_solar_system[1] = mercuryNode;
    m_solar_system[2] = venusNode;
    m_solar_system[3] = earthNode;
    m_solar_system[4] = marsNode;
    m_solar_system[5] = jupiterNode;
    m_solar_system[6] = saturnNode;
    m_solar_system[7] = uranusNode;
    m_solar_system[8] = neptuneNode;
    m_solar_system[9] = moonNode;
  }
       */

  // Camera
  m_camera = m_scene->createCamera("MainCamera");
  m_camera->pos = Vec3(0.0f, 6000.0f, 6000.0f);

  m_vbos.resize(255);
  m_ebos.resize(255);

  m_is_ctrl_pressed = false;

  auto* fpsTimer = new QTimer();
  connect(fpsTimer, SIGNAL(timeout()), this, SLOT(update()));
  fpsTimer->start(16);

  connect(m_timer, &QTimer::timeout, this, [this](){
    solar_system_update(m_timer->interval());
  });
  m_timer->start(16);
}

GLWidget::~GLWidget()
{
    cleanup();
}

QSize GLWidget::minimumSizeHint() const
{
    return {600, 480};
}

QSize GLWidget::sizeHint() const
{
    return {600, 480};
}

void GLWidget::cleanup()
{
    if (m_program == nullptr) return;
    makeCurrent();
  for (auto& texture : m_textures) {
    texture->destroy();
    delete texture;
  }
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

    for (const auto& obj : m_scene->gameObjects()) {
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
  glDepthFunc(GL_LESS);
}

void GLWidget::setupTextures()
{
  m_textures.resize(15);
  m_textures[0] = new OpenGLTexture(Image(":/Textures/sun").mirrored());
  m_textures[1] = new OpenGLTexture(Image(":/Textures/mercury").mirrored());
  m_textures[2] = new OpenGLTexture(Image(":/Textures/venus").mirrored());
  m_textures[3] = new OpenGLTexture(Image(":/Textures/earth").mirrored());
  m_textures[4] = new OpenGLTexture(Image(":/Textures/mars").mirrored());
  m_textures[5] = new OpenGLTexture(Image(":/Textures/jupiter").mirrored());
  m_textures[6] = new OpenGLTexture(Image(":/Textures/saturn").mirrored());
  m_textures[7] = new OpenGLTexture(Image(":/Textures/uranus").mirrored());
  m_textures[8] = new OpenGLTexture(Image(":/Textures/neptune").mirrored());
  m_textures[9] = new OpenGLTexture(Image(":/Textures/moon").mirrored());
  m_textures[10] = new OpenGLTexture(Image(":/Textures/milky").mirrored());
  m_textures[11] = new OpenGLTexture(Image(":/Textures/grass").mirrored());
  m_textures[12] = new OpenGLTexture(Image(":/Textures/rock").mirrored());
  m_textures[13] = new OpenGLTexture(Image(":/Textures/snowrock").mirrored());
  m_textures[14] = new OpenGLTexture(Image(":/Textures/ocean").mirrored());
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
    m_fps_label->setText("FPS : " + String::number(m_fps));
  }

  auto objs = m_scene->toBeRenderedGameObjects();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  m_physics_system->update(1.0/60.0);

  auto collisionObjects = m_physics_system->collisionObjects();

  m_vao.bind();

  static float a = 0;
  for (auto& obj : objs) {
    int idx = obj->vboIndex();
    m_vbos[idx].bind();
    glEnableVertexAttribArray(0); glEnableVertexAttribArray(1); glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3*sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6*sizeof(float)));
    m_ebos[idx]->bind();

    qDebug() << "render" << obj->name();
    m_model.setToIdentity();
    m_model = node->worldMatrix();
    btTransform state;
    btRigidBody::upcast(collisionObjects[0])->getMotionState()->getWorldTransform(state);
    m_model.translate(state.getOrigin().x(), state.getOrigin().y(), state.getOrigin().z());
    m_model.rotate(Quat(state.getRotation().w(), state.getRotation().x(), state.getRotation().y(), state.getRotation().z()));
    //m_model.rotate(a / 2, vec3::AxisY);
    QMatrix3x3 normalMatrix = m_model.normalMatrix();

    m_program->bind();
    if (obj->name() == "Terrain") {
      m_textures[11]->bind(0);
      m_textures[12]->bind(1);
      m_textures[13]->bind(2);
      m_textures[14]->bind(3);
      m_program->setUniformValue("isTerrain", 1);
    } else {
      m_program->setUniformValue("isTerrain", 0);
      glEnable(GL_CULL_FACE);
      glCullFace(GL_BACK);
      glFrontFace(GL_CCW);
      if (obj->name() == "Sun") {
        m_textures[0]->bind(0);
      } else if (obj->name() == "Mercury") {
        m_textures[1]->bind(0);
      } else if (obj->name() == "Venus") {
        m_textures[2]->bind(0);
      } else if (obj->name() == "Earth") {
        m_textures[3]->bind(0);
      } else if (obj->name() == "Mars") {
        m_textures[4]->bind(0);
      } else if (obj->name() == "Jupiter") {
        m_textures[5]->bind(0);
      } else if (obj->name() == "Saturn") {
        m_textures[6]->bind(0);
      } else if (obj->name() == "Uranus") {
        m_textures[7]->bind(0);
      } else if (obj->name() == "Neptune") {
        m_textures[8]->bind(0);
      } else if (obj->name() == "Moon") {
        m_textures[9]->bind(0);
      } else if (obj->name() == "Milky") {
        m_textures[10]->bind(0);
        glDisable(GL_CULL_FACE);
      }
    }

    m_program->setUniformValue(m_projMatrixLoc, m_proj);
    m_program->setUniformValue(m_mvMatrixLoc,m_camera->ViewMatrix() * m_model);
    m_program->setUniformValue(m_normalMatrixLoc, normalMatrix);

    glDrawElements(GL_TRIANGLES, obj->mesh()->indices().size(), GL_UNSIGNED_INT, (void*)nullptr);
  }
  a += 1.5;
  m_program->release();
}

void GLWidget::resizeGL(int w, int h)
{
    m_proj.setToIdentity();
    m_proj.perspective(45.0f, GLfloat(w) / h, 0.01f, 500000.0f);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
  //m_lastPos = event->pos();
}

void GLWidget::mouseReleaseEvent(QMouseEvent* event)
{
  // do nothing
  event->accept();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
  /*
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
    */
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

  event->accept();
  repaint();
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
  switch (event->key()) {
    case Qt::Key_Z: {
      m_camera->pos += 20 * m_camera->front;
    } break;

    case Qt::Key_S: {
      m_camera->pos += -20 * m_camera->front;
    } break;

    case Qt::Key_Q: {
      m_camera->pos += -20 * m_camera->right;
    } break;

    case Qt::Key_D: {
      m_camera->pos += 20 * m_camera->right;
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

void GLWidget::solar_system_update(Real dt)
{
  /*
  // mercury
  m_solar_system[1]->rotate(Quat::fromAxisAndAngle(vec3::AxisY, dt / 1000 * 5), ETransformSpace::Local);
  // venus
  m_solar_system[2]->rotate(Quat::fromAxisAndAngle(vec3::AxisY, dt / 1000 * 15), ETransformSpace::Local);
  // earth
  m_solar_system[3]->rotate(Quat::fromAxisAndAngle(vec3::AxisY, dt / 1000 * 25), ETransformSpace::Local);
  // mars
  m_solar_system[4]->rotate(Quat::fromAxisAndAngle(vec3::AxisY, dt / 1000 * 15), ETransformSpace::Local);
  // jupiter
  m_solar_system[5]->rotate(Quat::fromAxisAndAngle(vec3::AxisY, dt / 1000 * 35), ETransformSpace::Local);
  // saturn
  m_solar_system[6]->rotate(Quat::fromAxisAndAngle(vec3::AxisY, dt / 1000 * 55), ETransformSpace::Local);
  // uranus
  m_solar_system[7]->rotate(Quat::fromAxisAndAngle(vec3::AxisY, dt / 1000 * 25), ETransformSpace::Local);
  // neptune
  m_solar_system[8]->rotate(Quat::fromAxisAndAngle(vec3::AxisY, dt / 1000 * 55), ETransformSpace::Local);
  // moon
  m_solar_system[9]->rotate(Quat::fromAxisAndAngle(vec3::AxisY, dt / 1000 * 20), ETransformSpace::Local);
   */
}

//void GLWidget::update()
//{
  // process input
  // update
  // Schedule a redraw
//  QOpenGLWidget::update();
//}
