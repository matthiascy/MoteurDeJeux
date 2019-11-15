#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_ASSETS_ASSET_MANAGER_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_ASSETS_ASSET_MANAGER_HPP

#include <Engine/Core/Core.hpp>
#include <Engine/Graphics/Mesh.hpp>

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
  Array<Mesh*>          m_meshes;
  Array<OpenGLTexture*> m_textures;

public:
  AssetManager() = delete;
  explicit AssetManager(String name);
  ~AssetManager() override;

  AssetHandle loadMesh(const String& path);
  AssetHandle loadTexture(const String& path);

  // TODO
  //Asset* getAsset(AssetHandle handle);
  OpenGLTexture* getTexture(AssetHandle handle);
  Mesh* getMesh(AssetHandle handle);
};

#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_ASSETS_ASSET_MANAGER_HPP */
