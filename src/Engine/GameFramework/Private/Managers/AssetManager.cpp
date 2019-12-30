#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <Graphics/Public/Mesh.hpp>
#include <Graphics/Public/Model.hpp>
#include <Graphics/Public/Material.hpp>
#include <GameFramework/Public/Managers/AssetManager.hpp>
#include <Graphics/Public/Texture.hpp>
#include <Graphics/Public/AnimatedModel.hpp>
#include <QtCore/QDir>
#include <Graphics/Public/Animation.hpp>
#include <QtGui/QImageReader>

// TODO: remove replicated materials

namespace internal {

  /*
  static UInt32 addNodesToSkeleton(aiNode* node, Skeleton& skeleton)
  {
    UInt32 boneIdx = skeleton.bones.size();
    {
      Bone bone;
      bone.name = node->mName.C_Str();
    }
  }
   */

}

AssetManager::AssetManager(const String& name, Engine* engine, Object* parent)
  : Object(name, parent), m_engine{engine}
{
  qInfo() << "Creation =>" << objectName();
  m_meshes          = makeUnique<Array<Mesh*>>();
  m_models          = makeUnique<Array<Model*>>();
  m_textures        = makeUnique<Array<Texture*>>();
  m_materials       = makeUnique<Array<Material*>>();
  m_skeletons       = makeUnique<Array<Skeleton*>>();
  m_animations      = makeUnique<Array<Animation*>>();
  m_animated_models = makeUnique<Array<AnimatedModel*>>();
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
  // No animation loaded
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

/**
 * aiProcess_FixInfacingNormals will affect plants in sponza.obj
 *
 * FlipUVs causes the uv problem over helicopter.obj, sponza.obj
 */
Model* AssetManager::_load_model(const String& path)
{
  Assimp::Importer importer;

  const aiScene* scene = importer.ReadFile(path.toStdString(),
                                           aiProcess_Triangulate
                                           | aiProcess_CalcTangentSpace
                                           | aiProcess_SortByPType
                                           | aiProcess_GenSmoothNormals);

  if (scene) {

    auto* model = new Model();

    Int32 meshes_num = scene->mNumMeshes;
    Int32 textures_num = scene->mNumTextures;

    qDebug() << "Has materials ? " << scene->HasMaterials();

    for (auto i = 0; i < meshes_num; ++i) {
      const aiMesh* assimpMesh = scene->mMeshes[i];
      //m_meshes->push_back(_load_mesh(assimpMesh, scene, path));
      //model->addMesh({m_meshes->size() - 1});
      model->addMesh(_add_mesh(_load_mesh(assimpMesh, scene, path)));
    }

    return model;

  } else {

    qWarning() << "Cannot parse file: " << path << importer.GetErrorString();
    return nullptr;

  }
}

Mesh* AssetManager::_load_mesh(const aiMesh* assimpMesh, const aiScene* scene, const String& path)
{
  Array<UInt32> indices;
  Array<VertexLayoutPNTTB> vertices;
  Array<VertexBoneData> bones;

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

  //bones = _recursively_load_skeleton(assimpMesh);

  for (unsigned i = 0; i < assimpMesh->mNumFaces; ++i) {
    const aiFace& face = assimpMesh->mFaces[i];
    indices.push_back(face.mIndices[0]);
    indices.push_back(face.mIndices[1]);
    indices.push_back(face.mIndices[2]);
  }

  auto* mesh = new Mesh(assimpMesh->mNumVertices, vertices, indices);

  //qDebug() << " loading " << path << " " << scene->mNumMaterials;
  //qDebug() << scene->mMaterials[assimpMesh->mMaterialIndex]->GetName().C_Str();

  auto* material = _load_material(scene->mMaterials[assimpMesh->mMaterialIndex], path);

  if (material) {
    m_materials->push_back(material);
    mesh->setMaterial({m_materials->size() - 1});
  } else {
    // TODO:
    mesh->setMaterial({0});
    qDebug() << "Not material";
  }

  return mesh;
}

Material* AssetManager::_load_material(aiMaterial* assimpMaterial, const String& path)
{
  auto* material = new Material();

  if (std::strcmp(assimpMaterial->GetName().C_Str(), AI_DEFAULT_MATERIAL_NAME) != 0) {

    aiColor4D color;
    Real value;
    aiString materialName;

    String filepath = QFileInfo(path).absolutePath() + "/";

    // Lighting parameters :
    if (AI_SUCCESS == assimpMaterial->Get(AI_MATKEY_NAME, materialName)) {
      //qDebug() << materialName.C_Str();
      filepath += String(materialName.C_Str());
    } else {
      filepath += "Untitled";
    }

    if (AI_SUCCESS == assimpMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color)) {
      material->setAmbient({color.r, color.g, color.b});
    } else {
      material->setAmbient({1.0f, 1.0f, 1.0f});
    }

    if (AI_SUCCESS == assimpMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color)) {
      material->setDiffuse({color.r, color.g, color.b});
      material->setBaseColor({color.r, color.g, color.b});
    }

    if (AI_SUCCESS == assimpMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color)) {
      material->setSpecular({color.r, color.g, color.b});
    }

    if (AI_SUCCESS == assimpMaterial->Get(AI_MATKEY_SHININESS, value)) {
      material->setShininess(value);
    } else {
      material->setShininess(0.001f);
    }

    // Lighting maps : textures;
    for (const auto& type : EnumRange<ETextureType, ETextureType::Diffuse, ETextureType::Reflection>()) {
      Array<TextureHandle> textures = _load_material_textures(assimpMaterial, type, path);
      material->addTexturesOfType(type, textures);
    };
    return material;
  } else {
    return nullptr;
  }

  /*
  if (material->texturesOfType(ETextureType::Diffuse).isEmpty()) {
    // TODO: default value
    material->addTextureOfType(ETextureType::Diffuse, {0});
  }
   */
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
  QImageReader reader;
  //reader.setAutoDetectImageFormat(true);
  reader.setDecideFormatFromContent(true);

  for (unsigned i = 0; i < assimpMaterial->GetTextureCount(assimpTextureType); ++i) {
    aiString str;
    assimpMaterial->GetTexture(assimpTextureType, i, &str);
    String filepath = QFileInfo(path).absolutePath() + "/" + String(str.C_Str());

    // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
    bool isSkip = false;
    for (auto j = 0; j < m_textures->size(); ++j) {
      if (m_textures->at(j)->namePath() == filepath) {
        textures.push_back({j});
        isSkip = true;
        break;
      }
    }

    if (!isSkip) {
      auto* image = new Image;
      qDebug() << "Try to load image : " << filepath;
      reader.setFileName(filepath);
      if (reader.read(image)) {
        textures.push_back(_add_texture(new Texture(filepath, image, type)));
      } else {
        textures.push_back({0});
      }
    }
  }
  m_engine->renderSystem()->doneCurrent();

  return textures;
}

AnimatedModelHandle AssetManager::loadAnimatedModel(const String& path)
{
  auto* model = _load_animated_model(path);
  if (model) {
    return _add_animated_model(model);
  } else {
    qWarning() << "Failed to load animated model" << path;
    return { -1 };
  }
}

AnimatedModel* AssetManager::getAnimatedModel(AnimatedModelHandle handle)
{
  if (isValidHandle(handle) && handle.idx < m_animated_models->size())
    return m_animated_models->at(handle.idx);
  else
    return nullptr;
}

AnimatedModel* AssetManager::_load_animated_model(const String& path)
{
  qDebug() << "Load animated model " << path;
  Assimp::Importer importer;

  const aiScene* aScene = importer.ReadFile(path.toStdString(),
                                           aiProcess_Triangulate
                                           | aiProcess_CalcTangentSpace
                                           | aiProcess_SortByPType
                                           | aiProcess_GenSmoothNormals);
  if (aScene) {

    auto* model = new Model();
    auto* animatedModel = new AnimatedModel();

    Int32 meshes_num = aScene->mNumMeshes;
    Int32 textures_num = aScene->mNumTextures;

    //qDebug() << "Has materials ? " << aScene->HasMaterials();

    // TODO: recursively load model
    for (auto i = 0; i < meshes_num; ++i) {
      const aiMesh* assimpMesh = aScene->mMeshes[i];
      model->addMesh(_add_mesh(_load_mesh(assimpMesh, aScene, path)));
    }

    animatedModel->setSkin(_add_model(model));

    auto* skeleton = new Skeleton();

    _recursively_load_skeleton(aScene->mRootNode, skeleton);
    _load_bones(aScene, skeleton);

    animatedModel->setSkeleton(_add_skeleton(skeleton));

    for (int i = 0; i < aScene->mNumAnimations; ++i) {
      AnimationHandle animHandle = _add_animation(_load_animation(aScene->mAnimations[i]));
      animatedModel->addAnimation(animHandle);
    }

    animatedModel->setWorldInverseTransform(Math::fromAiMat4(aScene->mRootNode->mTransformation).inverted());

    return animatedModel;

  } else {

    qWarning() << "Cannot parse file: " << path << importer.GetErrorString();
    return nullptr;

  }
}

Int32 AssetManager::_recursively_load_skeleton(const aiNode* aNode, Skeleton* skeleton)
{
  Int32 boneId = skeleton->bones().size();

  {
    auto* bone = new Bone();
    bone->name = aNode->mName.C_Str();
    bone->nodeTransform = Math::fromAiMat4(aNode->mTransformation);
    for (auto i = 0; i < aNode->mNumChildren; ++i){
      bone->children.push_back(-1);
    }
    skeleton->bones().push_back(bone);
  }

  for (auto i = 0; i < aNode->mNumChildren; ++i) {
    Int32 childBoneId = _recursively_load_skeleton(aNode->mChildren[i], skeleton);
    skeleton->bones().at(boneId)->children[i] = childBoneId;
  }

  return boneId;
}

void AssetManager::_load_bones(const aiScene* aScene, Skeleton* skeleton)
{
  for (auto i = 0; i < aScene->mNumMeshes; ++i) {
    auto* aMesh = aScene->mMeshes[i];

    for (UInt32 j = 0; j < aMesh->mNumBones; ++j) {
      auto* aBone = aMesh->mBones[j];

      auto* bone = skeleton->bone(aBone->mName.C_Str());

      bone->offSetMatrix = Math::fromAiMat4(aBone->mOffsetMatrix);

      for (int k = 0; k < aBone->mNumWeights; ++k) {
        bone->weights.append({aBone->mWeights[k].mVertexId, aBone->mWeights[k].mWeight});
      }
    }
  }
}

Animation* AssetManager::_load_animation(const aiAnimation* aAnimation)
{
  auto* animation = new Animation();
  animation->name = aAnimation->mName.C_Str();

  for (auto i = 0; i < aAnimation->mNumChannels; ++i) {
    aiNodeAnim* nodeAnim = aAnimation->mChannels[i];

    NodeAnimation boneAnimation;
    boneAnimation.name = nodeAnim->mNodeName.C_Str();

    for (auto j = 0; j < nodeAnim->mNumRotationKeys; ++j) {
      AnimKeyFrame<Vec3> key;
      key.time = (Real)nodeAnim->mPositionKeys[i].mTime;
      key.value = Math::fromAiVec3(nodeAnim->mPositionKeys[i].mValue);
      boneAnimation.positionKeys.push_back(key);
    }

    for (auto j = 0; j < nodeAnim->mNumRotationKeys; ++j) {
      AnimKeyFrame<Quat> key;
      key.time = (Real)nodeAnim->mPositionKeys[i].mTime;
      key.value = Math::fromAiQuat(nodeAnim->mRotationKeys[i].mValue);
      boneAnimation.rotationKeys.push_back(key);
    }

    for (auto j = 0; j < nodeAnim->mNumRotationKeys; ++j) {
      AnimKeyFrame<Vec3> key;
      key.time = (Real)nodeAnim->mScalingKeys[i].mTime;
      key.value = Math::fromAiVec3(nodeAnim->mScalingKeys[i].mValue);
      boneAnimation.scalingKeys.push_back(key);
    }

    animation->nodeAnimations.push_back(boneAnimation);
  }

  animation->ticksPerSecond = (Real)aAnimation[0].mTicksPerSecond;
  animation->duration       = (Real)aAnimation[0].mDuration;

  return animation;
}

ModelHandle AssetManager::_add_model(Model* model)
{
  m_models->push_back(model);
  return { m_models->size() - 1 };
}

MeshHandle AssetManager::_add_mesh(Mesh* mesh)
{
  m_meshes->push_back(mesh);
  return { m_meshes->size() - 1 };
}

AnimatedModelHandle AssetManager::_add_animated_model(AnimatedModel* animatedModel)
{
  m_animated_models->push_back(animatedModel);
  return { m_animated_models->size() - 1 };
}

TextureHandle AssetManager::_add_texture(Texture* texture)
{
  m_textures->push_back(texture);
  return { m_textures->size() - 1 };
}

SkeletonHandle AssetManager::_add_skeleton(Skeleton* skeleton)
{
  m_skeletons->push_back(skeleton);
  return { m_skeletons->size() - 1 };
}

MaterialHandle AssetManager::_add_material(Material* material)
{
  m_materials->push_back(material);
  return { m_materials->size() - 1 };
}

AnimationHandle AssetManager::_add_animation(Animation* animation)
{
  m_animations->push_back(animation);
  return { m_animations->size() - 1 };
}

Animation* AssetManager::getAnimation(AnimationHandle handle)
{
  if (isValidHandle(handle) && handle.idx < m_animations->size()) {
    return m_animations->at(handle.idx);
  } else {
    return nullptr;
  }
}

Skeleton* AssetManager::getSkeleton(SkeletonHandle handle)
{
  if (isValidHandle(handle) && handle.idx < m_skeletons->size()) {
    return m_skeletons->at(handle.idx);
  } else {
    return nullptr;
  }
}

/*
const aiNodeAnim* AssetManager::_find_anim_node(const aiAnimation* anim, const String& nodeName) {
  return nullptr;
}

void AssetManager::_read_node_hierarchy(Real animTime, const aiNode* node, const Mat4& parentTransform) {

}

UInt32 AnimationLoader::findScaling(Real animTime, const aiNodeAnim* nodeAnim)
{
  assert(nodeAnim->mNumScalingKeys > 0);

  for (UInt32 i = 0; i < nodeAnim->mNumScalingKeys - 1; ++i) {
    if (animTime < (Real)nodeAnim->mScalingKeys[i + 1].mTime) {
      return i;
    }
  }
  return 0;
}

UInt32 AnimationLoader::findRotation(Real animTime, const aiNodeAnim* nodeAnim)
{
  assert(nodeAnim->mNumRotationKeys > 0);

  for (UInt32 i = 0; i < nodeAnim->mNumRotationKeys - 1; ++i) {
    if (animTime < (Real)nodeAnim->mRotationKeys[i + 1].mTime) {
      return i;
    }
  }
  return 0;
}

UInt32 AnimationLoader::findPosition(Real animTime, const aiNodeAnim* nodeAnim)
{
  for (UInt32 i = 0; i < nodeAnim->mNumPositionKeys - 1; ++i) {
    if (animTime < (Real)nodeAnim->mPositionKeys[i + 1].mTime) {
      return i;
    }
  }
  return 0;
}

Vec3 AnimationLoader::calcInterpolatedScaling(Real animTime, const aiNodeAnim* nodeAnim) {
  if (nodeAnim->mNumScalingKeys == 1) {
    return Math::fromAiVec3(nodeAnim->mScalingKeys[0].mValue);
  }

  UInt32 currKey = findScaling(animTime, nodeAnim);
  UInt32 nextKey = currKey + 1;
  assert(nextKey < nodeAnim->mNumScalingKeys);

  Real deltaTime = (Real)(nodeAnim->mScalingKeys[nextKey].mTime - nodeAnim->mScalingKeys[currKey].mTime);
  Real factor = (animTime - (Real)nodeAnim->mScalingKeys[currKey].mTime) / deltaTime;
  assert(factor >= 0.0f && factor <= 1.0f);

  const aiVector3D& start = nodeAnim->mScalingKeys[currKey].mValue;
  const aiVector3D& end   = nodeAnim->mScalingKeys[nextKey].mValue;
  aiVector3D delta = end - start;
  return Math::fromAiVec3(start + factor * delta);
}

Quat AnimationLoader::calcInterpolatedRotation(Real animTime, const aiNodeAnim* nodeAnim)
{
  if (nodeAnim->mNumRotationKeys == 1)
    return Math::fromAiQuat(nodeAnim->mRotationKeys[0].mValue);

  UInt32 currKey = findRotation(animTime, nodeAnim);
  UInt32 nextKey = currKey + 1;
  assert(nextKey < nodeAnim->mNumRotationKeys);

  Real deltaTime = (Real)(nodeAnim->mRotationKeys[nextKey].mTime - nodeAnim->mRotationKeys[currKey].mTime);
  Real factor = (animTime - (Real)nodeAnim->mRotationKeys[currKey].mTime) / deltaTime;
  assert(factor >= 0.0f && factor <= 1.0f);

  const aiQuaternion& start = nodeAnim->mRotationKeys[currKey].mValue;
  const aiQuaternion& end   = nodeAnim->mRotationKeys[nextKey].mValue;

  aiQuaternion out;

  aiQuaternion::Interpolate(out, start, end, factor);

  return Math::fromAiQuat(out.Normalize());
}

Vec3 AnimationLoader::calcInterpolatedPosition(Real animTime, const aiNodeAnim* nodeAnim)
{
  if (nodeAnim->mNumPositionKeys == 1) {
    return Math::fromAiVec3(nodeAnim->mPositionKeys[0].mValue);
  }

  UInt32 currKey = findPosition(animTime, nodeAnim);
  UInt32 nextKey = currKey + 1;

  assert(nextKey < nodeAnim->mNumPositionKeys);

  Real deltaTime = (float)(nodeAnim->mPositionKeys[nextKey].mTime - nodeAnim->mPositionKeys[currKey].mTime);
  Real factor = (animTime - (float)nodeAnim->mPositionKeys[currKey].mTime) / deltaTime;

  assert(factor >= 0.0f && factor <= 1.0f);

  const aiVector3D& start = nodeAnim->mPositionKeys[currKey].mValue;
  const aiVector3D& end   = nodeAnim->mPositionKeys[nextKey].mValue;
  aiVector3D delta = end - start;
  return Math::fromAiVec3(start + factor * delta);
}

const aiNodeAnim* AnimationLoader::findAnimNode(const aiAnimation* anim, const String& nodeName)
{
  for (UInt32 i = 0; i < anim->mNumChannels; ++i) {
    const aiNodeAnim* nodeAnim = anim->mChannels[i];
    if (String(nodeAnim->mNodeName.data) == nodeName) {
      return nodeAnim;
    }
  }

  return nullptr;
}

void AnimationLoader::readNodeHierarchy(Real animTime, const aiScene* scene, const aiNode* node, const Mat4& parentTransform)
{
  String nodeName(node->mName.data);

  const aiAnimation* animation = scene->mAnimations[0];

  Mat4 localTransformation(Math::fromAiMat4(node->mTransformation));

  const aiNodeAnim* nodeAnim = findAnimNode(animation, nodeName);

  if (nodeAnim) {
    // Interpolate scaling
    Vec3 scaling = calcInterpolatedScaling(animTime, nodeAnim);
    // Interpolate rotation
    Quat rotation = calcInterpolatedRotation(animTime, nodeAnim);
    // Interpolate translation
    Vec3 translation = calcInterpolatedPosition(animTime, nodeAnim);

    localTransformation.setToIdentity();
    localTransformation.scale(scaling);
    localTransformation.rotate(rotation);
    localTransformation.translate(translation);
  }

  Mat4 worldTransformation = parentTransform * localTransformation;

  if (m_)
}
 */
AnimatedModel* AnimatedModelLoader::load(const String& path)
{

}

// TODO: implementation
Model* ModelLoader::load(const String& path)
{
  return nullptr;
}