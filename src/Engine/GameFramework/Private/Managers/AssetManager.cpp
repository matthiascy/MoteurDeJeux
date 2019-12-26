#include <GameFramework/Public/Managers/AssetManager.hpp>
#include <Graphics/Public/Mesh.hpp>
#include <Graphics/Public/Model.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

AssetManager::AssetManager(const String& name, Object* parent)
  : Object(name, parent)
{
  m_meshes = makeUnique<Array<Mesh*>>();
  m_models = makeUnique<Array<Model*>>();
  qInfo() << "Creation =>" << objectName();
}

AssetManager::~AssetManager()
{
  qDebug() << "Shutting down...";
  for (auto i : *m_meshes) {
    delete i;
  }

  m_meshes.reset(nullptr);
  m_models.reset(nullptr);

  for (auto* texture : m_textures) {
    delete texture;
  }

  qDebug() << "Shutting down...[Finished]";
}

ModelHandle AssetManager::loadModel(const String& path)
{
  auto mesh_idx = m_meshes->size();
  auto model_idx = m_models->size();
  m_meshes->push_back(_load_mesh(path));
  m_models->push_back(new Model());
  m_models->at(model_idx)->addMesh({mesh_idx});
  return {model_idx};
}

TextureHandle AssetManager::loadTexture(const String & path)
{
  auto idx = m_meshes->size();
  m_textures.push_back(new OglTexture(Image(path).mirrored()));
  return {idx};
}

OglTexture* AssetManager::getTexture(TextureHandle handle)
{
  return m_textures[handle.idx];
}

Model* AssetManager::getModel(ModelHandle handle)
{
  if (handle.idx < m_models->size())
    return m_models->at(handle.idx);
}

Mesh* AssetManager::getMesh(MeshHandle handle)
{
  if (handle.idx < m_meshes->size()) {

    return m_meshes->at(handle.idx);

  }
}

Mesh* AssetManager::_load_mesh(const String& path)
{
  Assimp::Importer importer;
  Array<UInt32> indices;
  // [vertex] [normal] [tex coord]
  //  0  1  2   3  4  5   6  7
  // [x, y, z] [x, y, z] [u, v]
  Array<float> compact_data;

  const aiScene* scene = importer.ReadFile(path.toStdString(),
                                           aiProcess_Triangulate
                                           | aiProcess_CalcTangentSpace
                                           | aiProcess_SortByPType
                                           | aiProcess_GenNormals);

  if (!scene) {
    qDebug() << "loadMesh error: " << importer.GetErrorString();
    return nullptr;
  }

  const aiMesh* ai_mesh = scene->mMeshes[0];
  compact_data.resize(ai_mesh->mNumVertices * 8);

  if (ai_mesh->mNumVertices > 0) {
    for (unsigned i = 0; i < ai_mesh->mNumVertices; ++i) {
      const aiVector3D & pos = ai_mesh->mVertices[i];
      compact_data[i * 8 + 0] = pos.x;
      compact_data[i * 8 + 1] = pos.y;
      compact_data[i * 8 + 2] = pos.z;
    }
  }

  if (ai_mesh->HasNormals()) {
    for (unsigned i = 0; i < ai_mesh->mNumVertices; ++i) {
      const aiVector3D & n = ai_mesh->mNormals[i];
      compact_data[i * 8 + 3] = n.x;
      compact_data[i * 8 + 4] = n.y;
      compact_data[i * 8 + 5] = n.z;
    }
  }

  for (unsigned i = 0; i < ai_mesh->mNumVertices; ++i) {
    if (ai_mesh->HasTextureCoords(i)) {
      const aiVector3D & uvw = ai_mesh->mTextureCoords[0][i];
      compact_data[i * 8 + 6] = uvw.x;
      compact_data[i * 8 + 7] = uvw.y;
    } else {
      compact_data[i * 8 + 6] = 1.0f;
      compact_data[i * 8 + 7] = 1.0f;
    }
  }

  indices.reserve(ai_mesh->mNumFaces * 3);
  for (unsigned i = 0; i < ai_mesh->mNumFaces; ++i) {
    indices.push_back(ai_mesh->mFaces[i].mIndices[0]);
    indices.push_back(ai_mesh->mFaces[i].mIndices[1]);
    indices.push_back(ai_mesh->mFaces[i].mIndices[2]);
  }

  return new Mesh(ai_mesh->mNumVertices, compact_data, indices);
}

Model* AssetManager::_load_model(const String& path)
{
  /*
  Assimp::Importer importer;
  Array<UInt32> indices;
  const aiScene* scene = importer.ReadFile(path.toStdString(),
                                           aiProcess_Triangulate
                                           | aiProcess_CalcTangentSpace
                                           | aiProcess_SortByPType
                                           | aiProcess_GenNormals);

  if (!scene) {
    qDebug() << "loadMesh error: " << importer.GetErrorString();
    return nullptr;
  }

  const aiMesh* ai_mesh = scene->mMeshes[0];
  compact_data.resize(ai_mesh->mNumVertices * 8);

  if (ai_mesh->mNumVertices > 0) {
    for (unsigned i = 0; i < ai_mesh->mNumVertices; ++i) {
      const aiVector3D & pos = ai_mesh->mVertices[i];
      compact_data[i * 8 + 0] = pos.x;
      compact_data[i * 8 + 1] = pos.y;
      compact_data[i * 8 + 2] = pos.z;
    }
  }

  if (ai_mesh->HasNormals()) {
    for (unsigned i = 0; i < ai_mesh->mNumVertices; ++i) {
      const aiVector3D & n = ai_mesh->mNormals[i];
      compact_data[i * 8 + 3] = n.x;
      compact_data[i * 8 + 4] = n.y;
      compact_data[i * 8 + 5] = n.z;
    }
  }

  for (unsigned i = 0; i < ai_mesh->mNumVertices; ++i) {
    if (ai_mesh->HasTextureCoords(i)) {
      const aiVector3D & uvw = ai_mesh->mTextureCoords[0][i];
      compact_data[i * 8 + 6] = uvw.x;
      compact_data[i * 8 + 7] = uvw.y;
    } else {
      compact_data[i * 8 + 6] = 1.0f;
      compact_data[i * 8 + 7] = 1.0f;
    }
  }

  indices.reserve(ai_mesh->mNumFaces * 3);
  for (unsigned i = 0; i < ai_mesh->mNumFaces; ++i) {
    indices.push_back(ai_mesh->mFaces[i].mIndices[0]);
    indices.push_back(ai_mesh->mFaces[i].mIndices[1]);
    indices.push_back(ai_mesh->mFaces[i].mIndices[2]);
  }

  return new Mesh(ai_mesh->mNumVertices, compact_data, indices);
   */
}
