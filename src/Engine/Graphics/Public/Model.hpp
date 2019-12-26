#ifndef MOTEUR_DE_JEUX_SRC_GRAPHICS_MODEL_HPP
#define MOTEUR_DE_JEUX_SRC_GRAPHICS_MODEL_HPP

#include "Mesh.hpp"

/**
 * Class Model doesn't own the Mesh.
 */
class Model {
private:
  Array<Mesh*> meshes;

public:
  Model();
};


#endif //HOMELAND_MODEL_HPP
