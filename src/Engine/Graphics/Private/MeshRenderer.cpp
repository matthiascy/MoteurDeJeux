#include <Graphics/Public/MeshRenderer.hpp>
#include <Graphics/Public/RenderSystem.hpp>
#include <Graphics/Public/Model.hpp>
#include <GameFramework/Public/ECS/GameObject.hpp>
#include <Graphics/Public/Material.hpp>
#include <Graphics/Public/Texture.hpp>

MeshRenderer::MeshRenderer(const String& name, GameObject* gameObject, const ModelHandle& handle)
  : Renderer(name, gameObject), m_model{handle}
{ }

UInt64 MeshRenderer::typeID() const
{
  return family::type<MeshRenderer>;
}

ModelHandle MeshRenderer::modelHandle() const
{
  return m_model;
}

void MeshRenderer::draw(RenderSystem* renderSystem, OglProgram* program, AssetManager* assetManager, Real dt)
{
  if (!m_is_initialized)
    init(renderSystem, assetManager);

  for (auto i = 0; i < m_meshes_to_rendered.size(); ++i) {
    auto* material = assetManager->getMaterial(m_meshes_to_rendered[i]->material());
    program->setUniformValue("material.ambient", material->ambient());
    program->setUniformValue("material.diffuse", material->diffuse());
    program->setUniformValue("material.specular", material->specular());
    program->setUniformValue("material.shininess", material->shininess());

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
    renderSystem->fns()->glEnableVertexAttribArray(0);
    renderSystem->fns()->glEnableVertexAttribArray(1);
    renderSystem->fns()->glEnableVertexAttribArray(2);
    renderSystem->fns()->glEnableVertexAttribArray(3);
    renderSystem->fns()->glEnableVertexAttribArray(4);
    renderSystem->fns()->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), nullptr);
    renderSystem->fns()->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float),reinterpret_cast<void*>(3 * sizeof(float)));
    renderSystem->fns()->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float),reinterpret_cast<void*>(6 * sizeof(float)));
    renderSystem->fns()->glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float),reinterpret_cast<void*>(8 * sizeof(float)));
    renderSystem->fns()->glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float),reinterpret_cast<void*>(11 * sizeof(float)));
    renderSystem->iboAt(m_ibos[i].idx)->bind();
    renderSystem->fns()->glDrawElements(GL_TRIANGLES, m_meshes_to_rendered[i]->indices().size(), GL_UNSIGNED_INT, (void*)nullptr);
    renderSystem->vboAt(m_vbos[i].idx)->release();
    renderSystem->iboAt(m_ibos[i].idx)->release();

    for (auto* texture : boundTextures)
      texture->release();
  }
}

void MeshRenderer::init(RenderSystem* renderSystem, AssetManager* assetManager)
{
  auto* model = assetManager->getModel(m_model);
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
          vbo->allocate(&mesh->vertices()[0], mesh->dataCount() * sizeof(float));
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
