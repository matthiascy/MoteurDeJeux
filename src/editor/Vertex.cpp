#include "Vertex.hpp"


namespace gtd {

  const int Vertex::stride = sizeof(Vertex);
  const int Vertex::posOffset = offsetof(Vertex, pos);
  const int Vertex::colorOffset = offsetof(Vertex, color);
  const int Vertex::texCoordOffset = offsetof(Vertex, texCoord);

}  // !namespace gtd
