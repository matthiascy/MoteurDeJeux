#include <GameFramework/Systems/RenderSystem.hpp>
#include <QOpenGLFunctions>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>
#include <GameFramework/Scene.hpp>
#include <GameFramework/Engine.hpp>
#include <GameFramework/Components/PerspectiveCamera.hpp>
#include <Graphics/Components/MeshRenderer.hpp>
#include <GameFramework/Managers/SceneManager.hpp>
#include <GameFramework/GameObject.hpp>


RenderSystem::RenderSystem(const String& name, Engine* engine, Object* parent)
  : System(name, engine, parent),
    m_surface{nullptr}, m_render_graph{},
    m_projection_matrix{Math::Mat4Identity},
    m_model_matrix{Math::Mat4Identity},
    m_view_matrix{Math::Mat4Identity},
    m_fns{nullptr}
{
  qInfo() << "Creation =>" << objectName();
}

RenderSystem::~RenderSystem()
{
  qDebug() << "Shutting down...";
  m_surface.reset(nullptr);
  _destroy_array(m_vaos);
  _destroy_array(m_vbos);
  _destroy_array(m_ibos);
  _destroy_array(m_programs);
  qDebug() << "Shutting down...[Finished]";
}

void RenderSystem::init()
{
  qInfo() << "Initialization...";

  m_surface = makeUnique<OglOffscreenSurface>();
  m_surface->create();
  if (!m_surface->isValid())
    qFatal("OffScreenSurface Creation failed!!!");

  m_fns = m_surface->fns();

  m_textures.insert(0, new OglTexture(Image(":/Assets/checker000").mirrored()));
  m_textures.insert(1, new OglTexture(Image(":/Textures/sun").mirrored()));
  m_textures.insert(2, new OglTexture(Image(":/Textures/mercury").mirrored()));
  m_textures.insert(3, new OglTexture(Image(":/Textures/venus").mirrored()));
  m_textures.insert(4, new OglTexture(Image(":/Textures/earth").mirrored()));

  const char* vert_shader = "#version 330 core\n"
                            "\n"
                            "layout (location = 0) in vec3 inPosition;\n"
                            "layout (location = 1) in vec3 inNormal;\n"
                            "layout (location = 2) in vec2 inTexCoord;\n"
                            "\n"
                            "out vec3 aPosition;\n"
                            "out vec3 aNormal;\n"
                            "out vec2 aTexCoord;\n"
                            "\n"
                            "uniform mat4 modelMatrix;\n"
                            "uniform mat4 viewMatrix;\n"
                            "uniform mat4 projectionMatrix;\n"
                            "uniform mat3 normalMatrix;\n"
                            "\n"
                            "\n"
                            "void main()\n"
                            "{\n"
                            "    aPosition = inPosition;\n"
                            "    aNormal = normalMatrix * inNormal;\n"
                            "    aTexCoord = inTexCoord;\n"
                            "    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPosition, 1.0);\n"
                            "}";

  const char* frag_shader = "#version 330 core\n"
                            "\n"
                            "in vec3 aPosition;\n"
                            "in vec3 aNormal;\n"
                            "in vec2 aTexCoord;\n"
                            "\n"
                            "out vec4 pixelColor;\n"
                            "uniform vec3 lightPos;\n"
                            "uniform sampler2D texture0;\n"
                            "uniform sampler2D texture1;\n"
                            "uniform sampler2D texture2;\n"
                            "uniform sampler2D texture3;\n"
                            "vec3 L = normalize(lightPos - aPosition);\n"
                            "float NL = max(dot(normalize(aNormal), L), 0.0);\n"
                            "vec3 color = vec3(0.31, 0.77, 0.97);\n"
                            "vec3 col   = clamp(color* 0.2 + color * 0.8 * NL, 0.0, 1.0f);"
                            "void main()\n"
                            "{\n"
                            "    pixelColor = vec4(col, 1.0); //texture2D(texture0, aTexCoord);\n"
                            "}";

  m_programs.insert(0,new OglProgram);
  m_programs[0]->create();
  //m_programs[0]->addShaderFromSourceFile(OglShader::Vertex, ":/Shaders/SimplePosition");
  m_programs[0]->addShaderFromSourceCode(OglShader::Vertex, vert_shader);
  //m_programs[0]->addShaderFromSourceFile(OglShader::Fragment, ":/Shaders/SimpleColor");
  m_programs[0]->addShaderFromSourceCode(OglShader::Fragment, frag_shader);
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

  m_vaos.insert(0,new OglVAO);
  m_vaos[0]->create();
  m_vaos[0]->bind();

  m_fns->glEnable(GL_DEPTH_TEST);
  m_fns->glDepthFunc(GL_LESS);
  m_fns->glEnable(GL_CULL_FACE);
  m_fns->glCullFace(GL_BACK);
  m_fns->glFrontFace(GL_CCW);
  m_fns->glViewport(0, 0, m_surface->size().width(), m_surface->size().height());
  m_fns->glClearColor(0.2, 0.2, 0.2, 1.0);
  m_vaos[0]->release();

  m_physics_debug_draw_vbo_idx = genBufferObject(OglBuffer::VertexBuffer);
  m_vbos[m_physics_debug_draw_vbo_idx]->bind();
  m_vbos[m_physics_debug_draw_vbo_idx]->setUsagePattern(OglBuffer::DynamicDraw);
  m_physics_debug_draw_vbo_idx = genVertexArrayObject();

  m_surface->doneCurrent();
}

void RenderSystem::_render_scene(Scene* scene)
{
  m_surface->makeCurrent();

  if (scene) {

    auto* camera = scene->mainCamera()->getComponent<PerspectiveCamera>();

    for (GameObject* gameObject : scene->gameObjects()) {
      if (gameObject->isVisible()) {
        if (gameObject->hasComponent<MeshRenderer>()) {
          if (!m_render_graph.contains(gameObject)) {
            auto* meshRenderer = gameObject->getComponent<MeshRenderer>();
            auto* mesh = m_engine->assetManager()->getMesh(meshRenderer->meshHandle());

            //int vboIdx = m_vbos.size(), iboIdx = m_ibos.size();
            int vbo_idx = genBufferObject(OglBuffer::Type::VertexBuffer);
            int ibo_idx = genBufferObject(OglBuffer::Type::IndexBuffer);
            //m_render_graph.insert(gameObject, {vboIdx, {iboIdx, mesh->indices().size()}, {0}});
            m_render_graph.insert(gameObject, {vbo_idx, {ibo_idx, mesh->indices().size()}, {0}});

            //m_vbos.insert(vboIdx, new OglBuffer(OglBuffer::VertexBuffer));
            //m_ibos.insert(iboIdx, new OglBuffer(OglBuffer::IndexBuffer));

            //m_vbos[vboIdx]->create();
            //m_vbos[vboIdx]->bind();
            //m_vbos[vboIdx]->setUsagePattern(OglBuffer::StaticDraw);
            //m_vbos[vboIdx]->allocate(mesh->constData(), mesh->count() * sizeof(float));
            m_vbos[vbo_idx]->bind();
            m_vbos[vbo_idx]->setUsagePattern(OglBuffer::StaticDraw);
            m_vbos[vbo_idx]->allocate(mesh->constData(), mesh->count() * sizeof(float));

            //m_ibos[iboIdx]->create();
            //m_ibos[iboIdx]->bind();
            //m_ibos[iboIdx]->setUsagePattern(OglBuffer::StaticDraw);
            //m_ibos[iboIdx]->allocate(mesh->indices().constData(),mesh->vertexCount() * sizeof(UInt32));
            //m_ibos[ibo_idx]->create();
            m_ibos[ibo_idx]->bind();
            m_ibos[ibo_idx]->setUsagePattern(OglBuffer::StaticDraw);
            m_ibos[ibo_idx]->allocate(mesh->indices().constData(),mesh->vertexCount() * sizeof(UInt32));
          }
        }
      }
    }

    m_vaos[0]->bind();
    m_programs[0]->bind();
    m_programs[0]->setUniformValue("projectionMatrix", camera->projectionMatrix());
    m_programs[0]->setUniformValue("viewMatrix", camera->viewMatrix());

    m_fns->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (auto i = m_render_graph.begin(); i != m_render_graph.end(); ++i) {
      _render(i.key(), i.value(), m_programs[0]);
    }

    m_programs[0]->release();
    m_vaos[0]->release();

    if (m_is_physics_debug_draw_enabled) {
      m_vaos[m_physics_debug_draw_vao_idx]->bind();

      m_programs[0]->bind();
      m_fns->glDrawArrays(GL_LINES, 0, );
      m_programs[0]->release();
      m_vaos[m_physics_debug_draw_vbo_idx]->release();
    }
  }

  m_surface->swapBuffers();

  m_surface->doneCurrent();
}

void RenderSystem::_render(const GameObject* gameObject, const RenderInfo& info, OglProgram* program)
{
  auto* transform = gameObject->transform();
  program->setUniformValue("modelMatrix", transform->worldMatrix());
  program->setUniformValue("normalMatrix", transform->worldMatrix().normalMatrix());

  for (auto i : info.texIds) {
      m_textures[i]->bind();
  }

  m_surface->framebufferObject()->bind();
  m_vbos[info.vboIdx]->bind();
  m_fns->glEnableVertexAttribArray(0);
  m_fns->glEnableVertexAttribArray(1);
  m_fns->glEnableVertexAttribArray(3);
  m_fns->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
  m_fns->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3*sizeof(float)));
  m_fns->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6*sizeof(float)));
  m_ibos[info.ibo.idx]->bind();
  m_fns->glDrawElements(GL_TRIANGLES, info.ibo.size, GL_UNSIGNED_INT, (void*)nullptr);
  m_vbos[info.vboIdx]->release();
  m_ibos[info.vboIdx]->release();
  m_surface->framebufferObject()->release();
}

QImage RenderSystem::grabFramebuffer() const
{
  return m_surface->grabFramebuffer();
}

void RenderSystem::resize(const QSize& size)
{
  m_surface->resize(size);
}

void RenderSystem::update(Real dt)
{
  _render_scene(m_engine->sceneManager()->sceneAt(m_engine->sceneManager()->activatedScene()));
}

Int32 RenderSystem::genBufferObject(OglBuffer::Type type)
{
  switch (type) {
    case OglBuffer::Type::IndexBuffer: {
      Int32 idx = m_ibos.size();
      m_ibos.insert(idx, new OglBuffer(OglBuffer::IndexBuffer));
      m_ibos[idx]->create();
      if (!m_ibos[idx]->isCreated()) {
        m_ibos.remove(idx);
        return -1;
      } else {
        return idx;
      }
    }

    case OglBuffer::Type::VertexBuffer: {
      Int32 idx = m_vbos.size();
      m_vbos.insert(idx, new OglBuffer(OglBuffer::VertexBuffer));
      m_vbos[idx]->create();
      if (!m_vbos[idx]->isCreated()) {
        m_vbos.remove(idx);
        return -1;
      } else {
        return idx;
      }
    }

    default:
      return -1;
  }
}

Int32 RenderSystem::genVertexArrayObject()
{
  Int32 idx = m_vaos.size();
  m_vaos.insert(idx, new OglVAO());
  m_vaos[idx]->create();
  return (m_vaos[idx]->isCreated()) ? idx : -1;
}

Int32 RenderSystem::physicsDebugDrawVAOIdx() const
{
  return m_physics_debug_draw_vao_idx;
}

Int32 RenderSystem::physicsDebugDrawVBOIdx() const
{
  return m_physics_debug_draw_vbo_idx;
}

OglBuffer* RenderSystem::vboAt(Int32 idx)
{
  if (idx < 0)
    return nullptr;

  return m_vbos[idx];
}

OglVAO* RenderSystem::vaoAt(Int32 idx)
{
  if (idx < 0)
    return nullptr;

  return m_vaos[idx];
}

OglBuffer* RenderSystem::iboAt(Int32 idx)
{
  if (idx < 0)
    return nullptr;

  return m_ibos[idx];
}
