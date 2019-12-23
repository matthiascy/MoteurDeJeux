#ifndef MESH_HPP
#define MESH_HPP

#include <Core/Public/Core.hpp>

/*
 * Compact layout { position | normal | texCoord }
 *                  0  1  2    3  4  5   6  7
 */
class Mesh {
public:
  Mesh();
  Mesh(UInt32 vertexCount, const Array<float>& data, const Array<UInt32>& indices);

  [[nodiscard]]
  UInt32 count() const { return m_count; }

  [[nodiscard]]
  UInt32 vertexCount() const { return m_vertex_count; }

  [[nodiscard]]
  const Array<UInt32>& indices() const;

  Array<UInt32>& indices() { return m_indices; }

  void calculateSphericalUV();

  [[nodiscard]]
  Array<Vec3> vertices() const;

  [[nodiscard]]
  Array<Vec3> normals() const;

  [[nodiscard]]
  Array<Vec2> texCoords() const;

  [[nodiscard]]
  const Array<float>& constDataArray() const { return m_data; }

  Array<float>& dataArray() { return m_data; }

  [[nodiscard]]
  const float* constData() const { return m_data.constData(); }

  void addVertex(const Vec3& v, const Vec3& n, const Vec2& t);
  void addQuad(const Vec3& p1, const Vec3& p2, const Vec3& p3, const Vec3& p4,
               const Vec2& uv1, const Vec2& uv2, const Vec2& uv3, const Vec2& uv4);

  const String& name() { return m_name; }

protected:
  UInt32 m_vertex_count;
  Array<UInt32> m_indices;
  Array<float> m_data;
  UInt32 m_count;
  String m_name;
};

#endif // MESH_HPP
