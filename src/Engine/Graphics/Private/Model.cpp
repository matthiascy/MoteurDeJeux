#include "Graphics/Public/Model.hpp"

void Model::addMesh(MeshHandle mesh)
{
  if (!m_meshes.contains(mesh))
    m_meshes.push_back(mesh);
}

Array<MeshHandle> Model::meshes() const
{
  return m_meshes;
}

Int32 Model::meshesNum() const
{
  return m_meshes.size();
}