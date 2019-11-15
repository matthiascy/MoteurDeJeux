#include <Engine/GameFramework/GameObject.hpp>

#include <utility>

GameObject::GameObject(String name, Scene* scene)
  : Object(std::move(name)), m_tag{"None"}, m_transform{nullptr},
    m_is_static{false}, m_is_visible{true},
    m_scene{scene}, m_components{}
{
  m_transform = scene->
}

GameObject::GameObject(String name, String tag)
  : Object(name), m_tag{std::move(tag)},
    m_parent{nullptr},
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
