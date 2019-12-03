#ifndef MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_MESH_RENDERER_HPP
#define MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_MESH_RENDERER_HPP

#include "Renderer.hpp"
#include <GameFramework/Assets/AssetsTypes.hpp>

class MeshRenderer : Renderer<MeshRenderer> {
private:
  AssetHandle m_mesh_handle;

public:
  MeshRenderer(const String& name, GameObject* gameObject, const AssetHandle& handle);

  [[nodiscard]]
  AssetHandle meshHandle() const;
};

#endif  /* !MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_MESH_RENDERER_HPP */
