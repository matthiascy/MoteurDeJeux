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
  m_timer = new QTimer;
  m_fps_label = new QLabel(this);
  m_fps_label->setFrameStyle(QFrame::Panel | QFrame::Sunken);
  m_fps_label->setAlignment(Qt::AlignBottom | Qt::AlignRight);
  m_fps_label->setFixedWidth(60);
  m_fps_label->move(25, 25);
  m_fps_label->setStyleSheet("QLabel { background-color : white; color : black; }");
  // Terrain
  /*
  {
    auto* terrain = new Terrain();
    terrain->create(1023, 1023, Image(":/Textures/heightmap"), false, 100.0f, 100.0f);
    m_scn_mgr->addGameObject(terrain);
    SceneNode* terrainNode = m_scn_mgr->createSceneNode("TerrainNode");
    terrainNode->setParent(m_scn_mgr->rootSceneNode());
    //terrainNode->setOrientation(Quat::fromAxisAndAngle(vec3::AxisX, 90));
    terrainNode->attachObject(terrain);
    terrainNode->setVisible(true);
    terrainNode->scale(Vec3{0.5f, 0.5f, 0.5f}, ETransformSpace::Local);
  }*/
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
    milkyNode->setPosition(vec3::Zero);
    milkyNode->scale(vec3::mkVec3(20000), ETransformSpace::Local);
    milkyNode->update();

    auto* sun = new GameObject("Sun", "Habibi");
    sun->attachMesh(sphere);
    m_scn_mgr->addGameObject(sun);
    auto* sunNode = m_scn_mgr->createSceneNode("SunNode");
    sunNode->setParent(m_scn_mgr->rootSceneNode());
    sunNode->attachObject(sun);
    sunNode->setVisible(true);
    //sunNode->setPosition(Vec3{0.0, 1000.0, 0.0});
    sunNode->setPosition(Vec3{0.0, 0.0, 0.0});
    sunNode->scale(vec3::mkVec3(1000), ETransformSpace::Local);

    auto* mercuryOrbitNode = m_scn_mgr->createSceneNode("MercuryOrbitNode");
    mercuryOrbitNode->setParent(m_scn_mgr->rootSceneNode());
    mercuryOrbitNode->scale(sunNode->scale(), ETransformSpace::Local);

    auto* mercury = new GameObject("Mercury", "Habibi");
    mercury->attachMesh(sphere);
    m_scn_mgr->addGameObject(mercury);
    auto* mercuryNode = m_scn_mgr->createSceneNode("MercuryNode");
    mercuryNode->setInheritedTransformation(false, true, false);
    mercuryNode->setParent(mercuryOrbitNode);
    mercuryNode->attachObject(mercury);
    mercuryNode->setVisible(true);
    mercuryNode->setPosition(Vec3{1.5, 0.0, 0.0});
    mercuryNode->scale(vec3::mkVec3(0.2), ETransformSpace::Local);

    auto* venusOrbitNode = m_scn_mgr->createSceneNode("VenusOrbitNode");
    venusOrbitNode->setParent(m_scn_mgr->rootSceneNode());
    venusOrbitNode->scale(sunNode->scale(), ETransformSpace::Local);

    auto* venus = new GameObject("Venus", "Habibi");
    venus->attachMesh(sphere);
    m_scn_mgr->addGameObject(venus);
    auto* venusNode = m_scn_mgr->createSceneNode("VenusNode");
    venusNode->setParent(venusOrbitNode);
    venusNode->attachObject(venus);
    venusNode->setVisible(true);
    venusNode->setPosition(Vec3{2.2, 0, 0.0});
    venusNode->scale(vec3::mkVec3(0.3), ETransformSpace::Local);

    auto* earthOrbitNode = m_scn_mgr->createSceneNode("EarthOrbitNode");
    earthOrbitNode->setParent(m_scn_mgr->rootSceneNode());
    earthOrbitNode->scale(sunNode->scale(), ETransformSpace::Local);

    auto* earth = new GameObject("Earth", "Habibi");
    earth->attachMesh(sphere);
    m_scn_mgr->addGameObject(earth);
    auto* earthNode = m_scn_mgr->createSceneNode("EarthNode");
    earthNode->setInheritedTransformation(false, true, false);
    earthNode->setParent(earthOrbitNode);
    earthNode->attachObject(earth);
    earthNode->setVisible(true);
    earthNode->setPosition(Vec3{3.4, 0.0, 0.0});
    earthNode->scale(vec3::mkVec3(0.35), ETransformSpace::Local);

    auto* marsOrbitNode = m_scn_mgr->createSceneNode("MarsOrbitNode");
    marsOrbitNode->setParent(m_scn_mgr->rootSceneNode());
    marsOrbitNode->scale(sunNode->scale(), ETransformSpace::Local);

    auto* mars = new GameObject("Mars", "Habibi");
    mars->attachMesh(sphere);
    m_scn_mgr->addGameObject(mars);
    auto* marsNode = m_scn_mgr->createSceneNode("MarsNode");
    marsNode->setInheritedTransformation(false, true, false);
    marsNode->setParent(marsOrbitNode);
    marsNode->attachObject(mars);
    marsNode->setVisible(true);
    marsNode->setPosition(Vec3{5, 0.0, 0.0});
    marsNode->scale(vec3::mkVec3(0.15), ETransformSpace::Local);

    auto* jupiterOrbitNode = m_scn_mgr->createSceneNode("JupiterOrbitNode");
    jupiterOrbitNode->setParent(m_scn_mgr->rootSceneNode());
    jupiterOrbitNode->scale(sunNode->scale(), ETransformSpace::Local);

    auto* jupiter = new GameObject("Jupiter", "Habibi");
    jupiter->attachMesh(sphere);
    m_scn_mgr->addGameObject(jupiter);
    auto* jupiterNode = m_scn_mgr->createSceneNode("JupiterNode");
    jupiterNode->setParent(jupiterOrbitNode);
    jupiterNode->attachObject(jupiter);
    jupiterNode->setVisible(true);
    jupiterNode->setPosition(Vec3{7, 0.0, 0.0});
    jupiterNode->scale(vec3::mkVec3(0.7), ETransformSpace::Local);

    /*
    auto* saturnOrbitNode = m_scn_mgr->createSceneNode("SaturnOrbitNode");
    saturnOrbitNode->setParent(m_scn_mgr->rootSceneNode());
    saturnOrbitNode->scale(sunNode->scale(), ETransformSpace::Local);

    auto* saturn = new GameObject("Saturn", "Habibi");
    saturn->attachMesh(sphere);
    m_scn_mgr->addGameObject(saturn);
    auto* saturnNode = m_scn_mgr->createSceneNode("SaturnNode");
    saturnNode->setParent(saturnOrbitNode);
    saturnNode->attachObject(saturn);
    saturnNode->setVisible(true);
    saturnNode->setPosition(Vec3{9.6, 0.0, 0.0});
    saturnNode->scale(vec3::mkVec3(0.6), ETransformSpace::Local);
     */

    auto* uranusOrbitNode = m_scn_mgr->createSceneNode("UranusOrbitNode");
    uranusOrbitNode->setParent(m_scn_mgr->rootSceneNode());
    uranusOrbitNode->scale(sunNode->scale(), ETransformSpace::Local);

    auto* uranus = new GameObject("Uranus", "Habibi");
    uranus->attachMesh(sphere);
    m_scn_mgr->addGameObject(uranus);
    auto* uranusNode = m_scn_mgr->createSceneNode("UranusNode");
    uranusNode->setParent(uranusOrbitNode);
    uranusNode->attachObject(uranus);
    uranusNode->setVisible(true);
    uranusNode->setPosition(Vec3{10, 0.0, 0.0});
    uranusNode->scale(vec3::mkVec3(0.5), ETransformSpace::Local);

    auto* neptuneOrbitNode = m_scn_mgr->createSceneNode("NeptuneOrbitNode");
    neptuneOrbitNode->setParent(m_scn_mgr->rootSceneNode());
    neptuneOrbitNode->scale(sunNode->scale(), ETransformSpace::Local);

    auto* neptune = new GameObject("Neptune", "Habibi");
    neptune->attachMesh(sphere);
    m_scn_mgr->addGameObject(neptune);
    auto* neptuneNode = m_scn_mgr->createSceneNode("NeptuneNode");
    neptuneNode->setParent(neptuneOrbitNode);
    neptuneNode->attachObject(neptune);
    neptuneNode->setVisible(true);
    neptuneNode->setPosition(Vec3{11.5, 0.0, 0.0});
    neptuneNode->scale(vec3::mkVec3(0.45), ETransformSpace::Local);

    auto* moonOrbitNode = m_scn_mgr->createSceneNode("SaturnOrbitNode");
    moonOrbitNode->setParent(sunNode);
    moonOrbitNode->translate(earthNode->position(), ETransformSpace::Local);
    moonOrbitNode->scale(earthNode->scale(), ETransformSpace::Local);

    auto* moon = new GameObject("Moon", "Habibi");
    moon->attachMesh(sphere);
    m_scn_mgr->addGameObject(moon);
    auto* moonNode = m_scn_mgr->createSceneNode("MoonNode");
    moonNode->setInheritedTransformation(false, true, false);
    moonNode->setParent(moonOrbitNode);
    moonNode->attachObject(moon);
    moonNode->setVisible(true);
    moonNode->setPosition(Vec3{1.5, 0.0, 0.0});
    moonNode->scale(vec3::mkVec3(0.15), ETransformSpace::Local);

    m_solar_system.resize(20);
    m_solar_system[0] = sunNode;
    m_solar_system[1] = mercuryOrbitNode;
    m_solar_system[2] = mercuryNode;
    m_solar_system[3] = venusOrbitNode;
    m_solar_system[4] = venusNode;
    m_solar_system[5] = earthOrbitNode;
    m_solar_system[6] = earthNode;
    m_solar_system[7] = marsOrbitNode;
    m_solar_system[8] = marsNode;
    m_solar_system[9] = jupiterOrbitNode;
    m_solar_system[10] = jupiterNode;
    //m_solar_system[11] = saturnOrbitNode;
    //m_solar_system[12] = saturnNode;
    m_solar_system[13] = uranusOrbitNode;
    m_solar_system[14] = uranusNode;
    m_solar_system[15] = neptuneOrbitNode;
    m_solar_system[16] = neptuneNode;
    m_solar_system[17] = moonOrbitNode;
    m_solar_system[18] = moonNode;
  }

  // Camera
  m_camera = m_scn_mgr->createCamera("MainCamera");
  m_camera->pos = Vec3(0.0f, 8000.0f, 4000.0f);
  //m_camera->pos = Vec3(0.0f, 50.0f, 25.0f);

  m_vbos.resize(255);
  m_ebos.resize(255);

  //m_timer->setInterval(100);
  //connect(&*m_timer, &QTimer::timeout, this, &GLWidget::rotateView);
  //m_timer->start();

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

  m_scn_mgr->rootSceneNode()->update();
  auto nodes = m_scn_mgr->toBeRenderedSceneNodes();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  m_vao.bind();

  for (auto & node : nodes) {
    const auto& obj = node->attachedObject(0);
    int idx = obj->vboIndex();
    //qDebug() << "i:" << i;
    m_vbos[idx].bind();
    glEnableVertexAttribArray(0); glEnableVertexAttribArray(1); glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3*sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6*sizeof(float)));
    m_ebos[idx]->bind();

    qDebug() << "render" << obj->name();
    m_model = node->worldMatrix();
    //m_model = nodes[i]->worldMatrix();
    //m_model.translate(nodes[i]->combinedPosition());
    //m_model.rotate(nodes[i]->combinedOrientation());
    //m_model.scale(nodes[i]->combinedScale());
    //qDebug() << "S:" << nodes[i]->scale();
    //qDebug() << nodes[i]->name() << "O:" << nodes[i]->orientation();
    //qDebug() << nodes[i]->name() << "cO:" << nodes[i]->combinedOrientation();
    //qDebug() << nodes[i]->name() << "P:" << nodes[i]->position(); //<< "O:" << nodes[i]->combinedOrientation();
    //qDebug() << nodes[i]->name() << "cP:" << nodes[i]->combinedPosition(); //<< "O:" << nodes[i]->combinedOrientation();
    //m_model.rotate(90.0f, QVector3D(1.0f, 0.0f, 0.0f));
    //m_model.rotate(180.0f - (m_xRot / 16.0f), 1, 0, 0);
    //m_model.rotate(m_yRot / 16.0f, 0, 1, 0);
    //m_model.rotate(m_zRot / 16.0f, 0, 0, 1);
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
    m_proj.perspective(45.0f, GLfloat(w) / h, 0.01f, 50000.0f);
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

void GLWidget::solar_system_update(Real dt)
{
  // sun
  m_solar_system[0]->rotate(Quat::fromAxisAndAngle(vec3::AxisY, 0), ETransformSpace::Local);
  // mercury orbit
  m_solar_system[1]->rotate(Quat::fromAxisAndAngle(vec3::AxisY, dt / 1000 * 20), ETransformSpace::Local);
  // mercury
  m_solar_system[2]->rotate(Quat::fromAxisAndAngle(vec3::AxisY, dt / 1000 * 5), ETransformSpace::Local);
  // venus orbit
  m_solar_system[3]->rotate(Quat::fromAxisAndAngle(vec3::AxisY, dt / 1000 * 15), ETransformSpace::Local);
  // venus
  m_solar_system[4]->rotate(Quat::fromAxisAndAngle(vec3::AxisY, dt / 1000 * 5), ETransformSpace::Local);
  // earth orbit
  m_solar_system[5]->rotate(Quat::fromAxisAndAngle(vec3::AxisY, dt / 1000 * 30), ETransformSpace::Local);
  // earth
  m_solar_system[6]->rotate(Quat::fromAxisAndAngle(vec3::AxisY, dt / 1000 * 5), ETransformSpace::Local);
  // mars orbit
  m_solar_system[7]->rotate(Quat::fromAxisAndAngle(vec3::AxisY, dt / 1000 * 40), ETransformSpace::Local);
  // mars
  m_solar_system[8]->rotate(Quat::fromAxisAndAngle(vec3::AxisY, dt / 1000 * 5), ETransformSpace::Local);
  // jupiter orbit
  //m_solar_system[9]->rotate(Quat::fromAxisAndAngle(vec3::AxisY, dt / 1000 * 50), ETransformSpace::Local);
  // jupiter
  m_solar_system[10]->rotate(Quat::fromAxisAndAngle(vec3::AxisY, dt / 1000 * 5), ETransformSpace::Local);
  // saturn orbit
  //m_solar_system[11]->rotate(Quat::fromAxisAndAngle(vec3::AxisY, dt / 1000 * 40), ETransformSpace::Local);
  // saturn
  //m_solar_system[12]->rotate(Quat::fromAxisAndAngle(vec3::AxisY, dt / 1000 * 5), ETransformSpace::Local);
  // uranus orbit
  //m_solar_system[13]->rotate(Quat::fromAxisAndAngle(vec3::AxisY, dt / 1000 * 35), ETransformSpace::Local);
  // uranus
  m_solar_system[14]->rotate(Quat::fromAxisAndAngle(vec3::AxisY, dt / 1000 * 5), ETransformSpace::Local);
  // neptune orbit
  //m_solar_system[15]->rotate(Quat::fromAxisAndAngle(vec3::AxisY, dt / 1000 * 25), ETransformSpace::Local);
  // neptune
  m_solar_system[16]->rotate(Quat::fromAxisAndAngle(vec3::AxisY, dt / 1000 * 5), ETransformSpace::Local);
  // moon orbit
  m_solar_system[17]->rotate(Quat::fromAxisAndAngle(vec3::AxisY, dt / 1000 * 30), ETransformSpace::Local);
  // moon
  m_solar_system[18]->rotate(Quat::fromAxisAndAngle(vec3::AxisY, dt / 1000 * 5), ETransformSpace::Local);
  //qDebug() << m_solar_system[2]->position();
  //m_solar_system[2]->translate(m_solar_system[2]->position(), ETransformSpace::Local);

  //m_solar_system[3]->rotate(Quat::fromAxisAndAngle(vec3::AxisY, dt / 1000 * 20), ETransformSpace::Local);
  //m_solar_system[4]->rotate(Quat::fromAxisAndAngle(Vec3{1.0, 0.0, 0.0}, deltaLune), ETransformSpace::Parent);
  //m_solar_system[2]->rotate(Quat::fromAxisAndAngle(vec3::AxisY, dt / 1000 * 80), ETransformSpace::Parent);
  //m_solar_system[3]->rotate(Quat::fromAxisAndAngle(vec3::AxisY, dt / 1000 * 20), ETransformSpace::Parent);
}
