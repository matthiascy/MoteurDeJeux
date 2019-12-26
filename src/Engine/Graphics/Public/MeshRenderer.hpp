#ifndef MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_MESH_RENDERER_HPP
#define MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_MESH_RENDERER_HPP

#include "Renderer.hpp"
#include <GameFramework/Public/Managers/AssetManager.hpp>

class MeshRenderer : public Renderer {
private:
  ModelHandle m_model_handle;

public:
  MeshRenderer(const String& name, GameObject* gameObject, const ModelHandle& handle);

  [[nodiscard]]
  UInt64 typeID() const override {
    return family::type<MeshRenderer>;
  }

  [[nodiscard]]
  ModelHandle modelHandle() const;
};

#endif  /* !MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_MESH_RENDERER_HPP */
