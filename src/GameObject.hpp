#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include "Types.hpp"
#include "Mesh.hpp"

class GameObject {
public:
  GameObject();
  explicit GameObject(const String& name, const String& type);
  virtual ~GameObject();

  enum class Type {
    Light,
    Mesh,
    Camera,
    Count,
  };

  const String& name() const { return m_name; }
  const String& type() const { return m_type; }

  void setSceneNodeAttached(bool isAttached) { m_is_scene_node_attached = isAttached; }
  bool isSceneNodeAttached() const { return m_is_scene_node_attached; }

  bool isVisible() const { return m_is_visible; }
  void setVisible(bool isVisible) { m_is_visible = isVisible; }

  void attachMesh(Mesh* mesh) { m_mesh = mesh; }
  void detachMesh() { m_mesh = nullptr; }
  const Mesh* mesh() const { return m_mesh; }

  Int32 vboIndex() const { return m_vbo_idx; }
  void setVboIndex(Int32 idx) { m_vbo_idx = idx; }

protected:
  String m_name;
  String m_type;
  GameObject* m_parent;
  bool m_is_scene_node_attached;
  bool m_is_visible;
  Int32 m_vbo_idx;

  //ResourceHandle m_mesh;
  //Array<ResourceHandle> m_textures;
  Mesh* m_mesh;

  NameGenerator* m_name_generator;
};

#endif // GAMEOBJECT_HPP
