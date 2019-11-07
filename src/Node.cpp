#include "Node.hpp"

NameGenerator Node::m_name_generator(String("Node"));

Node::Node(Node* parent /* = nullptr */)
  : m_parent{parent}, m_children{}, m_orientation{quat::Identity},
    m_position{vec3::Zero}, m_scale{1.0f, 1.0f, 1.0f},
    m_combined_position{vec3::Zero},
    m_combined_scale{vec3::Zero},
    m_combined_orientation{quat::Identity},
    m_is_inherited_orientation{false},
    m_is_inherited_scale{false},
    m_is_inherited_position{false}
{
  m_name = m_name_generator.generate();
  if (m_parent) m_parent->addChild(this);
}

Node::Node(const String& name, Node* parent /* = nullptr */)
  : m_parent{parent}, m_children{}, m_orientation{quat::Identity},
    m_position{vec3::Zero}, m_scale{1.0f, 1.0f, 1.0f},
    m_combined_position{vec3::Zero},
    m_combined_scale{vec3::Zero},
    m_combined_orientation{quat::Identity},
    m_is_inherited_orientation{false},
    m_is_inherited_scale{false},
    m_is_inherited_position{false}
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

void Node::setInheritedTransformation(bool inheritedScale, bool inheritedPosition, bool inheritedOrientation)
{
  m_is_inherited_position = inheritedPosition;
  m_is_inherited_orientation = inheritedOrientation;
  m_is_inherited_scale = inheritedScale;
}

bool Node::isScaleInherited() const
{
  return m_is_inherited_scale;
}

bool Node::isPositionInherited() const
{
  return m_is_inherited_position;
}

bool Node::isOrientationInherited() const
{
  return m_is_inherited_orientation;
}

void Node::setOrientation(const Quat& q)
{
  m_orientation = q.normalized();
}

void Node::setPosition(const Vec3& pos)
{
  m_position = pos;
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
        Quat orientation_parent = m_parent->combinedOrientation().inverted();
        Vec3 tt = orientation_parent * t;
        m_position += tt - m_parent->combinedScale();
      } else {
        m_position += t;
      }
    } break;
  }
  update(true);
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
      const Quat& combined = combinedOrientation();
      m_orientation = m_orientation * combined.inverted() * q.normalized() * combined;
    } break;
  }
  update(true);
  //_update_local_matrix();
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

const Vec3& Node::worldPosition() const
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

const Vec3& Node::localPosition() const
{
  return vec3::Zero;
}

const Quat& Node::localOrientation() const
{
  return quat::Identity;
}

const Vec3& Node::localScale() const
{
  return m_scale;
}

const Vec3& Node::position() const
{
  return m_position;
}

const Quat& Node::orientation() const
{
  return m_orientation;
}

const Vec3& Node::scale() const
{
  return m_scale;
}

void Node::_update()
{
  //_update_local_matrix();
  //_update_world_matrix();
  //_update_from_parent();
}

void Node::update(bool isUpdateChildren) {
  _update_from_parent();
  _update_local_matrix();
  _update_world_matrix();
  if (isUpdateChildren) {
    for (auto i = m_children.begin(); i != m_children.end(); ++i) {
      i.value()->update(isUpdateChildren);
    }
  }
}

const Quat& Node::combinedOrientation()
{
  _update_from_parent();
  return m_combined_orientation;
}

const Vec3& Node::combinedPosition()
{
  _update_from_parent();
  return m_combined_position;
}

const Vec3& Node::combinedScale()
{
  _update_from_parent();
  return m_combined_scale;
}

void Node::_update_from_parent()
{
  //qDebug() << "Update from parent";
  if (m_parent) {
    //qDebug() << m_name << "update from parent : with parent";
    // update orientation
    const Quat& parent_orientation = m_parent->combinedOrientation();
    if (m_is_inherited_orientation) {
      m_combined_orientation = parent_orientation * m_orientation;
      //qDebug() << m_name << "In --- O" << m_orientation << "pcO" << parent_orientation << "mcO" << m_combined_orientation;
    } else {
      m_combined_orientation = m_orientation;
      //qDebug() << m_name << "No --- O" << m_orientation << "pcO" << parent_orientation << "mcO" << m_combined_orientation;
    }

    // update scale
    const Vec3& parent_scale = m_parent->combinedScale();
    if (m_is_inherited_scale) {
      // component-wise multiplication
      m_combined_scale = parent_scale * m_scale;
    } else {
      m_combined_scale = m_scale;
    }

    //m_combined_position = parent_orientation * parent_scale * m_position;
    m_combined_position = m_combined_orientation * m_combined_scale * m_position;
    //qDebug() << m_name << "parent cP: " << m_parent->combinedPosition();
    //qDebug() << "my cP" << m_combined_position;
    m_combined_position += m_parent->combinedPosition();
    //qDebug() << "my cP final" << m_combined_position;

  } else {
    //qDebug() << m_name << "update from parent : no parent";
    //qDebug() << "P:" << m_position << "O: " << m_orientation << "Scale: " << m_scale;
    //qDebug() << "cP:" << m_combined_position << "cO: " << m_combined_orientation << "cS: " << m_combined_scale;
    m_combined_position = m_position;
    m_combined_orientation = m_orientation;
    m_combined_scale = m_scale;
  }
}

Vec3 Node::localToWorldPosition(const Vec3& localPos)
{
  _update_from_parent();
  return m_combined_orientation * localPos * m_combined_scale + m_combined_position;
}

Vec3 Node::worldToLocalPosition(const Vec3& worldPos)
{
  _update_from_parent();
  return (m_combined_orientation.inverted() * m_combined_position) / m_combined_scale;
}

Quat Node::localToWorldOrientation(const Quat& localOrientation)
{
  _update_from_parent();
  return m_combined_orientation * localOrientation;
}

Quat Node::worldToLocalOrientation(const Quat& worldOrientation)
{
  _update_from_parent();
  return m_combined_orientation.inverted() * worldOrientation;
}

void Node::_update_world_matrix()
{
  if (m_parent) {
    m_world_matrix = m_parent->worldMatrix() * m_local_matrix;
  } else {
    m_world_matrix = m_local_matrix;
  }

  for (auto& child : m_children) {
    child->_update_world_matrix();
  }
}

void Node::_update_local_matrix()
{
  // position, quaternion, scale
  /*
  Real x = m_orientation.x(), y = m_orientation.y(), z = m_orientation.z(), w = m_orientation.scalar();
  Real x2 = x + x, y2 = y + y, z2 = z + z;
  Real xx = x * x2, xy = x * y2, xz = x * z2;
  Real yy = y * y2, yz = y * z2, zz = z * z2;
  Real wx = w * x2, wy = w * y2, wz = w * z2;
  Real sx = m_scale.x(), sy = m_scale.y(), sz = m_scale.z();
  Vec4 row0 = Vec4((1-(yy+zz))*sx, (xy+wz)*sx, (xz-wy)*sx, 0);
  Vec4 row1 = Vec4((xy-wz)*sy, (1-(xx+zz))*sy, (yz+wx)*sy, 0);
  Vec4 row2 = Vec4((xz+wy)*sz, (yz-wx)*sz, (1-(xx+yy))*sz, 0);
  Vec4 row3 = Vec4(m_position.x(), m_position.y(), m_position.z(), 1);
  m_local_matrix.setRow(0, row0);
  m_local_matrix.setRow(1, row1);
  m_local_matrix.setRow(2, row2);
  m_local_matrix.setRow(3, row3);
   */
  Mat4 T; T.setToIdentity();
  T.translate(m_position);
  Mat4 R; R.setToIdentity();
  R.rotate(m_orientation);
  Mat4 S; S.setToIdentity();
  S.scale(m_scale);
  m_local_matrix = T * R * S;
}

const Mat4& Node::worldMatrix()
{
  _update_world_matrix();
  return m_world_matrix;
}

const Mat4& Node::localMatrix()
{
  _update_local_matrix();
  return m_local_matrix;
}