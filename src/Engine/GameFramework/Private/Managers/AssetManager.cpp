#include <GameFramework/Public/Managers/AssetManager.hpp>
#include <GameFramework/Public/Managers/MeshLoader.hpp>
#include <Graphics/Public/Mesh.hpp>

AssetManager::AssetManager(const String& name, Object* parent)
  : Object(name, parent)
{
  m_meshes = makeUnique<Array<Mesh*>>();
  qInfo() << "Creation =>" << objectName();
}

AssetManager::~AssetManager()
{
  qDebug() << "Shutting down...";
  for (auto i : *m_meshes) {
    delete i;
  }
  /*
  for (auto& mesh : m_meshes) {
    mesh.reset(nullptr);
  }
   */

  for (auto* texture : m_textures) {
    delete texture;
  }
  qDebug() << "Shutting down...[Finished]";
}

AssetHandle AssetManager::loadMesh(const String& path)
{
  auto idx = static_cast<UInt32>(m_meshes->size());
  m_meshes->push_back(MeshLoader::loadMesh(path));
  return {idx, EAssetType::Mesh};
}

AssetHandle AssetManager::loadTexture(const String & path)
{
  auto idx = static_cast<UInt32>(m_meshes->size());
  m_textures.push_back(new OglTexture(Image(path).mirrored()));
  return {idx, EAssetType::Texture};
}

OglTexture* AssetManager::getTexture(AssetHandle handle)
{
  return m_textures[handle.idx];
}

Mesh* AssetManager::getMesh(AssetHandle handle)
{
  return m_meshes->operator[](handle.idx);
}

AssetHandle AssetManager::loadModel(const String& path)
{
  // TODO:
}
