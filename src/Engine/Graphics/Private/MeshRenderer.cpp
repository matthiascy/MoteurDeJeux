#include <Graphics/Public/MeshRenderer.hpp>

MeshRenderer::MeshRenderer(const String& name, GameObject* gameObject, const ModelHandle& handle)
  : Renderer(name, gameObject), m_model_handle{handle}
{ }

ModelHandle MeshRenderer::modelHandle() const
{
  return m_model_handle;
}
