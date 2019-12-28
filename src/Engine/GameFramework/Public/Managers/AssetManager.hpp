#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_ASSETS_ASSET_MANAGER_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_ASSETS_ASSET_MANAGER_HPP

#include <Core/Public/Core.hpp>
#include <Graphics/Public/Forward.hpp>
#include <Graphics/Public/OpenGL/OglTypes.hpp>

class Engine;
class aiMesh;
class aiScene;
class aiMaterial;
class aiNode;

class AssetManager : public Object {
protected:
  UniquePtr<Array<Mesh*>> m_meshes;
  UniquePtr<Array<Model*>> m_models;
  UniquePtr<Array<Material*>> m_materials;
  UniquePtr<Array<Texture*>>  m_textures;
  Engine* m_engine;

public:
  AssetManager() = delete;
  AssetManager(const String& name, Engine* engine, Object* parent = nullptr);
  AssetManager(const AssetManager& other) = delete;
  ~AssetManager() override;

  ModelHandle loadModel(const String& path);

  TextureHandle loadTexture(const String& path);

  Texture* getTexture(TextureHandle handle);
  Material* getMaterial(MaterialHandle handle);
  Model* getModel(ModelHandle handle);
  Mesh* getMesh(MeshHandle handle);

private:
  //void _process_node(const aiNode* node, const aiScene* scene);
  Mesh* _load_mesh(const aiMesh* assimpMesh, const aiScene* scene, const String& path);
  Material* _load_material(aiMaterial* assimpMaterial, const String& path);
  Array<TextureHandle> _load_material_textures(const aiMaterial* assimpMaterial, ETextureType type, const String& path);
  Model* _load_model(const String& path);
  Array<VertexBoneData> _load_bones(const aiMesh* assimpMesh);
};

class AnimationLoader {
public:
  static void readNodeHierarchy(Real animTime, const aiScene* scene, const aiNode* node, const Mat4& parentTransform);
  static UInt32 findScaling(Real animTime, const aiNodeAnim* nodeAnim);
  static UInt32 findRotation(Real animTime, const aiNodeAnim* nodeAnim);
  static UInt32 findPosition(Real animTime, const aiNodeAnim* nodeAnim);
  static Vec3 calcInterpolatedScaling(Real animTime, const aiNodeAnim* nodeAnim);
  static Quat calcInterpolatedRotation(Real animTime, const aiNodeAnim* nodeAnim);
  static Vec3 calcInterpolatedPosition(Real animTime, const aiNodeAnim* nodeAnim);
  static const aiNodeAnim* findAnimNode(const aiAnimation* anim, const String& nodeName);
};

#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_ASSETS_ASSET_MANAGER_HPP */
