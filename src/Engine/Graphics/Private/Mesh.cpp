#include "Engine/Graphics/Mesh.hpp"

Mesh::Mesh()
  : m_vertex_count{0}, m_indices{}, m_data{}, m_count{0}
{ }

Mesh::Mesh(UInt32 vcount, const Array<float>& data, const Array<UInt32>& indices)
  : m_vertex_count{vcount}
{
  m_data.clear();
  m_data = data;
  m_indices.clear();
  m_indices = indices;
  m_count = m_data.size();
}

const Array<UInt32>& Mesh::indices() const
{
  return m_indices;
}

Array<Vec3> Mesh::vertices() const
{
  Array<Vec3> vs;
  for (auto i = 0; i < m_vertex_count; ++i) {
    vs << Vec3{m_data[i*8], m_data[i*8+1], m_data[i*8+2]};
  }
  return vs;
}

Array<Vec3> Mesh::normals() const
{
  Array<Vec3> ns;
  for (auto i = 0; i < m_vertex_count; ++i) {
    ns << Vec3{m_data[i*8+3], m_data[i*8+4], m_data[i*8+5]};
  }
  return ns;
}

Array<Vec2> Mesh::texCoords() const
{
  Array<Vec2> ts;
  for (auto i = 0; i < m_vertex_count; ++i) {
    ts << Vec2{m_data[i*8+6], m_data[i*8+7]};
  }
  return ts;
}

void Mesh::addVertex(const Vec3& v, const Vec3& n, const Vec2& t)
{
  m_indices.push_back(m_data.size() / 8);
  m_data.push_back(v.x());
  m_data.push_back(v.y());
  m_data.push_back(v.z());
  m_data.push_back(n.x());
  m_data.push_back(n.y());
  m_data.push_back(n.z());
  m_data.push_back(t.x());
  m_data.push_back(t.y());
  m_count += 8;
  m_vertex_count += 1;
}

void Mesh::addQuad(const Vec3& p1, const Vec3& p2, const Vec3& p3, const Vec3& p4,
                   const Vec2& uv1, const Vec2& uv2, const Vec2& uv3, const Vec2& uv4)
{
  Vec3 n = Vec3::normal(Vec3(p4.x() - p1.x(), p4.y() - p1.y(), 0.0f), Vec3(p2.x() - p1.x(), p2.y() - p1.y(), 0.0f));
  addVertex(p1, n, uv1);//, Vec2(u1, v1));
  addVertex(p4, n, uv4);//, Vec2(u4, v4));
  addVertex(p2, n, uv2);//, Vec2(u2, v2));
  addVertex(p3, n, uv3);//, Vec2(u3, v3));
  addVertex(p2, n, uv2);//, Vec2(u2, v2));
  addVertex(p4, n, uv4);//, Vec2(u4, v4));
}

