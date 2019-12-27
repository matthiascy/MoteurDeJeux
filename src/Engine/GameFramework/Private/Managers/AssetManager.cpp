#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <Graphics/Public/Mesh.hpp>
#include <Graphics/Public/Model.hpp>
#include <Graphics/Public/Material.hpp>
#include <GameFramework/Public/Managers/AssetManager.hpp>
#include <Graphics/Public/Texture.hpp>
#include <QtCore/QDir>

AssetManager::AssetManager(const String& name, Engine* engine, Object* parent)
  : Object(name, parent), m_engine{engine}
{
  m_meshes = makeUnique<Array<Mesh*>>();
  m_models = makeUnique<Array<Model*>>();
  m_textures = makeUnique<Array<Texture*>>();
  m_materials = makeUnique<Array<Material*>>();
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

  for (auto* i : *m_textures) {
    delete i;
  }
  m_textures.reset(nullptr);

  qDebug() << "Shutting down...[Finished]";
}

ModelHandle AssetManager::loadModel(const String& path)
{
  auto model_idx = m_models->size();
  m_models->push_back(_load_model(path));
  return {model_idx};
}

TextureHandle AssetManager::loadTexture(const String & path)
{
  auto idx = m_meshes->size();
  //m_textures->push_back(new OglTexture(Image(path).mirrored()));
  // TODO: decoupling
  m_engine->renderSystem()->makeCurrent();
  auto* texture = new Texture(path);
  m_textures->push_back(texture);
  m_engine->renderSystem()->doneCurrent();
  return {idx};
}

Texture* AssetManager::getTexture(TextureHandle handle)
{
  if (handle.idx < m_textures->size())
    return m_textures->at(handle.idx);
  else
    return nullptr;
}

Model* AssetManager::getModel(ModelHandle handle)
{
  if (handle.idx < m_models->size())
    return m_models->at(handle.idx);
  else
    return nullptr;
}

Material* AssetManager::getMaterial(MaterialHandle handle)
{
  if (handle.idx < m_materials->size())
    return m_materials->at(handle.idx);
  else
    return nullptr;
}

Mesh* AssetManager::getMesh(MeshHandle handle) {
  if (handle.idx < m_meshes->size()) {
    return m_meshes->at(handle.idx);
  } else
    return nullptr;
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
      m_meshes->push_back(_load_mesh(assimpMesh, scene, path));
      model->addMesh({m_meshes->size() - 1});
    }

    return model;

  } else {

    qWarning() << "Cannot parse file: " + path;
    return nullptr;

  }
}

Mesh* AssetManager::_load_mesh(const aiMesh* assimpMesh, const aiScene* scene, const String& path)
{
  Array<UInt32> indices;
  Array<VertexLayoutPNTTB> vertices;

  const aiVector3D zero(0.0f, 0.0f, 0.0f);
  for (auto i = 0; i < assimpMesh->mNumVertices; ++i) {
    const aiVector3D& p = assimpMesh->mVertices[i];
    const aiVector3D& n = assimpMesh->mNormals[i];
    const aiVector3D& tex = assimpMesh->HasTextureCoords(0) ? assimpMesh->mTextureCoords[0][i] : zero;
    bool hasTangentBiTangent = assimpMesh->HasTangentsAndBitangents();
    const aiVector3D& tan = hasTangentBiTangent ? assimpMesh->mTangents[i] : zero;
    const aiVector3D& bt = hasTangentBiTangent ? assimpMesh->mBitangents[i] : zero;

    vertices.push_back({{p.x, p.y, p.z}, {n.x, n.y, n.z}, {tex.x, tex.y}, {tan.x, tan.y, tan.z}, {bt.x, bt.y, bt.z}});
  }

  for (unsigned i = 0; i < assimpMesh->mNumFaces; ++i) {
    const aiFace& face = assimpMesh->mFaces[i];
    indices.push_back(face.mIndices[0]);
    indices.push_back(face.mIndices[1]);
    indices.push_back(face.mIndices[2]);
  }

  auto* mesh = new Mesh(assimpMesh->mNumVertices, vertices, indices);

  //qDebug() << assimpMesh->mMaterialIndex;
  m_materials->push_back(_load_material(scene->mMaterials[assimpMesh->mMaterialIndex], path));

  mesh->setMaterial({m_materials->size() - 1});

  return mesh;
}

Material* AssetManager::_load_material(const aiMaterial* assimpMaterial, const String& path)
{
  auto* material = new Material();

  if (assimpMaterial) {

    aiColor4D color;
    Real value;
    aiString materialName;

    // Lighting parameters :
    if (AI_SUCCESS == assimpMaterial->Get(AI_MATKEY_NAME, materialName)) {
      //qDebug() << "Diffuse color " << color.a << " " << color.b << " " << color.r;
      //qDebug() << materialName.C_Str();
    }

    // Lighting maps : textures;
    for (const auto& type : EnumRange<ETextureType, ETextureType::Diffuse, ETextureType::Reflection>()) {
      Array<TextureHandle> textures = _load_material_textures(assimpMaterial, type, path);
      material->addTexturesOfType(type, textures);
    };

  }

  if (material->texturesOfType(ETextureType::Diffuse).isEmpty()) {
    // TODO: default value
    material->addTextureOfType(ETextureType::Diffuse, {0});
  }

  return material;
}

ETextureType fromAssimpTextureType(aiTextureType type)
{
  switch (type) {
    case aiTextureType_NONE:
      return ETextureType::None;

    case aiTextureType_DIFFUSE:
      return ETextureType::Diffuse;

    case aiTextureType_SPECULAR:
      return ETextureType::Specular;

    case aiTextureType_AMBIENT:
      return ETextureType::Ambient;

    case aiTextureType_EMISSIVE:
      return ETextureType::Emissive;

    case aiTextureType_HEIGHT:
      return ETextureType::Height;

    case aiTextureType_NORMALS:
      return ETextureType::Normals;

    case aiTextureType_SHININESS:
      return ETextureType::Shininess;

    case aiTextureType_OPACITY:
      return ETextureType::Opacity;

    case aiTextureType_DISPLACEMENT:
      return ETextureType::Displacement;

    case aiTextureType_LIGHTMAP:
      return ETextureType::LightMap;

    case aiTextureType_REFLECTION:
      return ETextureType::Reflection;

    default:
      return ETextureType::Unknown;
  }
}

aiTextureType toAssimpTextureType(ETextureType type)
{
  switch (type) {
    case ETextureType::Diffuse:
      return aiTextureType_DIFFUSE;

    case ETextureType::Specular:
      return aiTextureType_SPECULAR;

    case ETextureType::Ambient:
      return aiTextureType_AMBIENT;

    case ETextureType::Emissive:
      return aiTextureType_EMISSIVE;

    case ETextureType::Height:
      return aiTextureType_HEIGHT;

    case ETextureType::Normals:
      return aiTextureType_NORMALS;

    case ETextureType::Shininess:
      return aiTextureType_SHININESS;

    case ETextureType::Opacity:
      return aiTextureType_OPACITY;

    case ETextureType::Displacement:
      return aiTextureType_DISPLACEMENT;

    case ETextureType::LightMap:
      return aiTextureType_LIGHTMAP;

    case ETextureType::Reflection:
      return aiTextureType_REFLECTION;

    case ETextureType::None:
      return aiTextureType_NONE;

    default:
      return aiTextureType_UNKNOWN;
  }
}

Array<TextureHandle> AssetManager::_load_material_textures(const aiMaterial* assimpMaterial, ETextureType type, const String& path)
{
  m_engine->renderSystem()->makeCurrent();
  Array<TextureHandle> textures;
  aiTextureType assimpTextureType = toAssimpTextureType(type);
  for (unsigned i = 0; i < assimpMaterial->GetTextureCount(assimpTextureType); ++i) {
    aiString str;
    assimpMaterial->GetTexture(assimpTextureType, i, &str);
    String filepath = QFileInfo(path).absolutePath() + "/" + String(str.C_Str());

    // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
    bool isSkip = false;
    for (auto j = 0; j < m_textures->size(); ++j) {
      if (std::strcmp(m_textures->at(j)->path().toStdString().c_str(), str.C_Str()) == 0) {
        textures.push_back({j});
        isSkip = true;
        break;
      }
    }

    if (!isSkip) {
      m_textures->push_back(new Texture(filepath, type));
      textures.push_back({m_textures->size() - 1});
    }
  }
  m_engine->renderSystem()->doneCurrent();

  return textures;
}
