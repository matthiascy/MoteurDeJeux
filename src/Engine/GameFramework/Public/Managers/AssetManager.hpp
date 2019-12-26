#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_ASSETS_ASSET_MANAGER_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_ASSETS_ASSET_MANAGER_HPP

#include <Core/Public/Core.hpp>
#include <Graphics/Public/Mesh.hpp>
#include <Graphics/Public/Types.hpp>

class Engine;

enum class EAssetType {
  Texture,
  Audio,
  Mesh,
  Model,
};

struct AssetHandle {
  UInt32 idx;
  EAssetType type;
};

class AssetManager : public Object {
public:

protected:
  UniquePtr<Array<Mesh*>> m_meshes;
  Array<OglTexture*> m_textures;

public:
  AssetManager() = delete;
  explicit AssetManager(const String& name, Object* parent = nullptr);
  AssetManager(const AssetManager& other) = delete;
  ~AssetManager() override;

  AssetHandle loadMesh(const String& path);
  AssetHandle loadTexture(const String& path);
  AssetHandle loadModel(const String& path);

  // TODO
  //Asset* getAsset(AssetHandle handle);
  OglTexture* getTexture(AssetHandle handle);
  Mesh* getMesh(AssetHandle handle);
};

#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_ASSETS_ASSET_MANAGER_HPP */
