#include <Engine/GameFramework/GameObject.hpp>

#include <utility>

GameObject::GameObject(String name, Scene* scene)
  : Object(std::move(name)), m_tag{"None"}, m_transform{nullptr},
    m_is_static{false}, m_is_visible{true},
    m_scene{scene}, m_components{}
{ }

GameObject::GameObject(String name, Scene* scene, String tag)
    : Object(std::move(name)), m_tag{std::move(tag)}, m_transform{nullptr},
      m_is_static{false}, m_is_visible{true},
      m_scene{scene}, m_components{}
{ }

StringView GameObject::tag() const
{
  return m_tag;
}

void GameObject::setTag(const String& tag)
{
  m_tag = tag;
}

Transform* GameObject::transform()
{
  return m_transform;
}

const Transform* GameObject::transform() const
{
  return m_transform;
}

void GameObject::setTransform(Transform* trans)
{
  m_transform = trans;
}

bool GameObject::isVisible() const
{
  return m_is_visible;
}

void GameObject::setVisible(bool isVisible)
{
  m_is_visible = isVisible;
}

bool GameObject::isStatic() const
{
  return m_is_static;
}

void GameObject::setStatic(bool isStatic)
{
  m_is_static = isStatic;
}

Scene* GameObject::scene() const
{
  return m_scene;
}
