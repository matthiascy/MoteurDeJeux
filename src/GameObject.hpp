#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include "Types.hpp"
#include "Mesh.hpp"
#include "Transform.hpp"
#include <btBulletCollisionCommon.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>

// TODO: instantly all objects have a collider, seperate

class GameObject {
protected:
  String m_name;
  String m_type;

  GameObject* m_parent;
  Transform m_transform;
  Mat4 m_world_matrix;

  Array<GameObject*> m_children;

  bool m_is_visible;
  bool m_is_static;

  Int32 m_vbo_idx;

  Mesh*     m_mesh;
  Collider* m_collider;
public:
  GameObject() = delete;
  explicit GameObject(String name, String type);
  virtual ~GameObject() = default;

  const String& name() const { return m_name; }
  const String& type() const { return m_type; }

  bool isVisible() const { return m_is_visible; }
  void setVisible(bool isVisible) { m_is_visible = isVisible; }

  // TODO: who will destroy mesh ?
  void attachMesh(Mesh* mesh);
  void detachMesh();
  const Mesh* mesh() const;

  void attachCollider(Collider* collider);
  void detachCollider();
  const Collider* collider() const;

  Int32 vboIndex() const { return m_vbo_idx; }
  void setVboIndex(Int32 idx) { m_vbo_idx = idx; }

  GameObject& setParent(GameObject* parent) { m_parent = parent; return *this; }

  void setTransform(Transform trans) { m_transform = trans; }
  const Transform& transform() const { return m_transform; }
  Transform& transform() { return m_transform; }

  const Mat4& worldMatrix();

  void update();
};

#endif // GAMEOBJECT_HPP
