#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_ASSETS_ASSET_MANAGER_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_ASSETS_ASSET_MANAGER_HPP

#include <Core/Public/Core.hpp>
#include <Graphics/Public/Forward.hpp>
#include <Graphics/Public/OpenGL/OglTypes.hpp>
#include <Graphics/Public/Animation.hpp>

class Engine;
class aiMesh;
class aiScene;
class aiMaterial;
class aiNode;
class aiNodeAnim;
class aiAnimation;
class aiBone;

// TODO: try to reuse model loading code while loading AnimatedModel
// TODO: AnimationLoader, ModelLoader

class AssetManager : public Object {

  friend class AssetLoader;

protected:
  UniquePtr<Array<Mesh*>>          m_meshes;
  UniquePtr<Array<Model*>>         m_models;
  UniquePtr<Array<Texture*>>       m_textures;
  UniquePtr<Array<Material*>>      m_materials;
  UniquePtr<Array<Skeleton*>>      m_skeletons;
  UniquePtr<Array<Animation*>>     m_animations;
  UniquePtr<Array<AnimatedModel*>> m_animated_models;
  Engine* m_engine;

public:
  AssetManager() = delete;
  AssetManager(const String& name, Engine* engine, Object* parent = nullptr);
  AssetManager(const AssetManager& other) = delete;
  ~AssetManager() override;

  ModelHandle loadModel(const String& path, const String& name);
  Model* getModel(ModelHandle handle);
  ModelHandle getModel(const String& name);

  TextureHandle loadTexture(const String& path);
  Texture* getTexture(TextureHandle handle);

  Material* getMaterial(MaterialHandle handle);
  Mesh* getMesh(MeshHandle handle);

  Animation* getAnimation(AnimationHandle handle);
  Skeleton* getSkeleton(SkeletonHandle handle);

  AnimatedModelHandle loadAnimatedModel(const String& path);
  AnimatedModel* getAnimatedModel(AnimatedModelHandle handle);

private:
  ModelHandle         _add_model(Model* model);
  MeshHandle          _add_mesh(Mesh* mesh);
  AnimatedModelHandle _add_animated_model(AnimatedModel* animatedModel);
  TextureHandle       _add_texture(Texture* texture);
  SkeletonHandle      _add_skeleton(Skeleton* skeleton);
  MaterialHandle      _add_material(Material* material);
  AnimationHandle     _add_animation(Animation* animation);

private:
  //void _process_node(const aiNode* node, const aiScene* scene);
  Mesh* _load_mesh(const aiMesh* assimpMesh, const aiScene* scene, const String& path);
  Material* _load_material(aiMaterial* assimpMaterial, const String& path);
  Array<TextureHandle> _load_material_textures(const aiMaterial* assimpMaterial, ETextureType type, const String& path);

  Model* _load_model(const String& path);
  AnimatedModel* _load_animated_model(const String& path);

  Int32 _recursively_load_skeleton(const aiNode* aNode, Skeleton* skeleton);
  void  _load_bones(MeshHandle owner, aiBone** meshBones, Int32 num, Skeleton* skeleton);
  Animation* _load_animation(const aiAnimation* aAnimation);
};

class AssetLoader {
private:
  AssetManager* m_manager;

public:
  AssetLoader(AssetManager* manager);

  void loadModel(const String& path);
  void loadAnimatedModel(const String& path);

private:
};

class ModelLoader {
public:
  static Model* load(const String& path);
};

class AnimatedModelLoader {
public:
  static AnimatedModel* load(const String& path);

 /*
private:
  static void readNodeHierarchy(Real animTime, const aiScene* scene, const aiNode* node, const Mat4& parentTransform);
  static UInt32 findScaling(Real animTime, const aiNodeAnim* nodeAnim);
  static UInt32 findRotation(Real animTime, const aiNodeAnim* nodeAnim);
  static UInt32 findPosition(Real animTime, const aiNodeAnim* nodeAnim);
  static Vec3 calcInterpolatedScaling(Real animTime, const aiNodeAnim* nodeAnim);
  static Quat calcInterpolatedRotation(Real animTime, const aiNodeAnim* nodeAnim);
  static Vec3 calcInterpolatedPosition(Real animTime, const aiNodeAnim* nodeAnim);
  static const aiNodeAnim* findAnimNode(const aiAnimation* anim, const String& nodeName);
  */
};

#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_ASSETS_ASSET_MANAGER_HPP */
