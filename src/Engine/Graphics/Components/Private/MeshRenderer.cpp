#include <Graphics/Components/MeshRenderer.hpp>

MeshRenderer::MeshRenderer(const String& name, GameObject* gameObject, const AssetHandle& handle)
  : Renderer(name, gameObject), m_mesh_handle{handle}
{ }

AssetHandle MeshRenderer::meshHandle() const
{
  return m_mesh_handle;
}
