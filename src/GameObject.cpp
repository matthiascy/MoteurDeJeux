#include "GameObject.hpp"

#include <utility>
#include "NameGenerator.hpp"

GameObject::GameObject()
  : m_name{}, m_type{}, m_parent{nullptr}, m_is_scene_node_attached{false},
    m_is_visible{false}, m_mesh{nullptr}, m_name_generator{nullptr}, m_vbo_idx{-1}
{
  m_name_generator = new NameGenerator("GameObject");
  m_name = m_name_generator->generate();
  //m_mesh = new Mesh();
}

GameObject::GameObject(const String& name, const String& type)
  : m_name{name}, m_type{type}, m_parent{nullptr}, m_is_scene_node_attached{false},
    m_is_visible{false}, m_mesh{nullptr}, m_name_generator{nullptr}, m_vbo_idx{-1}
{
  //m_mesh = new Mesh();
}

GameObject::~GameObject()
{
  if (m_name_generator)
    delete m_name_generator;
}
