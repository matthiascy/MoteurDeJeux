#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_ASSETS_MESH_LOADER_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_ASSETS_MESH_LOADER_HPP

#include <Core/Core.hpp>

class Mesh;

class MeshLoader {
public:
  MeshLoader() = delete;
  static Mesh* loadMesh(const String& path);
};

#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_ASSETS_MESH_LOADER_HPP */
