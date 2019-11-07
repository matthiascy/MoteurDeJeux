#include "EngineRenderer.hpp"
#include "SceneManager.hpp"
#include "Terrain.hpp"
#include "MeshLoader.hpp"

EngineRenderer::EngineRenderer(QObject* parent /* = nullptr */)
    : QObject(parent), m_scn_mgr{}, m_vao{}, m_vbos{}, m_program{}, m_camera{nullptr},
      m_proj_mat_loc{}, m_model_view_mat_loc{}, m_normal_mat_loc{}, m_light_pos_loc{},
      m_window{nullptr}
{
  m_scn_mgr = memory::mkUnique<SceneManager>();

  // Terrain
  {
    auto* terrain = new Terrain();
    terrain->create(1023, 1023, Image(":/Textures/heightmap"), false, 100.0f, 100.0f);
    m_scn_mgr->addGameObject(terrain);
    SceneNode* terrainNode = m_scn_mgr->createSceneNode("TerrainNode");
    terrainNode->setParent(m_scn_mgr->rootSceneNode());
    terrainNode->attachObject(terrain);
    terrainNode->setVisible(true);
    terrainNode->scale(Vec3{0.5, 0.5, 0.5}, ETransformSpace::Local);
  }
  /*
  // Sun
  {
    auto* sun = new GameObject("Sun", "Star");
    sun->attachMesh(MeshLoader::loadMesh("./sphere.obj"));
    m_scn_mgr->addGameObject(sun);
    SceneNode* sunNode = m_scn_mgr->createSceneNode("SunNode");
    sunNode->setParent(m_scn_mgr->rootSceneNode());
    sunNode->attachObject(sun);
    sunNode->setVisible(true);
    sunNode->setPosition(Vec3{0.0, 10.0, 0.0});
    sunNode->scale(Vec3{50.0, 50.0, 50.0}, ETransformSpace::Local);
  }
  */

  m_camera = m_scn_mgr->createCamera("MainCamera");
  m_camera->pos = Vec3(0.0, 500.0, 1000);
}

EngineRenderer::~EngineRenderer()
{
  for (auto& vbo : m_vbos) {
    vbo.destroy();
  }

  m_grass_texture.reset();
  m_rock_texture.reset();
  m_snow_rock_texture.reset();
  m_ocean_texture.reset();
}

void EngineRenderer::initializeGL()
{
  initializeOpenGLFunctions();

  m_vbos.resize(255);

  if (!m_program.addShaderFromSourceFile(OpenGLShader::Vertex, ":/Shaders/BasicVert"))
    qDebug() << m_program.log();

  if (!m_program.addShaderFromSourceFile(OpenGLShader::Fragment, ":/Shaders/BasicFrag"))
    qDebug() << m_program.log();

  m_program.bindAttributeLocation("inPosition", 0);
  m_program.bindAttributeLocation("inNormal", 1);
  m_program.bindAttributeLocation("inTexCoord", 2);

  if (!m_program.link())
    qDebug() << m_program.log();

  if (!m_program.bind())
    qDebug() << m_program.log();

  m_program.setUniformValue("texGrass", 0);
  m_program.setUniformValue("texRock", 1);
  m_program.setUniformValue("texSnowRock", 2);
  m_program.setUniformValue("texOcean", 3);
  m_proj_mat_loc = m_program.uniformLocation("projMatrix");
  m_model_view_mat_loc = m_program.uniformLocation("modelViewMatrix");
  m_normal_mat_loc = m_program.uniformLocation("normalMatrix");
  m_light_pos_loc = m_program.uniformLocation("lightPos");

  m_vao.create();
  m_vao.bind();

  for (auto& vbo : m_vbos)
    vbo.create();

  m_program.setUniformValue(m_light_pos_loc, Vec3{0.0, 0.0, 70.0});
  m_program.release();

  // textures
  m_grass_texture = memory::mkUnique<OpenGLTexture>(Image(":/Textures/grass").mirrored());
  m_rock_texture = memory::mkUnique<OpenGLTexture>(Image(":/Textures/rock").mirrored());
  m_snow_rock_texture = memory::mkUnique<OpenGLTexture>(Image(":/Textures/snowrock").mirrored());
  m_ocean_texture = memory::mkUnique<OpenGLTexture>(Image(":/Textures/ocean").mirrored());
  m_grass_texture->setMinificationFilter(QOpenGLTexture::Nearest);
  m_grass_texture->setMagnificationFilter(QOpenGLTexture::Linear);
  m_grass_texture->setWrapMode(QOpenGLTexture::Repeat);

  glEnable(GL_DEPTH_TEST);
  //glEnable(GL_CULL_FACE);
  //glEnable(GL_BACK);
  //glEnable(GL_CW);
}

void EngineRenderer::resizeGL(Int32 w, Int32 h)
{
  glViewport(0, 0, w, h);
  m_proj.setToIdentity();
  m_proj.perspective(45.0f, GLfloat(w) / h, 0.01f, 10000.0f);
}

void EngineRenderer::render()
{
  static int iii = 0;
  qDebug() << "Render......" << ++iii;
  m_scn_mgr->rootSceneNode()->update();
  auto nodes = m_scn_mgr->toBeRenderedSceneNodes();
  glClearColor(0.251, 0.231, 0.259, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  m_vao.bind();

  for (auto i = 0; i < nodes.size(); ++i) {
    m_vbos[i].bind();
    const auto& obj = nodes[i]->attachedObject(0);
    m_vbos[i].allocate(obj->mesh()->constData(), obj->mesh()->count() * sizeof(float));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3*sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6*sizeof(float)));

    m_grass_texture->bind(0);
    m_rock_texture->bind(1);
    m_snow_rock_texture->bind(2);
    m_ocean_texture->bind(3);

    m_model.setToIdentity();
    m_model.scale(nodes[i]->scale());
    Mat3 normalMatrix = m_model.normalMatrix();

    m_program.bind();
    m_program.setUniformValue(m_proj_mat_loc, m_proj);
    m_program.setUniformValue(m_model_view_mat_loc,
                               m_camera->ViewMatrix() * m_model);
    m_program.setUniformValue(m_normal_mat_loc, normalMatrix);

    // TODO by index
    glDrawArrays(GL_TRIANGLES, 0, obj->mesh()->vertexCount());
  }

  //glDrawElements(GL_TRIANGLE_STRIP, 34, GL_UNSIGNED_SHORT, 0);
  m_program.release();
  m_window->resetOpenGLState();
}

/*
void EngineRenderer::initializeCube()
{
  VertexData vertices[] =
      {
          // Vertex data for face 0
          {QVector3D(-1.0f, -1.0f,  1.0f), QVector3D(1.0f, 0.0f, 0.0f), QVector2D(0.0f, 0.0f)},  // v0
          {QVector3D( 1.0f, -1.0f,  1.0f), QVector3D(0.0f, 1.0f, 0.0f), QVector2D(0.33f, 0.0f)}, // v1
          {QVector3D(-1.0f,  1.0f,  1.0f), QVector3D(0.0f, 0.0f, 1.0f), QVector2D(0.0f, 0.5f)},  // v2
          {QVector3D( 1.0f,  1.0f,  1.0f), QVector3D(1.0f, 1.0f, 1.0f), QVector2D(0.33f, 0.5f)}, // v3

          // Vertex data for face 1
          {QVector3D( 1.0f, -1.0f,  1.0f), QVector3D(1.0f, 1.0f, 0.0f), QVector2D(0.0f, 0.5f)}, // v4
          {QVector3D( 1.0f, -1.0f, -1.0f), QVector3D(1.0f, 0.0f, 0.7f), QVector2D(0.33f, 0.5f)}, // v5
          {QVector3D( 1.0f,  1.0f,  1.0f), QVector3D(1.0f, 0.0f, 0.0f), QVector2D(0.0f, 1.0f)},  // v6
          {QVector3D( 1.0f,  1.0f, -1.0f), QVector3D(1.0f, 0.0f, 0.0f), QVector2D(0.33f, 1.0f)}, // v7

          // Vertex data for face 2
          {QVector3D( 1.0f, -1.0f, -1.0f), QVector3D(1.0f, 0.0f, 1.0f), QVector2D(0.66f, 0.5f)}, // v8
          {QVector3D(-1.0f, -1.0f, -1.0f), QVector3D(0.5f, 0.0f, 0.7f), QVector2D(1.0f, 0.5f)},  // v9
          {QVector3D( 1.0f,  1.0f, -1.0f), QVector3D(1.0f, 0.4f, 0.0f), QVector2D(0.66f, 1.0f)}, // v10
          {QVector3D(-1.0f,  1.0f, -1.0f), QVector3D(1.0f, 1.0f, 0.0f), QVector2D(1.0f, 1.0f)},  // v11

          // Vertex data for face 3
          {QVector3D(-1.0f, -1.0f, -1.0f), QVector3D(0.0f, 1.0f, 1.0f), QVector2D(0.66f, 0.0f)}, // v12
          {QVector3D(-1.0f, -1.0f,  1.0f), QVector3D(0.0f, 0.0f, 1.0f), QVector2D(1.0f, 0.0f)},  // v13
          {QVector3D(-1.0f,  1.0f, -1.0f), QVector3D(1.0f, 0.0f, 0.5f), QVector2D(0.66f, 0.5f)}, // v14
          {QVector3D(-1.0f,  1.0f,  1.0f), QVector3D(1.0f, 0.0f, 1.0f), QVector2D(1.0f, 0.5f)},  // v15

          // Vertex data for face 4
          {QVector3D(-1.0f, -1.0f, -1.0f), QVector3D(0.0f, 0.0f, 1.0f), QVector2D(0.33f, 0.0f)}, // v16
          {QVector3D( 1.0f, -1.0f, -1.0f), QVector3D(0.0f, 1.0f, 0.5f), QVector2D(0.66f, 0.0f)}, // v17
          {QVector3D(-1.0f, -1.0f,  1.0f), QVector3D(1.0f, 0.0f, 0.0f), QVector2D(0.33f, 0.5f)}, // v18
          {QVector3D( 1.0f, -1.0f,  1.0f), QVector3D(0.7f, 0.5f, 0.0f), QVector2D(0.66f, 0.5f)}, // v19

          // Vertex data for face 5
          {QVector3D(-1.0f,  1.0f,  1.0f), QVector3D(0.0f, 1.0f, 0.0f), QVector2D(0.33f, 0.5f)}, // v20
          {QVector3D( 1.0f,  1.0f,  1.0f), QVector3D(1.0f, 0.0f, 0.0f), QVector2D(0.66f, 0.5f)}, // v21
          {QVector3D(-1.0f,  1.0f, -1.0f), QVector3D(0.0f, 0.0f, 0.0f), QVector2D(0.33f, 1.0f)}, // v22
          {QVector3D( 1.0f,  1.0f, -1.0f), QVector3D(1.0f, 0.5f, 0.5f), QVector2D(0.66f, 1.0f)}  // v23
      };

  GLushort indices[] =
      {
          0,  1,  2,  3,  3,     // Face 0 - triangle strip ( v0,  v1,  v2,  v3)
          4,  4,  5,  6,  7,  7, // Face 1 - triangle strip ( v4,  v5,  v6,  v7)
          8,  8,  9, 10, 11, 11, // Face 2 - triangle strip ( v8,  v9, v10, v11)
          12, 12, 13, 14, 15, 15, // Face 3 - triangle strip (v12, v13, v14, v15)
          16, 16, 17, 18, 19, 19, // Face 4 - triangle strip (v16, v17, v18, v19)
          20, 20, 21, 22, 23      // Face 5 - triangle strip (v20, v21, v22, v23)
      };

  m_vbo.bind();
  m_vbo.allocate(vertices, 24 * sizeof (VertexData));

  m_ebo.bind();
  m_ebo.allocate(indices, 34 * sizeof (GLushort));
}
*/

void EngineRenderer::keyPressEvent(QKeyEvent* event)
{
  //qDebug() << "Renderer press" << event->key();
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
  }

  qDebug() << "KeyPress";

  event->accept();
  m_window->update();
}