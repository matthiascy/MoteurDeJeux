#ifndef MOTEUR_DE_JEUX_SRC_GRAPHICS_MODEL_HPP
#define MOTEUR_DE_JEUX_SRC_GRAPHICS_MODEL_HPP

#include "Mesh.hpp"
#include <Graphics/Public/Forward.hpp>

/**
 * Class Model doesn't own the Mesh.
 */
class Model {
private:
  Array<MeshHandle> m_meshes;
  String m_name;

public:
  Model() = default;

  void addMesh(MeshHandle mesh);

  [[nodiscard]]
  String name() const;

  void setName(const String& name);

  [[nodiscard]]
  Array<MeshHandle> meshes() const;

  [[nodiscard]]
  Int32 meshesNum() const;
};


#endif //HOMELAND_MODEL_HPP
