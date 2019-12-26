#ifndef MESH_HPP
#define MESH_HPP

#include <Core/Public/Core.hpp>
#include <Graphics/Public/Forward.hpp>

/*
 * Compact layout { position | normal | texCoord }
 *                  0  1  2    3  4  5   6  7
 */
class Mesh {
protected:
  String m_name;
  UInt32 m_data_count;
  UInt32 m_vertex_count;
  Array<UInt32> m_indices;
  Array<VertexLayoutPNT> m_vertices;
  //Array<float> m_data;
  MaterialHandle m_material{};

public:
  Mesh();
  Mesh(UInt32 vertexCount, const Array<VertexLayoutPNT>& data, const Array<UInt32>& indices);
  ~Mesh() = default;

  [[nodiscard]]
  UInt32 dataCount() const { return m_data_count; }

  [[nodiscard]]
  UInt32 vertexCount() const { return m_vertex_count; }

  [[nodiscard]]
  const Array<VertexLayoutPNT>& vertices() const;

  Array<VertexLayoutPNT>& vertices();

  [[nodiscard]]
  const Array<UInt32>& indices() const;

  Array<UInt32>& indices();

  void calculateSphericalUV();

  [[nodiscard]]
  Array<Vec3> positions() const;

  [[nodiscard]]
  Array<Vec3> normals() const;

  [[nodiscard]]
  Array<Vec2> texCoords() const;

  void setVertices(UInt32 vertexCount, const Array<VertexLayoutPNT>& data, const Array<UInt32>& indices);

  const String& name() { return m_name; }
};

#endif // MESH_HPP
