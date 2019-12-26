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
  //m_meshes->push_back(_load_mesh(path));
  //m_models->push_back(new Model());
  //m_models->at(model_idx)->addMesh({mesh_idx});
  m_models->push_back(_load_model(path));
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
  Array<VertexLayoutPNT> compact_data;

  const aiScene* scene = importer.ReadFile(path.toStdString(),
                                           aiProcess_Triangulate
                                           | aiProcess_CalcTangentSpace
                                           | aiProcess_SortByPType
                                           | aiProcess_GenNormals);

  if (!scene) {
    qDebug() << "loadMesh error: " << importer.GetErrorString();
    return nullptr;
  }

  //return _load_assimp_mesh(scene->mMeshes[0]);

  const aiMesh* ai_mesh = scene->mMeshes[0];
  compact_data.resize(ai_mesh->mNumVertices * 8);

  if (ai_mesh->mNumVertices > 0) {
    for (auto i = 0; i < ai_mesh->mNumVertices; ++i) {
      const aiVector3D & pos = ai_mesh->mVertices[i];
      compact_data[i].position = {pos.x, pos.y, pos.z};
    }
  }

  if (ai_mesh->HasNormals()) {
    for (auto i = 0; i < ai_mesh->mNumVertices; ++i) {
      const aiVector3D & n = ai_mesh->mNormals[i];
      compact_data[i].normal = {n.x, n.y, n.z};
    }
  }

  for (auto i = 0; i < ai_mesh->mNumVertices; ++i) {
    if (ai_mesh->HasTextureCoords(i)) {
      const aiVector3D & uvw = ai_mesh->mTextureCoords[0][i];
      compact_data[i].texCoord = { uvw.x, uvw.y };
    } else {
      compact_data[i].texCoord = { 1.0f, 1.0f };
    }
  }

  for (unsigned i = 0; i < ai_mesh->mNumFaces; ++i) {
    indices.push_back(ai_mesh->mFaces[i].mIndices[0]);
    indices.push_back(ai_mesh->mFaces[i].mIndices[1]);
    indices.push_back(ai_mesh->mFaces[i].mIndices[2]);
  }

  return new Mesh(ai_mesh->mNumVertices, compact_data, indices);
}


Model* AssetManager::_load_model(const String& path)
{
  Assimp::Importer importer;

  const aiScene* scene = importer.ReadFile(path.toStdString(),
                                           aiProcess_Triangulate
                                           | aiProcess_CalcTangentSpace
                                           | aiProcess_SortByPType
                                           | aiProcess_GenNormals/*aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs*/);

  if (scene) {

    auto* model = new Model();

    Int32 meshes_num = scene->mNumMeshes;
    Int32 textures_num = scene->mNumTextures;

    for (auto i = 0; i < meshes_num; ++i) {
      const aiMesh* assimpMesh = scene->mMeshes[i];
      m_meshes->push_back(_load_assimp_mesh(assimpMesh));
      model->addMesh({m_meshes->size() - 1});
    }

    return model;

  } else {

    qWarning() << "Cannot parse file: " + path;
    return nullptr;

  }
}

Mesh* AssetManager::_load_assimp_mesh(const aiMesh* assimpMesh)
{
  Array<UInt32> indices;
  Array<VertexLayoutPNT> vertices;

  const aiVector3D zero(0.0f, 0.0f, 0.0f);
  for (auto i = 0; i < assimpMesh->mNumVertices; ++i) {
    const aiVector3D& p = assimpMesh->mVertices[i];
    const aiVector3D& n = assimpMesh->mNormals[i];
    const aiVector3D& t = assimpMesh->HasTextureCoords(0) ? assimpMesh->mTextureCoords[0][i] : zero;

    vertices.push_back({{p.x, p.y, p.z}, {n.x, n.y, n.z}, {t.x, t.y}});
  }

  for (unsigned i = 0; i < assimpMesh->mNumFaces; ++i) {
    const aiFace& face = assimpMesh->mFaces[i];
    indices.push_back(face.mIndices[0]);
    indices.push_back(face.mIndices[1]);
    indices.push_back(face.mIndices[2]);
  }

  return new Mesh(assimpMesh->mNumVertices, vertices, indices);
}
