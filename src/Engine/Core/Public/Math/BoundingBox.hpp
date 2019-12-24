#ifndef MOTEUR_DE_JEUX_SRC_CORE_PUBLIC_BOUNDING_BOX_HPP
#define MOTEUR_DE_JEUX_SRC_CORE_PUBLIC_BOUNDING_BOX_HPP

#include "MathTypes.hpp"

class BoundingBox {
private:
  BBox m_bbox;

public:
  BoundingBox(Real min, Real max);
  BoundingBox(const Vec3& min, const Vec3& max);

  Vec3 center() const;
  Vec3 size() const;
  Vec3 halfSize() const;

  bool operator==(const BoundingBox& other) const;

  bool operator!=(const BoundingBox& other) const;
};


#endif  /* !MOTEUR_DE_JEUX_SRC_CORE_PUBLIC_BOUNDING_BOX_HPP */
