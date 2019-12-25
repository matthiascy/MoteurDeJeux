#include <GameFramework/Public/Systems/RenderSystem.hpp>
#include <QOpenGLFunctions>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>
#include <GameFramework/Public/Scene.hpp>
#include <GameFramework/Public/Engine.hpp>
#include <GameFramework/Public/Components/PerspectiveCamera.hpp>
#include <Graphics/Public/MeshRenderer.hpp>
#include <GameFramework/Public/ECS/GameObject.hpp>
#include <GameFramework/Public/Managers/SceneManager.hpp>
#include <Graphics/Public/Light.hpp>
#include <Graphics/Public/DirectionalLight.hpp>
#include <Graphics/Public/SpotLight.hpp>
#include <Graphics/Public/PointLight.hpp>


RenderSystem::RenderSystem(const String& name, Engine* engine, Object* parent)
  : System(name, engine, parent),
    m_surface{nullptr}, m_render_graph{},
    m_projection_matrix{Math::Mat4Identity},
    m_model_matrix{Math::Mat4Identity},
    m_view_matrix{Math::Mat4Identity},
    m_fns{nullptr}, m_fns4_0{nullptr},
    m_is_physics_debug_draw_enabled{true}
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
  m_fns4_0 = m_surface->fnsCore40();

  m_textures.insert(0, new OglTexture(Image(":/Textures/Checker000").mirrored()));
  m_textures[0]->setMinMagFilters(OglTexture::Filter::LinearMipMapLinear, OglTexture::Filter::LinearMipMapLinear);
  //m_textures.insert(0, new OglTexture(Image(":/Textures/moon").mirrored()));
  //m_textures.insert(2, new OglTexture(Image(":/Textures/mercury").mirrored()));
  //m_textures.insert(3, new OglTexture(Image(":/Textures/venus").mirrored()));
  //m_textures.insert(4, new OglTexture(Image(":/Textures/earth").mirrored()));

  m_programs.insert(0,new OglProgram);
  m_programs[0]->create();
  m_programs[0]->addShaderFromSourceFile(OglShader::Vertex, ":/Shaders/StandardVert");
  m_programs[0]->addShaderFromSourceFile(OglShader::Fragment, ":/Shaders/StandardFrag");
  m_programs[0]->link();
  //m_programs[0]->bind();

  //m_programs[0]->release();

  m_vaos.insert(0,new OglVAO);
  m_vaos[0]->create();

  _init_physics_system_debug_draw();

  m_fns->glEnable(GL_DEPTH_TEST);
  m_fns->glDepthFunc(GL_LESS);
  m_fns->glEnable(GL_CULL_FACE);
  m_fns->glCullFace(GL_BACK);
  m_fns->glFrontFace(GL_CCW);
  m_fns->glViewport(0, 0, m_surface->size().width(), m_surface->size().height());
  m_fns->glClearColor(0.2, 0.2, 0.2, 1.0);

  m_surface->doneCurrent();
}

void RenderSystem::_render_scene(Scene* scene)
{
  m_surface->makeCurrent();

  if (scene) {

    auto* camera = scene->mainCamera()->getComponent<PerspectiveCamera>();
    Light* light = nullptr;

    for (GameObject* gameObject : scene->gameObjects()) {
      // TODO: multiple lights
      if (gameObject->hasComponent<Light>())
        light = gameObject->getComponent<Light>();

      if (gameObject->isVisible()) {
        if (gameObject->hasComponent<MeshRenderer>()) {
          if (!m_render_graph.contains(gameObject)) {
            auto* meshRenderer = gameObject->getComponent<MeshRenderer>();
            auto* mesh = m_engine->assetManager()->getMesh(meshRenderer->meshHandle());

            int vbo_idx = createBufferObject(OglBuffer::Type::VertexBuffer);
            int ibo_idx = createBufferObject(OglBuffer::Type::IndexBuffer);
            m_render_graph.insert(gameObject, {vbo_idx, {ibo_idx, mesh->indices().size()}, {0}});

            m_vbos[vbo_idx]->bind();
            m_vbos[vbo_idx]->setUsagePattern(OglBuffer::StaticDraw);
            m_vbos[vbo_idx]->allocate(mesh->constData(), mesh->count() * sizeof(float));

            m_ibos[ibo_idx]->bind();
            m_ibos[ibo_idx]->setUsagePattern(OglBuffer::StaticDraw);
            m_ibos[ibo_idx]->allocate(mesh->indices().constData(),mesh->vertexCount() * sizeof(UInt32));
          }
        }
      }
    }

    m_surface->framebufferObject()->bind();

    m_vaos[0]->bind();
    m_programs[0]->bind();
    m_programs[0]->setUniformValue("projectionMatrix", camera->projectionMatrix());
    m_programs[0]->setUniformValue("viewMatrix", camera->viewMatrix());
    m_programs[0]->setUniformValue("eyePosition", camera->gameObject()->transform()->worldPosition());

    // Ambient color
    m_programs[0]->setUniformValue("lighting.ambient.color", Vec3{0.7, 0.7, 0.7});
    m_programs[0]->setUniformValue("lighting.ambient.intensity", 0.3f);
    m_programs[0]->setUniformValue("lighting.ambientCoeff", 0.5f);
    m_programs[0]->setUniformValue("lighting.diffuseCoeff", 0.5f);
    m_programs[0]->setUniformValue("lighting.specularCoeff", 1.0f);

    if (light) {
      switch (light->lightType()) {
        // Directional light
        case ELightType::Directional: {
          m_programs[0]->setUniformValue("lighting.directional.color", light->color());
          m_programs[0]->setUniformValue("lighting.directional.direction", light->gameObject()->transform()->forward());
          m_programs[0]->setUniformValue("lighting.directional.intensity", light->intensity());
        } break;

        case ELightType::Spot: {
          m_programs[0]->setUniformValue("spotLight.position", light->gameObject()->transform()->worldPosition());
          m_programs[0]->setUniformValue("spotLight.color", dynamic_cast<SpotLight*>(light)->color());
        } break;

        case ELightType::Point: {
          m_programs[0]->setUniformValue("pointLight.position", light->gameObject()->transform()->worldPosition());
          m_programs[0]->setUniformValue("pointLight.color", dynamic_cast<PointLight*>(light)->color());
        } break;

        default:
          break;
      }
    }

    m_fns->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (auto i = m_render_graph.begin(); i != m_render_graph.end(); ++i) {
      _render(i.key(), i.value(), m_programs[0]);
    }

    m_programs[0]->release();
    m_vaos[0]->release();

    if (m_is_physics_debug_draw_enabled) {
      _physics_system_debug_draw(camera);
    }

    m_surface->framebufferObject()->release();
  }
  m_surface->doneCurrent();

  m_surface->swapBuffers();
}

void RenderSystem::_render(const GameObject* gameObject, const RenderInfo& info, OglProgram* program)
{
  auto* transform = gameObject->transform();
  program->setUniformValue("modelMatrix", transform->worldMatrix());
  program->setUniformValue("normalMatrix", transform->worldMatrix().normalMatrix());

  for (auto i = 0, j = 0; i < info.texIds.size(); ++i, ++j) {
    m_textures[i]->bind(j);
  }

  m_vbos[info.vboIdx]->bind();
  m_fns->glEnableVertexAttribArray(0);
  m_fns->glEnableVertexAttribArray(1);
  m_fns->glEnableVertexAttribArray(2);
  m_fns->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
  m_fns->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3*sizeof(float)));
  m_fns->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6*sizeof(float)));
  m_ibos[info.ibo.idx]->bind();
  m_fns->glDrawElements(GL_TRIANGLES, info.ibo.size, GL_UNSIGNED_INT, (void*)nullptr);
  m_vbos[info.vboIdx]->release();
  m_ibos[info.ibo.idx]->release();
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

Int32 RenderSystem::createBufferObject(OglBuffer::Type type)
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

Int32 RenderSystem::createVertexArrayObject()
{
  Int32 idx = m_vaos.size();
  m_vaos.insert(idx, new OglVAO());
  m_vaos[idx]->create();
  return (m_vaos[idx]->isCreated()) ? idx : -1;
}

Int32 RenderSystem::createShaderProgram()
{
  Int32 idx = m_programs.size();
  m_programs.insert(idx, new OglProgram());
  m_programs[idx]->create();
  return idx;
}

Int32 RenderSystem::createShader(OglShader::ShaderTypeBit type)
{
  Int32 idx = m_shaders.size();
  m_shaders.insert(idx, new OglShader(type));
  return idx;
}

const RenderSystem::physics_debug_draw_info_t& RenderSystem::physicsDebugDrawInfo() const
{
  return m_physics_debug_draw_info;
}

RenderSystem::physics_debug_draw_info_t& RenderSystem::physicsDebugDrawInfo() {
  return m_physics_debug_draw_info;
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

OglProgram* RenderSystem::programAt(Int32 idx)
{
  if (idx < 0)
    return nullptr;

  return m_programs[idx];
}

void RenderSystem::_init_physics_system_debug_draw()
{
  m_physics_debug_draw_info.vaoIdx = createVertexArrayObject();
  m_vaos[m_physics_debug_draw_info.vaoIdx]->bind();
  m_physics_debug_draw_info.vboIdx = createBufferObject(OglBuffer::VertexBuffer);
  m_vbos[m_physics_debug_draw_info.vboIdx]->bind();
  m_vbos[m_physics_debug_draw_info.vboIdx]->setUsagePattern(OglBuffer::DynamicDraw);
  m_vbos[m_physics_debug_draw_info.vboIdx]->release();
  m_vaos[m_physics_debug_draw_info.vaoIdx]->release();
  m_physics_debug_draw_info.vertShaderIdx = createShader(OglShader::Vertex);
  m_physics_debug_draw_info.fragShaderIdx = createShader(OglShader::Fragment);
  m_shaders[m_physics_debug_draw_info.vertShaderIdx]->compileSourceFile(":/Shaders/PhysicsDebugDrawVert");
  m_shaders[m_physics_debug_draw_info.fragShaderIdx]->compileSourceFile(":/Shaders/PhysicsDebugDrawFrag");
  m_physics_debug_draw_info.programIdx = createShaderProgram();
  m_programs[m_physics_debug_draw_info.programIdx]->addShader(m_shaders[m_physics_debug_draw_info.vertShaderIdx]);
  m_programs[m_physics_debug_draw_info.programIdx]->addShader(m_shaders[m_physics_debug_draw_info.fragShaderIdx]);
  m_programs[m_physics_debug_draw_info.programIdx]->link();
  if (!m_programs[m_physics_debug_draw_info.programIdx]->isLinked())
    qDebug() << "Physics system debug draw shader program link failed.";
}

void RenderSystem::makeCurrent()
{
  m_surface->makeCurrent();
}

void RenderSystem::doneCurrent()
{
  m_surface->doneCurrent();
}

void RenderSystem::_physics_system_debug_draw(Camera* camera)
{
  m_programs[m_physics_debug_draw_info.programIdx]->bind();
  m_programs[m_physics_debug_draw_info.programIdx]->setUniformValue("projectionMatrix", camera->projectionMatrix());
  m_programs[m_physics_debug_draw_info.programIdx]->setUniformValue("viewMatrix", camera->viewMatrix());
  m_programs[m_physics_debug_draw_info.programIdx]->setUniformValue("modelMatrix", Math::Mat4Identity);
  m_vaos[m_physics_debug_draw_info.vaoIdx]->bind();
  m_vbos[m_physics_debug_draw_info.vboIdx]->bind();
  m_fns->glEnableVertexAttribArray(0);
  m_fns->glEnableVertexAttribArray(1);
  m_fns->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
  m_fns->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3*sizeof(float)));
  m_vbos[m_physics_debug_draw_info.vboIdx]->release();
  m_fns->glDrawArrays(GL_LINES, 0, m_physics_debug_draw_info.size);
  m_vaos[m_physics_debug_draw_info.vaoIdx]->release();
  m_programs[m_physics_debug_draw_info.programIdx]->release();
}
