#ifndef MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_RENDERER_HPP
#define MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_RENDERER_HPP

#include <GameFramework/Public/ECS/Component.hpp>

class RenderSystem;

class Renderer : public Component {
protected:
  bool m_is_initialized { false };

public:
  Renderer(const String& name, GameObject* gameObject);
  ~Renderer() override = default;

  virtual void init(RenderSystem* renderSystem, AssetManager* assetManager) = 0;

  virtual void draw(RenderSystem* renderSystem, OglProgram* program, AssetManager* assetManager, Real dt) = 0;

protected:
  void enable_vertex_attrib_array(UInt32 num, OglFnsCore4_0* fns);
};

#endif  /* !MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_RENDERER_HPP */
