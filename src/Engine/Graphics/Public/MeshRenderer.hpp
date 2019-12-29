#ifndef MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_MESH_RENDERER_HPP
#define MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_MESH_RENDERER_HPP

#include "Renderer.hpp"
#include <GameFramework/Public/Managers/AssetManager.hpp>

class MeshRenderer : public Renderer {
private:
  ModelHandle m_model { -1 };

  /** Following attributes will be initialized before drawing */
  Array<VboHandle> m_vbos { };
  Array<IboHandle> m_ibos { };
  Array<Mesh*> m_meshes_to_rendered { };

public:
  MeshRenderer(const String& name, GameObject* gameObject, const ModelHandle& handle);
  ~MeshRenderer() override = default;

  [[nodiscard]]
  UInt64 typeID() const override;

  [[nodiscard]]
  ModelHandle modelHandle() const;

  void init(RenderSystem* renderSystem, AssetManager* assetManager) override;

  void draw(RenderSystem* renderSystem, OglProgram* program, AssetManager* assetManager, Real dt) override;
};

#endif  /* !MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_MESH_RENDERER_HPP */
