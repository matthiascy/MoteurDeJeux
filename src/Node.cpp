#include "Node.hpp"

NameGenerator Node::m_name_generator(String("Node"));

Node::Node(Node* parent /* = nullptr */)
  : m_parent{parent}, m_children{}, m_orientation{quat::Identity},
    m_position{vec3::Zero}, m_scale{1.0f, 1.0f, 1.0f},
    m_is_inherited_orientation{true}, m_is_inherited_scale(false)
{
  m_name = m_name_generator.generate();
  if (m_parent) m_parent->addChild(this);
}

Node::Node(const String& name, Node* parent /* = nullptr */)
  : m_parent{parent}, m_children{}, m_orientation{quat::Identity},
    m_position{vec3::Zero}, m_scale{1.0f, 1.0f, 1.0f},
    m_is_inherited_orientation{true}, m_is_inherited_scale(false)
{
  m_name = name;
  if (m_parent) m_parent->addChild(this);
}

Node::~Node()
{
  removeAllChildren();
  if(m_parent) {
    m_parent->removeChild(this);
  }
}

void Node::setParent(Node* parent)
{
  if (parent != this) {
    m_parent = parent;
    parent->addChild(this);
  }
}

Node* Node::parent() const
{
  return m_parent;
}

const Quat& Node::orientation() const
{
  return m_orientation;
}

const Quat& Node::combinedOrientation() const
{
  return m_combined_orientation;
}

void Node::setOrientation(const Quat& q)
{
  m_orientation = q.normalized();
}

const Vec3& Node::position() const
{
  return m_position;
}

const Vec3& Node::combinedPosition() const
{
  return m_combined_position;
}

void Node::setPosition(const Vec3& pos)
{
  m_position = pos;
}

const Vec3& Node::scale() const
{
  return m_scale;
}

const Vec3& Node::combinedScale() const
{
  return m_combined_scale;
}

void Node::setScale(const Vec3& scale)
{
  m_scale = scale;
}

void Node::translate(const Vec3& t, ETransformSpace relativeTo)
{
  switch (relativeTo) {
    case ETransformSpace::Local: {
      m_position += m_orientation * t;
    } break;

    case ETransformSpace::Parent: {
      m_position += t;
    } break;

    case ETransformSpace::World: {
      if (m_parent) {
        // TODO::
        //Quat orientation_parent = m_parent->orientation().inverted();
        //Vec3 tt = orientation_parent * t;
        //m_position += tt - m_parent->scale();
      } else {
        m_position += t;
      }
    } break;
  }
}

void Node::rotate(const Vec3& axis, float angle, ETransformSpace relativeTo)
{
  rotate(Quat::fromAxisAndAngle(axis, angle), relativeTo);
}

void Node::rotate(const Quat& q, ETransformSpace relativeTo)
{
  switch (relativeTo) {
      case ETransformSpace::Local: {
        m_orientation = q.normalized() * m_orientation;
      } break;

      case ETransformSpace::Parent: {
        m_orientation = m_orientation * q.normalized();
      } break;

      case ETransformSpace::World: {
        // TODO::
        //m_orientation = m_orientation *
      } break;
    }
}

void Node::scale(const Vec3& s, ETransformSpace relativeTo)
{
  // TODO update children
  switch (relativeTo) {
    case ETransformSpace::Local: {
      m_scale = s;
    } break;

    case ETransformSpace::Parent: {
      // TODO
    } break;

    case ETransformSpace::World: {
      // TODO
    } break;
  }
}

Node* Node::child(const String& name) const
{
  ConstChildNodeIterator i = m_children.find(name);
  if (i != m_children.end()) {
    return i.value();
  } else {
    return nullptr;
  }
}

Node* Node::child(UInt32 index) const
{
  if (static_cast<Int32>(index) < m_children.count()) {
    ConstChildNodeIterator i = m_children.begin();
    while (index--) ++i;
    return i.value();
  } else {
    return nullptr;
  }
}

bool Node::hasChildren() const
{
return (m_children.count() > 0);
}

UInt32 Node::childCount() const
{
  return static_cast<UInt32>(m_children.count());
}

Node* Node::removeChild(const String& name)
{
  ConstChildNodeIterator i = m_children.find(name);
  if (i != m_children.end()) {
    Node* node = i.value();
    m_children.erase(i);
    node->setParent(nullptr);
    return node;
  } else {
    return nullptr;
  }
}

Node* Node::removeChild(UInt32 index)
{
  if (static_cast<Int32>(index) < m_children.count()) {
    ConstChildNodeIterator i = m_children.begin();
    while (index--) ++i;
    Node* node = i.value();
    m_children.erase(i);
    node->setParent(nullptr);
    return node;
  } else {
    return nullptr;
  }
}

Node* Node::removeChild(Node *child)
{
  if (child) {
    ConstChildNodeIterator i = m_children.find(child->name());
    if (i != m_children.end() && i.value() == child) {
      m_children.erase(i);
      child->setParent(nullptr);
    }
  }

  return child;
}

void Node:: removeAllChildren()
{
  for (auto i = m_children.begin(); i != m_children.end(); ++i) {
    i.value()->setParent(nullptr);
  }
  m_children.clear();
}

void Node::addChild(Node *child)
{
  if (child->parent()) {
    // TODO log error
  } else {
    m_children.insert(child->name(), child);
    child->setParent(this);
  }
}

Node* Node::createChild(const Vec3& translation, const Quat& rotation)
{
  Node* node = new Node(this);
  node->translate(translation, ETransformSpace::Parent);
  node->rotate(rotation, ETransformSpace::Local);
  this->addChild(node);
  return node;
}

Node* Node::createChild(const String& name, const Vec3& translation, const Quat& rotation)
{
  Node* node = new Node(name, this);
  node->translate(translation, ETransformSpace::Parent);
  node->rotate(rotation, ETransformSpace::Local);
  this->addChild(node);
  return node;
}

Vec3 Node::convertWorldPositionToLocalPosition(const Vec3& pos)
{
  return (m_combined_orientation.inverted() * m_combined_position) / m_combined_scale;
}

Vec3 Node::convertLocalPositionToWorldPosition(const Vec3& pos)
{
  // TODO
  return vec3::Zero * pos;
}

Quat Node::convertWorldOrientationToLocalOrientation(const Quat& q)
{
  // TODO
  return quat::Identity * q;
}

Quat Node::convertLocalOrientationToLocalOrientation(const Quat& q)
{
  // TODO
  return quat::Identity * q;
}

const Vec3& Node::worldPoistion() const
{
  return vec3::Zero;
}

const Quat& Node::worldOrientation() const
{
  return quat::Identity;
}

const Vec3& Node::worldScale() const
{
  return vec3::Zero;
}

void Node::_update()
{ }

void Node::update(bool isUpdateChildren) {
  _update();
  if (isUpdateChildren) {
    for (auto i = m_children.begin(); i != m_children.end(); ++i) {
      i.value()->update(isUpdateChildren);
    }
  }
}
