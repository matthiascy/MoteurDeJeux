#include <GameFramework/Systems/RenderSystem.hpp>
#include <QOpenGLFunctions>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>
#include <GameFramework/Scene.hpp>
#include <GameFramework/Engine.hpp>

RenderSystem::RenderSystem(const String& name, Engine* engine, Object* parent)
  : System(name, engine, parent),
    m_surface{nullptr}, m_render_infos{},
    m_projection_matrix{Math::Mat4Identity},
    m_model_matrix{Math::Mat4Identity},
    m_view_matrix{Math::Mat4Identity}
{
  qInfo() << "Render System creation =>" << objectName();
}

RenderSystem::~RenderSystem()
{
  m_surface.reset(nullptr);
}

void RenderSystem::init()
{
  qInfo() << "- Render system initialization...\n"
          << "\t- OffScreenSurface creation.";

  m_surface = makeUnique<OglOffscreenSurface>();
  m_surface->create();
  if (!m_surface->isValid())
    qFatal("OffScreenSurface Creation failed!!!");
}

void RenderSystem::renderScene(Scene* scene)
{
  m_surface->render();

  if (scene) {

    //GameObject *camera = scene->mainCamera();

    //m_projection_matrix.perspective(45.0f, GLfloat(m_size.width()) / m_size.height(), 0.01f, 500000.0f);
    //m_program->setUniformValue("projMatrix", m_projection);
    //m_program->setUniformValue("normalMatrix", m_model);
  }

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
}

QImage RenderSystem::grabFramebuffer() const
{
  return m_surface->grabFramebuffer();
}
