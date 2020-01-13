#include <Graphics/Public/AnimatedMeshRenderer.hpp>
#include <GameFramework/Public/ECS/GameObject.hpp>
#include <Graphics/Public/Animator.hpp>
#include <Graphics/Public/Model.hpp>
#include <Graphics/Public/AnimatedModel.hpp>
#include <Graphics/Public/Material.hpp>
#include <Graphics/Public/Texture.hpp>

AnimatedMeshRenderer::AnimatedMeshRenderer(const String& name, GameObject* gameObject,
                                           AnimatedModelHandle handle)
  : Renderer(name, gameObject), m_model_handle{handle}, m_animator{nullptr}
{ }

UInt64 AnimatedMeshRenderer::typeID() const
{
  return family::type<AnimatedMeshRenderer>;
}

AnimatedModelHandle AnimatedMeshRenderer::modelHandle() const
{
  return m_model_handle;
}

void AnimatedMeshRenderer::init(RenderSystem* renderSystem, AssetManager* assetManager)
{
  // TODO: demands the render system to allocate rendering related resources (vbo, ibo ...)
  auto* animatedModel = assetManager->getAnimatedModel(m_model_handle);
  auto* model = assetManager->getModel(animatedModel->skin());

  if (model) {
    for (auto meshHandle : model->meshes()) {
      auto* mesh = assetManager->getMesh(meshHandle);
      if (mesh) {
        m_meshes_to_rendered.push_back(mesh);
        VboHandle vboHandle = renderSystem->createVertexBufferObject();
        IboHandle iboHandle = renderSystem->createIndexBufferObject();

        if (isValidHandle(vboHandle) && isValidHandle(iboHandle)) {
          m_vbos.push_back(vboHandle);
          m_ibos.push_back(iboHandle);

          auto* vbo = renderSystem->vboAt(vboHandle.idx);
          auto* ibo = renderSystem->iboAt(iboHandle.idx);

          vbo->bind();
          vbo->setUsagePattern(OglBuffer::StaticDraw);
          //vbo->allocate(&mesh->vertices()[0], mesh->dataCount() * sizeof(float));
          vbo->allocate(&mesh->verticesWithBoneInfo()[0], mesh->vertexCount() * sizeof(VertexLayoutPNTTB_B_W));
          vbo->release();

          ibo->bind();
          ibo->setUsagePattern(OglBuffer::StaticDraw);
          ibo->allocate(&mesh->indices()[0], mesh->vertexCount() * sizeof(UInt32));
          ibo->release();
        } else {
          qWarning() << "Failed to get valid vbo or ibo handle." << m_game_object->name();
        }
      }
    }
  }
  m_is_initialized = true;
}

void AnimatedMeshRenderer::draw(RenderSystem* renderSystem, OglProgram* program, AssetManager* assetManager, Real dt)
{
  if (!m_is_initialized)
    init(renderSystem, assetManager);

  // auto* animator = m_game_object->getComponent<Animator>();
  // animator->interpolate(dt);
  // pass bones weights
  //      bones indices
  //      bones transforms  => program
  // render
  auto* animator = m_game_object->getComponent<Animator>();
  if (animator) {
    qDebug() << gameObject()->name() << " : yes, we have animator";
    if (animator->boneTransforms().isEmpty()) {
      qDebug() << "boneTransform Nums : 0";
      program->setUniformValue("hasAnimation", false);
    } else {
      if (animator->isAnimationStopped()) {
        qDebug() << "animation stopped.";
        program->setUniformValue("hasAnimation", false);
      } else {
        qDebug() << "boneTransform Nums :" << animator->boneTransforms().size();
        program->setUniformValue("hasAnimation", true);
        program->setUniformValueArray("uBoneTransforms", animator->boneTransforms().constData(),
                                      animator->boneTransforms().size());
      }
    }
  }

  for (auto i = 0; i < m_meshes_to_rendered.size(); ++i) {
    auto* material = assetManager->getMaterial(m_meshes_to_rendered[i]->material());
    program->setUniformValue("material.ambient", material->ambient());
    program->setUniformValue("material.diffuse", material->diffuse());
    program->setUniformValue("material.specular", material->specular());
    program->setUniformValue("material.shininess", material->shininess());

    // TODO: deal with other type of texture
    // Because of that not all textures are bound in shader, so we need to
    // register texture information here and release after draw
    Array<OglTexture*> boundTextures;
    if (!material->textures().isEmpty()) {
      int textureUnit = 0;
      for (const auto& type : EnumRange<ETextureType, ETextureType::Diffuse, ETextureType::Reflection>()) {
        String uniform = QStringLiteral("material.isTextureEnabled[%1]").arg(textureUnit);
        auto textures = material->texturesOfType(type);
        if (!textures.isEmpty()) {
          program->setUniformValue(uniform.toStdString().c_str(), true);
          //qDebug() << uniform << "true";
          auto* texture = assetManager->getTexture(textures[0]);
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

    renderSystem->vboAt(m_vbos[i].idx)->bind();
    enable_vertex_attrib_array(9, renderSystem->fns());
    renderSystem->fns()->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 30 * sizeof(float), nullptr);
    renderSystem->fns()->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 30 * sizeof(float),reinterpret_cast<void*>(3 * sizeof(float)));
    renderSystem->fns()->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 30 * sizeof(float),reinterpret_cast<void*>(6 * sizeof(float)));
    renderSystem->fns()->glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 30 * sizeof(float),reinterpret_cast<void*>(8 * sizeof(float)));
    renderSystem->fns()->glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 30 * sizeof(float),reinterpret_cast<void*>(11 * sizeof(float)));
    renderSystem->fns()->glVertexAttribPointer(5, 4, GL_INT, GL_FALSE, 30 * sizeof(int),reinterpret_cast<void*>(14 * sizeof(float)));
    renderSystem->fns()->glVertexAttribPointer(6, 4, GL_INT, GL_FALSE, 30 * sizeof(int),reinterpret_cast<void*>(18 * sizeof(float)));
    renderSystem->fns()->glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 30 * sizeof(float),reinterpret_cast<void*>(22 * sizeof(float)));
    renderSystem->fns()->glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, 30 * sizeof(float),reinterpret_cast<void*>(26 * sizeof(float)));
    renderSystem->iboAt(m_ibos[i].idx)->bind();
    renderSystem->fns()->glDrawElements(GL_TRIANGLES, m_meshes_to_rendered[i]->indices().size(), GL_UNSIGNED_INT, (void*)nullptr);
    renderSystem->vboAt(m_vbos[i].idx)->release();
    renderSystem->iboAt(m_ibos[i].idx)->release();

    for (auto* texture : boundTextures)
      texture->release();
  }
}

void AnimatedMeshRenderer::setAnimator(Animator* animator)
{
  m_animator = animator;
}

Animator* AnimatedMeshRenderer::animator()
{
  return m_animator;
}
