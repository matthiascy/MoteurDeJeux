#include "Terrain.hpp"
#include "Mesh.hpp"

Terrain::Terrain() : GameObject("Terrain", "Terrain")
{ }

void Terrain::create(int width, int height, const QImage& hmap, bool isSameSize, float baseAlt, float scale)
{
  m_mesh = new Mesh();
  qDebug() << "Terrain mesh pointer is null ? " << (m_mesh == nullptr);
  int w = isSameSize ? hmap.width() : width;
  int h = isSameSize ? hmap.height() : height;

  //m_mesh->data().resize(w * h * 8 * 8);
  const float step = 5.0f;
  const float ustep = 1.0f / w;
  const float vstep = 1.0f / h;
  const float w_half = w * step / (isSameSize? 4.0f : 2.0f);
  const float h_half = h * step / (isSameSize? 4.0f : 2.0f);

  for (int i = 0; i < width; ++i) {
    for (int j = 0; j < height; ++j) {
      m_mesh->addQuad(
           Vec3(i * step - w_half,     j * step - h_half,     baseAlt * ((hmap.pixelColor(i,   j).red()   - 128.0f) / scale)),
           Vec3((i+1) * step - w_half, j * step     - h_half, baseAlt * ((hmap.pixelColor(i+1, j).red()   - 128.0f) / scale)),
           Vec3((i+1) * step - w_half, (j+1) * step - h_half, baseAlt * ((hmap.pixelColor(i+1, j+1).red() - 128.0f) / scale)),
           Vec3(i * step     - w_half, (j+1) * step - h_half, baseAlt * ((hmap.pixelColor(i,   j+1).red() - 128.0f) / scale)),
           Vec2(i * ustep, j * vstep),
           Vec2((i+1) * ustep, j * vstep),
           Vec2((i+1) * ustep, (j+1) * vstep),
           Vec2(i * ustep, (j+1) * vstep));
    }
  }
}

/*
void Terrain::add(const Vec3 &v, const Vec3 &n,  const Vec2 &t)
{
    GLfloat *p = m_mesh->data().data() + m_count;
    *p++ = v.x();
    *p++ = v.y();
    *p++ = v.z();
    *p++ = n.x();
    *p++ = n.y();
    *p++ = n.z();
    *p++ = t.x();
    *p++ = t.y();
    m_count += 8;
    m_mesh->setCount(m_count);
}

void Terrain::quad(Vec3 p1, Vec3 p2, Vec3 p3, Vec3 p4, Vec2 uv1, Vec2 uv2, Vec2 uv3, Vec2 uv4)
{
    Vec3 n = Vec3::normal(Vec3(p4.x() - p1.x(), p4.y() - p1.y(), 0.0f), Vec3(p2.x() - p1.x(), p2.y() - p1.y(), 0.0f));

    add(p1, n, uv1);//, Vec2(u1, v1));
    add(p4, n, uv4);//, Vec2(u4, v4));
    add(p2, n, uv2);//, Vec2(u2, v2));

    add(p3, n, uv3);//, Vec2(u3, v3));
    add(p2, n, uv2);//, Vec2(u2, v2));
    add(p4, n, uv4);//, Vec2(u4, v4));
}
*/
