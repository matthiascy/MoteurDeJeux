#include <GameFramework/GameObject.hpp>
#include <GameFramework/Components/Transform.hpp>

#include <utility>

GameObject::GameObject(const String& name, Scene* scene, Object* parent)
  : Object(name, parent), m_tag{"None"}, m_transform{nullptr},
    m_is_static{false}, m_is_visible{true},
    m_scene{scene}, m_components{}, m_mesh{}
{ }

GameObject::GameObject(const String& name, Scene* scene, const String& tag, Object* parent)
    : Object(name, parent), m_tag{tag}, m_transform{nullptr},
      m_is_static{false}, m_is_visible{true},
      m_scene{scene}, m_components{}, m_mesh{}
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

AssetHandle GameObject::meshHandle() const
{
  return m_mesh;
}

Scene* GameObject::scene() const
{
  return m_scene;
}

void GameObject::addComponent(AbstractComponent* component)
{
  if (!m_components.contains(component)) {
    m_components.push_back(component);
  }
}

const Array<AbstractComponent*>& GameObject::getComponents() const
{
  return m_components;
}

Array<AbstractComponent*>& GameObject::getComponents()
{
  return m_components;
}