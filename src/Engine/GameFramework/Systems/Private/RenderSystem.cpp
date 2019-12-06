#include <GameFramework/Systems/RenderSystem.hpp>
#include <QOpenGLFunctions>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>
#include <GameFramework/Scene.hpp>
#include <GameFramework/Engine.hpp>

RenderSystem::RenderSystem(const String& name, Engine* engine, Object* parent)
  : System(name, engine, parent),
    m_surface{nullptr}, m_render_graph{},
    m_projection_matrix{Math::Mat4Identity},
    m_model_matrix{Math::Mat4Identity},
    m_view_matrix{Math::Mat4Identity},
    program{nullptr}, vao{nullptr}, vbo{nullptr}, ebo{nullptr},
    m_fns{nullptr}
{
  qInfo() << "Creation =>" << objectName();
  m_programs.push_back(new OglProgram);
  m_vaos.push_back(new OglVAO);
}

RenderSystem::~RenderSystem()
{
  qDebug() << "Shutting down...";
  m_surface.reset(nullptr);
  qDebug() << "Shutting down...[Finished]";
}

void RenderSystem::init()
{
  qInfo() << "Initialization...";

  m_surface = makeUnique<OglOffscreenSurface>();
  m_surface->create();
  if (!m_surface->isValid())
    qFatal("OffScreenSurface Creation failed!!!");

  m_programs[0]->addShaderFromSourceFile(OglShader::Vertex, ":/Shaders/BasicVert");
  m_programs[0]->addShaderFromSourceFile(OglShader::Fragment, ":/Shaders/BasicVert");
  m_programs[0]->bindAttributeLocation("inPosition", 0);
  m_programs[0]->bindAttributeLocation("inNormal", 1);
  m_programs[0]->bindAttributeLocation("inTexCoord", 2);
  m_programs[0]->link();
  m_programs[0]->bind();
  m_programs[0]->setUniformValue("texture0", 0);
  m_programs[0]->setUniformValue("texture1", 1);
  m_programs[0]->setUniformValue("texture2", 2);
  m_programs[0]->setUniformValue("texture3", 3);
  m_programs[0]->setUniformValue("lightPos", Vec3(0, 0, 70));
  m_programs[0]->release();

  m_vaos[0]->create();

/*
  float triangle[] = {
      -0.5f, -0.5f, 0.0f,
       0.5f, -0.5f, 0.0f,
       0.0f, 0.5f,  0.0f,
  };
  UInt32 indices[] = {
      0, 1, 2
  };
  program = new OglProgram;
  program->addShaderFromSourceFile(OglShader::Vertex, ":/Shaders/SimplePosition");
  program->addShaderFromSourceFile(OglShader::Fragment, ":/Shaders/SimpleColor");
  program->bindAttributeLocation("inPosition", 0);
  program->link();
  program->bind();
  vao = new OglVAO;
  vao->create();
  vao->bind();
  vbo = new OglBuffer(OglBuffer::VertexBuffer);
  vbo->create();
  vbo->bind();
  vbo->setUsagePattern(OglBuffer::StaticDraw);
  vbo->allocate(triangle, 24 * sizeof(float));;
  m_surface->fns()->glEnableVertexAttribArray(0);
  m_surface->fns()->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
  ebo = new OglBuffer(OglBuffer::IndexBuffer);
  ebo->create();
  ebo->bind();
  ebo->setUsagePattern(OglBuffer::StaticDraw);
  ebo->allocate(indices, 3 * sizeof(UInt32));
  program->release();
  */

  m_surface->fns()->glEnable(GL_DEPTH_TEST);
  m_surface->fns()->glDepthFunc(GL_LESS);
  m_surface->fns()->glViewport(0, 0, m_surface->size().width(), m_surface->size().height());
  m_surface->fns()->glClearColor(0.5, 0.4, 0.8, 1.0);
  m_surface->doneCurrent();
}

void RenderSystem::renderScene(Scene* scene)
{
  m_surface->makeCurrent();

  vao->bind();
  program->bind();
  m_surface->fns()->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  m_surface->fns()->glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)nullptr);

  program->release();
  vao->release();
  m_surface->swapBuffers();

  if (scene) {
    GameObject* camera = scene->mainCamera();

    for (GameObject* gameObject : scene->gameObjects()) {
      if (gameObject->isVisible()) {
        if (!m_render_graph.contains(gameObject)) {
          m_render_graph.insert(gameObject, {m_vbos.size(), m_ibos.size()});
          m_vbos.push_back(new OglBuffer(OglBuffer::VertexBuffer));
          m_ibos.push_back(new OglBuffer(OglBuffer::IndexBuffer));
          m_vbos[m_render_graph[gameObject].vboIdx]->create();
          m_vbos[m_render_graph[gameObject].vboIdx]->bind();
          m_vbos[m_render_graph[gameObject].vboIdx]->setUsagePattern(OglBuffer::StaticDraw);
          Mesh* gameObjectMesh = m_engine->assetManager()->getMesh(gameObject->meshHandle());
          m_vbos[m_render_graph[gameObject].vboIdx]->allocate(gameObjectMesh->constData(), gameObjectMesh->count() * sizeof(float));

        }
      }
    }

    m_programs[0]->bind();
    m_programs[0]->setUniformValue("projMatrix", );
    m_programs[0]->setUniformValue("modelViewMatrix", );

    for (const auto& info : m_render_graph) {

    }

    m_programs[0]->release();

  }

  m_surface->doneCurrent();
  m_surface->render();

  /*
  if (scene) {
    GameObject* camera = scene->mainCamera();

    for (GameObject *gameObject : scene->gameObjects()) {
      if (gameObject->isVisible()) {
        render(gameObject);
      }
    }
  }
   */

  //if (scene) {


    //GameObject *camera = scene->mainCamera();

    //m_projection_matrix.perspective(45.0f, GLfloat(m_size.width()) / m_size.height(), 0.01f, 500000.0f);
    //m_program->setUniformValue("projMatrix", m_projection);
    //m_program->setUniformValue("normalMatrix", m_model);
  //}

  //m_program->setUniformValue("projMatrix", camera->getComponent<Camera>()->projectionMatrix());


    /*
    for (auto& obj : scene->gameObjects()) {
      if (obj->isVisible()) {
        render(obj);
      }
    }
  Q_INIT_RESOURCE(resources);
  auto meshHandle = m_engine->assetManager()->loadMesh("./sphere.obj");
  Mesh* mesh = m_engine->assetManager()->getMesh(meshHandle);
  auto vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
  auto ebo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
  vbo->create(); vbo->bind();
  vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
  vbo->allocate(mesh->constData(), mesh->count() * sizeof(float));
  ebo->create(); ebo->bind();
  ebo->setUsagePattern(QOpenGLBuffer::StaticDraw);
  ebo->allocate(mesh->indices().data(), mesh->vertexCount() * sizeof(UInt32));

  //vbo->bind(); ebo->bind();
  m_fns->glEnableVertexAttribArray(0);
  m_fns->glEnableVertexAttribArray(1);
  m_fns->glEnableVertexAttribArray(2);
  m_fns->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
  m_fns->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3*sizeof(float)));
  m_fns->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6*sizeof(float)));
  m_fns->glDrawElements(GL_TRIANGLES, mesh->indices().size(), GL_UNSIGNED_INT, (void*)nullptr);


    m_program->release();
    m_painter->endNativePainting();
    m_painter->end();
    QImage img = m_fbo->toImage();
    qDebug() << "Saving fbo to image...";
    img.save("fbo.png");
    m_fbo->release();
    m_context->doneCurrent();
  //}
   */
}

void RenderSystem::render(GameObject* gameObject)
{
  /*
  // TODO: set model view matrix
  m_program->setUniformValue("modelViewMatrix", m_view * m_model);
  // Not in render system
  if (m_render_infos.find(gameObject) == m_render_infos.end()) {

    auto vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    auto ebo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    vbo->create();
    vbo->bind();
    vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
    Mesh* mesh = m_engine->assetManager()->getMesh(gameObject->meshHandle());
    vbo->allocate(mesh->constData(), mesh->count() * sizeof(float));
    m_vbos->push_back(vbo);
    ebo->create();
    ebo->setUsagePattern(QOpenGLBuffer::StaticDraw);
    ebo->allocate(mesh->indices().data(), mesh->vertexCount() * sizeof(UInt32));
    m_ebos->push_back(ebo);
    m_render_infos.insert(gameObject, {
        m_vbos->size()-1, m_ebos->size()-1
    });

  } else {

    Mesh* mesh = m_engine->assetManager()->getMesh(gameObject->meshHandle());
    (*m_vbos)[m_render_infos[gameObject].vboIdx]->bind();
    m_fns->glEnableVertexAttribArray(0);
    m_fns->glEnableVertexAttribArray(1);
    m_fns->glEnableVertexAttribArray(2);
    m_fns->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
    m_fns->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3*sizeof(float)));
    m_fns->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6*sizeof(float)));
    m_fns->glDrawElements(GL_TRIANGLES, mesh->indices().size(), GL_UNSIGNED_INT, (void*)nullptr);
  }
  */
  m_surface->makeCurrent();

  m_surface->doneCurrent();
}

QImage RenderSystem::grabFramebuffer() const
{
  return m_surface->grabFramebuffer();
}

void RenderSystem::resize(const QSize& size)
{
  m_surface->resize(size);
}
