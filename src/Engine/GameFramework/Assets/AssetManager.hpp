#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_ASSETS_ASSET_MANAGER_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_ASSETS_ASSET_MANAGER_HPP

#include <Core/Core.hpp>
#include <Graphics/Mesh.hpp>
#include <Graphics/GraphicsTypes.hpp>

class Engine;

enum class EAssetType {
  Texture,
  Audio,
  Mesh,
};

struct AssetHandle {
  UInt32 idx;
  EAssetType type;
};

class AssetManager : Object {
public:

protected:
  Array<Mesh*>       m_meshes;
  Array<OglTexture*> m_textures;

public:
  AssetManager() = delete;
  explicit AssetManager(String name);
  ~AssetManager() override;

  AssetHandle loadMesh(const String& path);
  AssetHandle loadTexture(const String& path);

  // TODO
  //Asset* getAsset(AssetHandle handle);
  OglTexture* getTexture(AssetHandle handle);
  Mesh* getMesh(AssetHandle handle);
};

#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_ASSETS_ASSET_MANAGER_HPP */
