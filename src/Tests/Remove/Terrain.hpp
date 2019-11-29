#ifndef PLANE_H
#define PLANE_H

#include <GameFramework/GameObject.hpp>

class Terrain : public GameObject {
public:
    Terrain(const String& name, const String& type);
    ~Terrain() override;
    void create(int width, int height, const QImage& hmap, bool isSameSize, float baseAlt, float scale);
};

#endif // PLANE_H