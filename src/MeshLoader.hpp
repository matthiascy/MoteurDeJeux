#ifndef MESHLOADER_HPP
#define MESHLOADER_HPP

#include "Types.hpp"


class MeshLoader {
public:
  MeshLoader() = delete;
  static Mesh* loadMesh(const String& path);
};

#endif // MESHLOADER_HPP
