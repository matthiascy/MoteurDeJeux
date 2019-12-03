#include <GameFramework/Components/MeshRenderer.hpp>

MeshRenderer::MeshRenderer(String name, GameObject* gameObject, const AssetHandle& handle)
  : Renderer(std::move(name), gameObject), m_mesh_handle{handle}
{ }

AssetHandle MeshRenderer::meshHandle() const
{
  return m_mesh_handle;
}
