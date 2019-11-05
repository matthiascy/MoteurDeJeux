#ifndef PLANE_H
#define PLANE_H

#include "Types.hpp"
#include "GameObject.hpp"

class Terrain : public GameObject {
public:
    Terrain();
    ~Terrain() override = default;
    void create(int width, int height, const QImage& hmap, bool isSameSize, float baseAlt, float scale);
};

#endif // PLANE_H