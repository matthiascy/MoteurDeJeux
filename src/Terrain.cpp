#include "Terrain.hpp"
#include "graphics/Mesh.hpp"

Terrain::Terrain(const String& name, const String& type) : GameObject(name, type)
{ }

void Terrain::create(int width, int height, const QImage& hmap, bool isSameSize, float baseAlt, float scale)
{
  m_mesh = new Mesh();
  int w = isSameSize ? hmap.width() : width;
  int h = isSameSize ? hmap.height() : height;

  //m_mesh->data().resize(w * h * 8 * 8);
  const float step = 10.0f;
  const float ustep = 1.0f / w;
  const float vstep = 1.0f / h;
  const float w_half = w * step / (isSameSize? 4.0f : 2.0f);
  const float h_half = h * step / (isSameSize? 4.0f : 2.0f);

  for (int i = 0; i < width; ++i) {
    for (int j = 0; j < height; ++j) {
      m_mesh->addQuad(
           Vec3(i * step - w_half,     baseAlt * ((hmap.pixelColor(i,   j).red()   - 128.0f) / scale), j * step - h_half),
           Vec3((i+1) * step - w_half, baseAlt * ((hmap.pixelColor(i+1, j).red()   - 128.0f) / scale), j * step     - h_half),
           Vec3((i+1) * step - w_half, baseAlt * ((hmap.pixelColor(i+1, j+1).red() - 128.0f) / scale), (j+1) * step - h_half),
           Vec3(i * step     - w_half, baseAlt * ((hmap.pixelColor(i,   j+1).red() - 128.0f) / scale), (j+1) * step - h_half),
           Vec2(i * ustep, j * vstep),
           Vec2((i+1) * ustep, j * vstep),
           Vec2((i+1) * ustep, (j+1) * vstep),
           Vec2(i * ustep, (j+1) * vstep));
    }
  }

  m_collision_shape = new btBoxShape(btVector3(1, 1, 1));
  btVector3 localInertial = btVector3(0, 0, 0);
  m_collision_shape->calculateLocalInertia(BT_ONE, localInertial);
      // new btStaticPlaneShape(btVector3(0.0, 1.0, 0.0), BT_ONE);
  m_motion_state = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
  m_rigid_body = new btRigidBody(BT_ONE, m_motion_state, m_collision_shape);
}

Terrain::~Terrain()
{
  delete m_mesh;
  delete m_collision_shape;
  delete m_motion_state;
  delete m_rigid_body;
}