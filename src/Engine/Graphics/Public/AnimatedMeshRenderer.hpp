#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_ANIMATED_MESH_RENDERER_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_ANIMATED_MESH_RENDERER_HPP

#include "Renderer.hpp"

/**
 * Contains the animated model.
 */
class AnimatedMeshRenderer : public Renderer {
private:
  AnimatedModelHandle m_model_handle;

public:
  AnimatedMeshRenderer(const String& name, GameObject* gameObject, const AnimatedModelHandle& handle);

  [[nodiscard]]
  UInt64 typeID() const override;

  [[nodiscard]]
  AnimatedModelHandle modelHandle() const;

  void init(RenderSystem* renderSystem, AssetManager* assetManager) override;

  void draw(RenderSystem* renderSystem, OglProgram* program, AssetManager* assetManager, Real dt) override;
};


#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_ANIMATED_MESH_RENDERER_HPP */
