#include "Graphics/Public/Mesh.hpp"

Mesh::Mesh()
  : m_vertex_count{0}, m_indices{}, m_vertices{}, m_data_count{0}, m_material{0}
{ }

Mesh::Mesh(UInt32 vcount, const Array<VertexLayoutPNTTB>& data, const Array<UInt32>& indices)
  : m_vertex_count{vcount}, m_material{0}
{
  m_vertices.clear();
  m_vertices = data;
  m_indices.clear();
  m_indices = indices;
  m_data_count = m_vertex_count * 14;
}

const Array<UInt32>& Mesh::indices() const
{
  return m_indices;
}

Array<UInt32>& Mesh::indices()
{
  return m_indices;
}

Array<Vec3> Mesh::positions() const
{
  Array<Vec3> vs;
  for (auto i = 0; i < m_vertex_count; ++i) {
    vs << m_vertices[i].position;
  }
  return vs;
}

Array<Vec3> Mesh::normals() const
{
  Array<Vec3> ns;
  for (auto i = 0; i < m_vertex_count; ++i) {
    ns << m_vertices[i].normal;
  }
  return ns;
}

Array<Vec2> Mesh::texCoords() const
{
  Array<Vec2> ts;
  for (auto i = 0; i < m_vertex_count; ++i) {
    ts << m_vertices[i].texCoord;
  }
  return ts;
}

void Mesh::calculateSphericalUV()
{
  for (UInt32 i = 0; i < vertexCount(); ++i) {
    Vec3 n = m_vertices[i].position.normalized();
    Real u = qAtan2(n.x(), n.z()) / (2 * M_PI) + 0.5;
    Real v = n.y() * 0.5 + 0.5;

    m_vertices[i].texCoord = { u, v };
  }
}

Array <VertexLayoutPNTTB>& Mesh::vertices() 
{
  return m_vertices;
}

const Array<VertexLayoutPNTTB>& Mesh::vertices() const
{
  return m_vertices;
}

void Mesh::setVertices(UInt32 vertexCount, const Array<VertexLayoutPNTTB>& data, const Array<UInt32>& indices)
{
  m_vertex_count = vertexCount;
  m_vertices.clear();
  m_vertices = data;
  m_indices.clear();
  m_indices = indices;
  m_data_count = m_vertices.size() * 14;
}

Array<Vec3> Mesh::tangents() const
{
  Array<Vec3> ts;
  for (auto i = 0; i < m_vertex_count; ++i) {
    ts << m_vertices[i].tangent;
  }
  return ts;
}

Array<Vec3> Mesh::biTangents() const
{
  Array<Vec3> bs;
  for (auto i = 0; i < m_vertex_count; ++i) {
    bs << m_vertices[i].biTangent;
  }
  return bs;
}

void Mesh::setMaterial(MaterialHandle handle)
{
  m_material = handle;
}

MaterialHandle Mesh::material() const
{
  return m_material;
}

