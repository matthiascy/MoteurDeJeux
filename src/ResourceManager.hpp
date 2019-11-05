#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include "Types.hpp"
#include "NameGenerator.hpp"


class ResourceManager {
public:
  typedef HashMap<String, Mesh*> MeshMap;
  typedef HashMap<String, QImage*> TextureMap;

protected:
  MeshMap    m_meshes;
  TextureMap m_textures;

  static NameGenerator m_name_generator;

public:
  ResourceManager();
  ResourceHandle loadMesh(const String& path);
  ResourceHandle loadTexture(const String& path);
};

#endif // RESOURCEMANAGER_HPP
