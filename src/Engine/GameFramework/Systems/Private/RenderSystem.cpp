#include <GameFramework/Systems/RenderSystem.hpp>
#include <QOpenGLFunctions>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>
#include <GameFramework/Scene.hpp>
#include <GameFramework/Engine.hpp>

RenderSystem::RenderSystem(String name, Engine* engine)
  : System(std::move(name), engine), m_context{nullptr}, m_surface{nullptr}
{
  qDebug() << "Render System creation =>" << m_name;
  m_vao = nullptr;
  m_vbos = {};
  m_ebos = {};
  m_fbo = nullptr;
  m_program = nullptr;
  m_size = {800, 600};
  m_projection.setToIdentity();
  m_view.setToIdentity();
  m_model.setToIdentity();
  m_device = new QOpenGLPaintDevice(m_size);
  m_painter = new QPainter(m_device);
  m_render_map = {};
  m_fns = nullptr;
}

RenderSystem::~RenderSystem()
{
  m_surface.reset(nullptr);
  m_context.reset(nullptr);
  m_vao.reset(nullptr);
  for (auto& e : *m_vbos)
    delete e;
  m_vbos.reset(nullptr);
  for (auto& e : *m_ebos)
    delete e;
  m_ebos.reset(nullptr);
  m_fbo.reset(nullptr);
  m_program.reset(nullptr);
  delete m_device;
  delete m_painter;
}

void RenderSystem::init()
{
  qDebug() << "\tRender system initialization";

  QSurfaceFormat format;
  format.setRenderableType(QSurfaceFormat::OpenGL);
  format.setProfile(QSurfaceFormat::CoreProfile);
  format.setVersion(4, 0);
  format.setDepthBufferSize(24);
  format.setSamples(16);
  QSurfaceFormat::setDefaultFormat(format);

  qDebug() << "\t\t1. Context creation : OpenGL Core Profile 4.6.0.";
  m_context = makeUnique<QOpenGLContext>();
  if (!m_context->create())
    qDebug() << "\t\t\t!!!Creation failed";

  qDebug() << "\t\t2. OffScreenSurface creation.";
  m_surface = makeUnique<OglOffscreenSurface>();
  m_surface->create();
  m_surface->init();

  if (!m_surface->isValid())
    qDebug() << "\t\t\tCreation failed";

  m_context->makeCurrent(m_surface.get());
  m_fns = m_context->functions();
  m_fbo = makeUnique<QOpenGLFramebufferObject>(m_size);
  m_vao = makeUnique<QOpenGLVertexArrayObject>();
  m_program = makeUnique<QOpenGLShaderProgram>();

  m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Shaders/BasicVert");
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
  m_program->setUniformValue("lightPos", QVector3D(0, 0, 70));
  m_vao->create();
  m_fns->glEnable(GL_DEPTH_TEST);
  m_fns->glDepthFunc(GL_LESS);
  m_fns->glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
  m_program->release();
  m_context->doneCurrent();
}

void RenderSystem::renderScene(Scene* scene)
{
  //if (scene) {
    m_device->setSize(m_size);
    qDebug() << "Is painter active ? " << m_painter->isActive();
    m_painter->setRenderHint(QPainter::RenderHint::Antialiasing, true);

    m_context->makeCurrent(m_surface.get());
    m_painter->beginNativePainting();
    m_fns->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_fbo->bind();
    m_vao->bind();
    m_program->bind();

    //GameObject *camera = scene->mainCamera();

    //m_program->setUniformValue("projMatrix", camera->getComponent<Camera>()->projectionMatrix());
    m_projection.perspective(45.0f, GLfloat(m_size.width()) / m_size.height(), 0.01f, 500000.0f);
    m_program->setUniformValue("projMatrix", m_projection);
    m_program->setUniformValue("normalMatrix", m_model);

    /*
    for (auto& obj : scene->gameObjects()) {
      if (obj->isVisible()) {
        render(obj);
      }
    }
     */
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
}

void RenderSystem::render(GameObject* gameObject)
{
  // TODO: set model view matrix
  m_program->setUniformValue("modelViewMatrix", m_view * m_model);
  // Not in render system
  if (m_render_map.find(gameObject) == m_render_map.end()) {

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
    m_render_map.insert(gameObject, {
        m_vbos->size()-1, m_ebos->size()-1
    });

  } else {

    Mesh* mesh = m_engine->assetManager()->getMesh(gameObject->meshHandle());
    (*m_vbos)[m_render_map[gameObject].vboIdx]->bind();
    m_fns->glEnableVertexAttribArray(0);
    m_fns->glEnableVertexAttribArray(1);
    m_fns->glEnableVertexAttribArray(2);
    m_fns->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
    m_fns->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3*sizeof(float)));
    m_fns->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6*sizeof(float)));
    m_fns->glDrawElements(GL_TRIANGLES, mesh->indices().size(), GL_UNSIGNED_INT, (void*)nullptr);
  }
}
