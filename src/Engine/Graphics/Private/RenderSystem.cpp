#include <Graphics/Public/RenderSystem.hpp>
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
#include <Graphics/Public/Model.hpp>
#include <Graphics/Public/Texture.hpp>
#include <Graphics/Public/Material.hpp>

String toString(ETextureType type) {
  switch (type) {
    case ETextureType::Diffuse:
      return "diffuse";

    case ETextureType::Specular:
      return "specular";

    case ETextureType::Ambient:
      return "ambient";

    case ETextureType::Emissive:
      return "emissive";

    case ETextureType::Height:
      return "height";

    case ETextureType::Normals:
      return "normals";

    case ETextureType::Shininess:
      return "shininess";

    case ETextureType::Opacity:
      return "opacity";

    case ETextureType::Displacement:
      return "displacement";

    case ETextureType::LightMap:
      return "lightmap";

    case ETextureType::Reflection:
      return "reflection";

    default:
      return "none";
  }
}


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

  m_programs.insert(0,new OglProgram);
  m_programs[0]->create();
  m_programs[0]->addShaderFromSourceFile(OglShader::Vertex, ":/Shaders/StandardVert");
  m_programs[0]->addShaderFromSourceFile(OglShader::Fragment, ":/Shaders/StandardFrag");
  m_programs[0]->link();

  m_vaos.insert(0,new OglVAO);
  m_vaos[0]->create();

  _init_physics_system_debug_draw();

  m_fns->glEnable(GL_DEPTH_TEST);
  m_fns->glDepthFunc(GL_LESS);
  m_fns->glEnable(GL_CULL_FACE);
  //m_fns4_0->glDisable(GL_CULL_FACE);
  m_fns->glCullFace(GL_BACK);
  m_fns->glFrontFace(GL_CCW);
  //m_fns4_0->glPolygonMode(GL_FRONT_AND_BACK, GL_POLYGON);
  m_fns->glViewport(0, 0, m_surface->size().width(), m_surface->size().height());
  //m_fns->glClearColor(0.2, 0.2, 0.2, 1.0);
  m_fns->glClearColor(0.4, 0.4, 0.4, 1.0);

  m_surface->doneCurrent();

  m_engine->assetManager()->loadTexture(":/Textures/Checker000");
}

void RenderSystem::_render_scene(Scene* scene)
{
  m_surface->makeCurrent();

  if (scene) {

    auto* camera = scene->mainCamera()->getComponent<PerspectiveCamera>();
    Array<Light*> lights;

    for (GameObject* gameObject : scene->gameObjects()) {
      // TODO: multiple lights
      if (gameObject->hasComponent<Light>())
        lights.push_back(gameObject->getComponent<Light>());

      if (gameObject->isVisible()) {
        if (gameObject->hasComponent<MeshRenderer>()) {
          if (!m_render_graph.contains(gameObject)) {
            _register_mesh_renderer(gameObject->getComponent<MeshRenderer>());
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
    m_programs[0]->setUniformValue("lighting.ambient.color", Vec3{0.5, 0.5, 0.5});
    m_programs[0]->setUniformValue("lighting.ambient.intensity", 0.6f);
    m_programs[0]->setUniformValue("lighting.ambientCoeff", 0.5f);
    m_programs[0]->setUniformValue("lighting.diffuseCoeff", 1.0f);
    m_programs[0]->setUniformValue("lighting.specularCoeff", 1.0f);

    m_programs[0]->setUniformValue("material.textures.diffuse", 0);
    m_programs[0]->setUniformValue("material.textures.specular", 1);
    m_programs[0]->setUniformValue("material.textures.ambient", 2);
    m_programs[0]->setUniformValue("material.textures.emissive", 3);
    m_programs[0]->setUniformValue("material.textures.height", 4);
    m_programs[0]->setUniformValue("material.textures.normals", 5);
    m_programs[0]->setUniformValue("material.textures.shininess", 6);
    m_programs[0]->setUniformValue("material.textures.opacity", 7);
    m_programs[0]->setUniformValue("material.textures.lightmap", 8);
    m_programs[0]->setUniformValue("material.textures.reflection", 9);


    if (!lights.isEmpty()) {
      int idx_directional = 0, idx_spot = 0, idx_point = 0;
      for (Light* light : lights) {
        switch (light->lightType()) {
          case ELightType::Directional: {
            String lightName = QStringLiteral("lighting.directionalLights[%1]").arg(idx_directional++);;
            m_programs[0]->setUniformValue((lightName + ".color").toStdString().c_str(), light->color());
            m_programs[0]->setUniformValue((lightName + ".direction").toStdString().c_str(), light->gameObject()->transform()->forward());
            m_programs[0]->setUniformValue((lightName + ".intensity").toStdString().c_str(), light->intensity());
          } break;

          case ELightType::Spot: {
            auto* ptr = dynamic_cast<SpotLight*>(light);
            String lightName = QStringLiteral("lighting.spotLights[%1]").arg(idx_spot++);;
            m_programs[0]->setUniformValue((lightName + ".color").toStdString().c_str(), light->color());
            m_programs[0]->setUniformValue((lightName + ".position").toStdString().c_str(), light->gameObject()->transform()->worldPosition());
            m_programs[0]->setUniformValue((lightName + ".direction").toStdString().c_str(), light->gameObject()->transform()->forward());
            m_programs[0]->setUniformValue((lightName + ".intensity").toStdString().c_str(), light->intensity());
            m_programs[0]->setUniformValue((lightName + ".range").toStdString().c_str(), ptr->range());
            m_programs[0]->setUniformValue((lightName + ".cutOff").toStdString().c_str(), std::cos(qDegreesToRadians(ptr->cutOffAngle())));
            m_programs[0]->setUniformValue((lightName + ".attenuation.constant").toStdString().c_str(), ptr->attenuation().constant);
            m_programs[0]->setUniformValue((lightName + ".attenuation.linear").toStdString().c_str(), ptr->attenuation().linear);
            m_programs[0]->setUniformValue((lightName + ".attenuation.exponent").toStdString().c_str(), ptr->attenuation().exponent);
          } break;

          case ELightType::Point: {
            auto* ptr = dynamic_cast<PointLight*>(light);
            String lightName = QStringLiteral("lighting.pointLights[%1]").arg(idx_point++);;
            m_programs[0]->setUniformValue((lightName + ".color").toStdString().c_str(), light->color());
            m_programs[0]->setUniformValue((lightName + ".position").toStdString().c_str(), light->gameObject()->transform()->worldPosition());
            m_programs[0]->setUniformValue((lightName + ".intensity").toStdString().c_str(), light->intensity());
            m_programs[0]->setUniformValue((lightName + ".range").toStdString().c_str(), ptr->range());
            m_programs[0]->setUniformValue((lightName + ".attenuation.constant").toStdString().c_str(), ptr->attenuation().constant);
            m_programs[0]->setUniformValue((lightName + ".attenuation.linear").toStdString().c_str(), ptr->attenuation().linear);
            m_programs[0]->setUniformValue((lightName + ".attenuation.exponent").toStdString().c_str(), ptr->attenuation().exponent);
          }
            break;

          default:
            break;
        }
      }
      m_programs[0]->setUniformValue("directionalLightsNum", idx_directional);
      m_programs[0]->setUniformValue("pointLightsNum", idx_point);
      m_programs[0]->setUniformValue("spotLightsNum", idx_spot);
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

  for (auto i = 0; i < info.meshes.size(); ++i) {
    auto* material = m_engine->assetManager()->getMaterial(info.meshes[i].materialIdx);
    program->setUniformValue("material.ambient", material->ambient());
    program->setUniformValue("material.diffuse", material->diffuse());
    program->setUniformValue("material.specular", material->specular());
    program->setUniformValue("material.shininess", material->shininess());

    Array<OglTexture*> boundTextures;
    if (!material->textures().isEmpty()) {
      program->setUniformValue("hasTexture", true);
      int textureUnit = 0;
      for (const auto& type : EnumRange<ETextureType, ETextureType::Diffuse, ETextureType::Reflection>()) {
        String uniform = QStringLiteral("material.isTextureEnabled[%1]").arg(textureUnit);
        auto textures = material->texturesOfType(type);
        if (!textures.isEmpty()) {
          program->setUniformValue(uniform.toStdString().c_str(), true);
          //qDebug() << uniform << "true";
          auto* texture = m_engine->assetManager()->getTexture(textures[0]);
          texture->oglTexture()->bind(textureUnit);
          boundTextures.push_back(texture->oglTexture());
          //qDebug() << texture->namePath() << " ===> bind ==> " << textureUnit;
        } else {
          //qDebug() << uniform << "false";
          program->setUniformValue(uniform.toStdString().c_str(), false);
        }
        textureUnit++;
      }
    }

    m_vbos[info.meshes[i].vboIdx]->bind();
    m_fns->glEnableVertexAttribArray(0);
    m_fns->glEnableVertexAttribArray(1);
    m_fns->glEnableVertexAttribArray(2);
    m_fns->glEnableVertexAttribArray(3);
    m_fns->glEnableVertexAttribArray(4);
    m_fns->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), nullptr);
    m_fns->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float),reinterpret_cast<void*>(3 * sizeof(float)));
    m_fns->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float),reinterpret_cast<void*>(6 * sizeof(float)));
    m_fns->glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float),reinterpret_cast<void*>(8 * sizeof(float)));
    m_fns->glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float),reinterpret_cast<void*>(11 * sizeof(float)));

    m_ibos[info.meshes[i].ibo.idx]->bind();
    m_fns->glDrawElements(GL_TRIANGLES, info.meshes[i].ibo.size, GL_UNSIGNED_INT, (void*) nullptr);

    for (auto* texture : boundTextures)
      texture->release();

    m_vbos[info.meshes[i].vboIdx]->release();
    m_ibos[info.meshes[i].ibo.idx]->release();
  }
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

void RenderSystem::_register_mesh_renderer(MeshRenderer* meshRenderer)
{
  RenderInfo info{};

  auto* model = m_engine->assetManager()->getModel(meshRenderer->modelHandle());
  auto meshes = model->meshes();

  info.meshes.resize(meshes.size());

  for (auto i = 0; i < info.meshes.size(); ++i){
    auto* mesh = m_engine->assetManager()->getMesh(meshes[i]);

    int vbo_idx = createBufferObject(OglBuffer::Type::VertexBuffer);
    int ibo_idx = createBufferObject(OglBuffer::Type::IndexBuffer);

    info.meshes[i].vboIdx = vbo_idx;
    info.meshes[i].ibo.idx = ibo_idx;
    info.meshes[i].ibo.size = mesh->indices().size();

    info.meshes[i].materialIdx = mesh->material();
    auto* material = m_engine->assetManager()->getMaterial(mesh->material());
    auto textures = material->textures();
    for (auto texture : textures)
      info.meshes[i].texIndices.push_back(texture);

    m_vbos[vbo_idx]->bind();
    m_vbos[vbo_idx]->setUsagePattern(OglBuffer::StaticDraw);
    m_vbos[vbo_idx]->allocate(&mesh->vertices()[0], mesh->dataCount() * sizeof(float));

    m_ibos[ibo_idx]->bind();
    m_ibos[ibo_idx]->setUsagePattern(OglBuffer::StaticDraw);
    m_ibos[ibo_idx]->allocate(mesh->indices().constData(),mesh->vertexCount() * sizeof(UInt32));

    m_ibos[ibo_idx]->release();
    m_vbos[vbo_idx]->release();
  }

  m_render_graph.insert(meshRenderer->gameObject(), info);
}
