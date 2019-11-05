#include "GLWidget.hpp"
#include "MeshLoader.hpp"
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>
#include <iostream>
#include <math.h>
#include <QtMath>

GLWidget::GLWidget(QWidget *parent)
  : QOpenGLWidget(parent), m_scn_mgr{}, m_xRot(0), m_yRot(0), m_zRot(0), m_program(nullptr)
{
  m_scn_mgr = memory::mkUnique<SceneManager>();
  m_timer = memory::mkUnique<QTimer>();

  // Terrain
  {
    auto* terrain = new Terrain();
    terrain->create(1023, 1023, Image(":/Textures/heightmap"), false, 100.0f, 100.0f);
    m_scn_mgr->addGameObject(terrain);
    SceneNode* terrainNode = m_scn_mgr->createSceneNode("TerrainNode");
    terrainNode->setParent(m_scn_mgr->rootSceneNode());
    terrainNode->attachObject(terrain);
    terrainNode->setVisible(true);
    terrainNode->scale(Vec3{0.5f, 0.5f, 0.5f}, ETransformSpace::Local);
  }

  // Soleil
  {
    auto* sun = new GameObject("Sun", "Habibi");
    sun->attachMesh(MeshLoader::loadMesh("./sphere.obj"));
    m_scn_mgr->addGameObject(sun);
    SceneNode* sunNode = m_scn_mgr->createSceneNode("SunNode");
    sunNode->setParent(m_scn_mgr->rootSceneNode());
    sunNode->attachObject(sun);
    sunNode->setVisible(true);
    sunNode->setPosition(vec3::Zero);
    sunNode->scale(Vec3(50.0f, 50.0f, 50.0f), ETransformSpace::Local);
  }

  // Camera
  m_camera = m_scn_mgr->createCamera("MainCamera");
  m_camera->pos = Vec3(0.0f, 500.0f, 1000.0f);

  m_vbos.resize(255);

  m_timer->setInterval(100);
  connect(&*m_timer, &QTimer::timeout, this, &GLWidget::rotateView);
  m_timer->start();
  m_is_ctrl_pressed = false;
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
    m_rock_texture->destroy();
    m_snow_rock_texture->destroy();
    m_ocean_texture->destroy();
    for (auto& vbo : m_vbos) vbo.destroy();
    delete m_program;
    m_program = nullptr;
    doneCurrent();
}

void GLWidget::initializeGL()
{
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &GLWidget::cleanup);
    QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
    initializeOpenGLFunctions();
    f->glClearColor(0.2f, 0.3f, 0.4f, 1.0f);

    m_program = new QOpenGLShaderProgram;
    // fonction qui contient glCreateShader, glShaderSource, glCompileShader, glCreateProgram, glAttachShader
    m_program->addShaderFromSourceFile(OpenGLShader::Vertex, ":/Shaders/BasicVert");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Shaders/BasicFrag");
    m_program->bindAttributeLocation("inPosition", 0);
    m_program->bindAttributeLocation("inNormal", 1);
    m_program->bindAttributeLocation("inTexCoord", 2);
    m_program->link();
    m_program->bind();
    m_program->setUniformValue("texGrass", 0);
    m_program->setUniformValue("texRock", 1);
    m_program->setUniformValue("texSnowRock", 2);
    m_program->setUniformValue("texOcean", 3);

    m_projMatrixLoc = m_program->uniformLocation("projMatrix");  // egale a glGetUniformLocation()
    m_mvMatrixLoc = m_program->uniformLocation("modelViewMatrix");
    m_normalMatrixLoc = m_program->uniformLocation("normalMatrix");
    m_lightPosLoc = m_program->uniformLocation("lightPos");

    m_vao.create();
    m_vao.bind();

    for (auto& vbo : m_vbos) {
      vbo.create();
    }

    // Light position is fixed.
    m_program->setUniformValue(m_lightPosLoc, QVector3D(0, 0, 70));
    m_program->release();
    setupTextures();
  f->glEnable(GL_DEPTH_TEST);
  //f->glEnable(GL_CULL_FACE);
  //f->glCullFace(GL_BACK);
  //f->glFrontFace(GL_CW);
}

void GLWidget::setupTextures()
{
  m_grass_texture = std::make_unique<QOpenGLTexture>(QImage(":/Textures/grass").mirrored());
  m_rock_texture = std::make_unique<QOpenGLTexture>(QImage(":/Textures/rock").mirrored());
  m_snow_rock_texture = std::make_unique<QOpenGLTexture>(QImage(":/Textures/snowrock").mirrored());
  m_ocean_texture = std::make_unique<QOpenGLTexture>(QImage(":/Textures/ocean").mirrored());
}

void GLWidget::paintGL()
{
  m_scn_mgr->rootSceneNode()->update(true);
  //auto objs = m_scn_mgr->toBeRenderedGameObjects();
  auto nodes = m_scn_mgr->toBeRenderedSceneNodes();
  //qDebug() << objs.size() << "objects will be rendered...";
  qDebug() << nodes.size() << "nodes will be rendered...";

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  m_vao.bind();

  for (auto i = 0; i < nodes.size(); ++i) {
    m_vbos[i].bind();
    const auto& obj = nodes[i]->attachedObject(0);
    m_vbos[i].allocate(obj->mesh()->constData(), obj->mesh()->count() * sizeof(float));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    // position, normal, tex coord
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3*sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6*sizeof(float)));

    m_grass_texture->bind(0);
    m_rock_texture->bind(1);
    m_snow_rock_texture->bind(2);
    m_ocean_texture->bind(3);

    m_model.setToIdentity();
    //qDebug() << nodes[i]->combinedScale();
    qDebug() << nodes[i]->scale();
    m_model.scale(nodes[i]->scale());
    m_model.rotate(90.0f, QVector3D(1.0f, 0.0f, 0.0f));
    m_model.rotate(180.0f - (m_xRot / 16.0f), 1, 0, 0);
    m_model.rotate(m_yRot / 16.0f, 0, 1, 0);
    m_model.rotate(m_zRot / 16.0f, 0, 0, 1);
    QMatrix3x3 normalMatrix = m_model.normalMatrix();

    m_program->bind();
    m_program->setUniformValue(m_projMatrixLoc, m_proj);
    m_program->setUniformValue(m_mvMatrixLoc,
                               m_camera->ViewMatrix() * m_model);
    m_program->setUniformValue(m_normalMatrixLoc, normalMatrix);

    // TODO by index
    glDrawArrays(GL_TRIANGLES, 0, obj->mesh()->vertexCount());
  }

  //qDebug() << "Release program...";
  m_program->release();
  //qDebug() << "Finish rendering...\n";
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

    m_camera->pitch += num_degrees.y() >= 0 ? 1.0f : -1.0f;

  } else {

    QVector3D dir = QVector3D::crossProduct(m_camera->up, m_camera->right);
    m_camera->pos += num_degrees.y() >= 0 ? 4.0f * dir.normalized() : -4.0f * dir.normalized();
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
  std::cout << "ctrl released" << std::endl;
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
