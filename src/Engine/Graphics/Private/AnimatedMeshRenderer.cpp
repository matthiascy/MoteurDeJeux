#include "Graphics/Public/AnimatedMeshRenderer.hpp"

AnimatedMeshRenderer::AnimatedMeshRenderer(const String& name, GameObject* gameObject,
                                           const AnimatedModelHandle& handle)
  : Renderer(name, gameObject), m_model_handle{handle}
{

}

UInt64 AnimatedMeshRenderer::typeID() const
{
  return family::type<AnimatedMeshRenderer>;
}

AnimatedModelHandle AnimatedMeshRenderer::modelHandle() const
{
  return m_model_handle;
}

void AnimatedMeshRenderer::init(RenderSystem* renderSystem)
{
}

void AnimatedMeshRenderer::draw(RenderSystem* renderSystem, OglProgram* program, AssetManager* assetManager, Real dt)
{
  renderSystem->makeCurrent();
  renderSystem->doneCurrent();
}
