#ifndef MESH_HPP
#define MESH_HPP

#include <Core/Public/Core.hpp>
#include <Graphics/Public/Forward.hpp>

class aiMesh;
class aiNode;
class aiNodeAnim;
class aiAnimation;

#define NUM_BONES_PER_VERTEX 4

struct BoneInfo {
  Mat4 boneOffset     { Math::Mat4Identity };
  Mat4 finalTransform { Math::Mat4Identity };
};

struct VertexBoneData {
  UInt32 bonesIdx[NUM_BONES_PER_VERTEX];
  Real   weights[NUM_BONES_PER_VERTEX];

  VertexBoneData() : bonesIdx{}, weights{}
  { };

  void addBoneData(UInt32 boneId, Real weight);
};

/*
 * Layout { position | normal | texCoord | tangent | biTangent }
 */
class Mesh {
private:
  String m_name;
  UInt32 m_data_count;
  UInt32 m_vertex_count;
  Array<UInt32> m_indices;
  Array<VertexLayoutPNTTB> m_vertices;
  MaterialHandle m_material{};

  UInt32 m_num_bones;
  Mat4   m_world_inverse_transform;
  HashMap<String, UInt32> m_bone_mapping; // bone name <--> index
  Array<BoneInfo> m_bone_info;

public:
  Mesh();
  Mesh(UInt32 vertexCount, const Array<VertexLayoutPNTTB>& data, const Array<UInt32>& indices);
  ~Mesh() = default;

  [[nodiscard]]
  UInt32 dataCount() const { return m_data_count; }

  [[nodiscard]]
  UInt32 vertexCount() const { return m_vertex_count; }

  [[nodiscard]]
  const Array<VertexLayoutPNTTB>& vertices() const;

  Array<VertexLayoutPNTTB>& vertices();

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

  [[nodiscard]]
  Array<Vec3> tangents() const;

  [[nodiscard]]
  Array<Vec3> biTangents() const;

  void setVertices(UInt32 vertexCount, const Array<VertexLayoutPNTTB>& data, const Array<UInt32>& indices);

  [[nodiscard]]
  MaterialHandle material() const;

  void setMaterial(MaterialHandle handle);

  const String& name() { return m_name; }

  [[nodiscard]]
  UInt32 numBones() const;

  void boneTransform(Real dt, Array<Mat4>& transforms);
};

#endif // MESH_HPP
