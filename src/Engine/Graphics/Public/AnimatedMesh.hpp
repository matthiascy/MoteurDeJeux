#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_ANIMATED_MESH_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_ANIMATED_MESH_HPP

#include "Mesh.hpp"

class AnimatedMesh {
private:
  MeshHandle m_mesh;
  Array<BoneHandle> m_bones;

public:
  void attachMesh(MeshHandle handle);

  void addBone(BoneHandle bone);
};


#endif /* !MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_ANIMATED_MESH_HPP */
