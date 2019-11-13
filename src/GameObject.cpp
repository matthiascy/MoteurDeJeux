#include "GameObject.hpp"

#include <utility>
#include "NameGenerator.hpp"

GameObject::GameObject(String name, String type)
  : m_name{std::move(name)}, m_type{std::move(type)}, m_parent{nullptr},
    m_transform{}, m_is_visible{true}, m_vbo_idx{-1},
    m_mesh{nullptr}, m_collider{nullptr}
{ }

/*
GameObject::~GameObject()
{ }
 */

void GameObject::attachMesh(Mesh* mesh)
{
  m_mesh = mesh;
}

void GameObject::detachMesh()
{
  m_mesh = nullptr;
}

const Mesh* GameObject::mesh() const
{
  return m_mesh;
}

const Collider* GameObject::collider() const
{
  return m_collider;
}

void GameObject::attachCollider(Collider* collider)
{
  m_collider = collider;
}

void GameObject::detachCollider()
{
  m_collider = nullptr;
}
