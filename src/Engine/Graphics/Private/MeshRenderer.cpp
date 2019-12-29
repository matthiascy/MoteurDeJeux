#include <Graphics/Public/MeshRenderer.hpp>
#include <Graphics/Public/RenderSystem.hpp>

MeshRenderer::MeshRenderer(const String& name, GameObject* gameObject, const ModelHandle& handle)
  : Renderer(name, gameObject), m_model{handle}
{ }

UInt64 MeshRenderer::typeID() const
{
  return family::type<MeshRenderer>;
}

ModelHandle MeshRenderer::modelHandle() const
{
  return m_model;
}

void MeshRenderer::draw(RenderSystem* renderSystem, OglProgram* program, AssetManager* assetManager, Real dt)
{
  renderSystem->makeCurrent();
  renderSystem->doneCurrent();
}

void MeshRenderer::init(RenderSystem* renderSystem)
{
}
