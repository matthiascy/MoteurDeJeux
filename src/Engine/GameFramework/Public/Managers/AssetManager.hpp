#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_ASSETS_ASSET_MANAGER_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_ASSETS_ASSET_MANAGER_HPP

#include <Core/Public/Core.hpp>
#include <Graphics/Public/Forward.hpp>
#include <Graphics/Public/Mesh.hpp>
#include <Graphics/Public/OpenGL/OglTypes.hpp>

class Engine;
class aiMesh;

class AssetManager : public Object {
public:

protected:
  UniquePtr<Array<Mesh*>> m_meshes;
  UniquePtr<Array<Model*>> m_models;
  UniquePtr<Array<Material*>> m_materials;
  Array<OglTexture*> m_textures;

public:
  AssetManager() = delete;
  explicit AssetManager(const String& name, Object* parent = nullptr);
  AssetManager(const AssetManager& other) = delete;
  ~AssetManager() override;

  ModelHandle loadModel(const String& path);

  TextureHandle loadTexture(const String& path);

  // TODO
  //Asset* getAsset(AssetHandle handle);
  OglTexture* getTexture(TextureHandle handle);
  Model* getModel(ModelHandle handle);
  Mesh* getMesh(MeshHandle handle);

private:
  Mesh* _load_mesh(const String& path);
  Mesh* _load_assimp_mesh(const aiMesh* assimpMesh);
  Model* _load_model(const String& path);
};

#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_ASSETS_ASSET_MANAGER_HPP */
