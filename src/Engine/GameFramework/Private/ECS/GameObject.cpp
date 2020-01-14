#include <GameFramework/Public/ECS/GameObject.hpp>
#include <GameFramework/Public/Components/Transform.hpp>

#include <utility>

GameObject::GameObject(const String& name, Scene* scene, Object* parent)
  : Object(name, parent), m_tag{"None"}, m_transform{nullptr},
    m_is_static{false}, m_is_visible{true}, m_is_simulated{false},
    m_has_collider{false}, m_scene{scene},
    m_components{}
{ }

GameObject::GameObject(const String& name, Scene* scene, const String& tag, Object* parent)
    : Object(name, parent), m_tag{tag}, m_transform{nullptr},
      m_is_static{false}, m_is_visible{true}, m_is_simulated{false},
      m_has_collider{false}, m_scene{scene},
      m_components{}
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

void GameObject::addComponent(Component* component)
{
  if (!m_components.contains(component)) {
    m_components.push_back(component);

    if (component->typeID() == Component::family::type<Transform>) {
      m_transform = dynamic_cast<Transform*>(component);
    }
  }
}

const Array<Component*>& GameObject::getComponents() const
{
  return m_components;
}

Array<Component*>& GameObject::getComponents()
{
  return m_components;
}

bool GameObject::hasCollider() const
{
  return m_has_collider;
}

void GameObject::setHasCollider(bool hasCollider)
{
  m_has_collider = hasCollider;
}

bool GameObject::isSimulated() const
{
  return m_is_simulated;
}

void GameObject::setIsSimulated(bool isSimulated)
{
  m_is_simulated = isSimulated;
}

void GameObject::setToBeDestroyed(bool destroyed)
{
  m_to_be_destroyed = destroyed;
}

bool GameObject::isToBeDestroyed() const
{
  return m_to_be_destroyed;
}
