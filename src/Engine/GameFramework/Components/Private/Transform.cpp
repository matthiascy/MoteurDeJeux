#include <GameFramework/Components/Transform.hpp>
#include <GameFramework/GameObject.hpp>

Transform::Transform(const String& name, GameObject* gameObject, Transform* parent, const Vec3& localPosition, const Quat& localRotation, const Vec3& localScale)
  : Component<Transform>(name, gameObject),
    m_position{}, m_rotation{}, m_scale{},
    m_local_position{localPosition}, m_local_rotation{localRotation}, m_local_scale{localScale},
    m_local_to_world_matrix{}, m_world_to_local_matrix{},
    m_has_changed{false}, m_parent{parent}, m_children{}
{
  if (m_parent) {
    m_position = m_parent->position() + m_local_position;
    m_rotation = m_local_rotation * m_parent->rotation();
    m_scale = m_parent->scale() * m_local_scale;
  } else {
    m_position = m_local_position;
    m_rotation = m_local_rotation;
    m_scale = m_local_scale;
    m_local_position = Math::Zero;
    m_local_rotation = Math::QuatIdentity;
    m_local_scale = Math::mkVec3(1);
  }
  instant_update();
}

inline const Vec3& Transform::position()
{
  if (m_has_changed) {
    instant_update();
    m_has_changed = false;
  }
  return m_position;
}

void Transform::setPosition(const Vec3& position)
{
  if (m_has_changed) {
    instant_update();
    m_has_changed = false;
  }
  m_position = position;
  if (m_parent) {
    m_local_position = m_position - m_parent->position();
  } else {
    m_local_position = Math::Zero;
  }
  set_has_changed();
}

inline const Vec3& Transform::localPosition()
{
  if (m_has_changed) {
    instant_update();
    m_has_changed = false;
  }
  return m_local_position;
}

void Transform::setLocalPosition(const Vec3& position)
{
  if (m_has_changed) {
    instant_update();
    m_has_changed = false;
  }
  m_local_position = position;
  if (m_parent) {
    m_position = m_parent->position() + m_local_position;
  } else {
    m_position = m_local_position;
    m_local_position = Math::Zero;
  }
  set_has_changed();
}

inline const Vec3& Transform::scale()
{
  if (m_has_changed) {
    instant_update();
    m_has_changed = false;
  }
  return m_scale;
}

void Transform::setScale(const Vec3 & scale)
{
  if (m_has_changed) {
    instant_update();
    m_has_changed = false;
  }
  m_scale = scale;
  if (m_parent) {
    m_local_scale = m_scale / m_parent->scale();
  } else {
    m_local_scale = Math::mkVec3(1);
  }
  set_has_changed();
}

inline const Vec3& Transform::localScale()
{
  if (m_has_changed) {
    instant_update();
    m_has_changed = false;
  }
  return m_local_scale;
}

void Transform::setLocalScale(const Vec3 & scale)
{
  if (m_has_changed) {
    instant_update();
    m_has_changed = false;
  }
  m_local_scale = scale;
  if (m_parent) {
    m_scale = m_parent->scale() * m_local_scale;
  } else {
    m_scale = m_local_scale;
    m_local_scale = Math::mkVec3(1);
  }
  set_has_changed();
}

inline const Quat& Transform::rotation()
{
  if (m_has_changed) {
    instant_update();
    m_has_changed = false;
  }
  return m_rotation;
}

void Transform::setRotation(const Quat& rotation)
{
  if (m_has_changed) {
    instant_update();
    m_has_changed = false;
  }
  m_rotation = rotation;
  // Qr = Ql * Qp, Qr * Qp.inverted = Ql * Qp * Qp.inverted
  if (m_parent) {
    m_local_rotation = m_rotation * m_parent->m_rotation.inverted();
  } else {
    m_local_rotation = Math::QuatIdentity;
  }
  set_has_changed();
}

inline const Quat& Transform::localRotation()
{
  if (m_has_changed) {
    instant_update();
    m_has_changed = false;
  }
  return m_local_rotation;
}

void Transform::setLocalRotation(const Quat& rotation)
{
  if (m_has_changed) {
    instant_update();
    m_has_changed = false;
  }
  m_local_rotation = rotation;

  if (m_parent) {
    m_rotation = m_local_rotation * m_parent->rotation();
  } else {
    m_rotation = m_local_rotation;
    m_local_rotation = Math::QuatIdentity;
  }
  set_has_changed();
}

Vec3 Transform::forward()
{
  if (m_has_changed) {
    instant_update();
    m_has_changed = false;
  }
  return m_rotation * Math::Forward;
}

Vec3 Transform::up()
{
  if (m_has_changed) {
    instant_update();
    m_has_changed = false;
  }
  return m_rotation * Math::Up;
}

Vec3 Transform::right()
{
  if (m_has_changed) {
    instant_update();
    m_has_changed = false;
  }
  return m_rotation * Math::Right;
}

Mat4 Transform::localToWorldMatrix()
{
  if (m_has_changed) {
    // Re-calculate
    instant_update();
    m_has_changed = false;
  }

  return m_local_to_world_matrix;
}

Mat4 Transform::worldToLocalMatrix()
{
  if (m_has_changed) {
    instant_update();
    m_has_changed = false;
  }
  return m_world_to_local_matrix;
}

Transform* Transform::parent() const
{
  return m_parent;
}

void Transform::setParent(Transform* parent)
{
  if (m_parent != parent) {
    if (m_parent) {
      auto& children = m_parent->m_children;
      children.erase(std::remove(children.begin(), children.end(), this));
    }

    m_parent = parent;

    if (parent) {
      parent->m_children.push_back(this);
    }
  }
  set_has_changed();
}

Transform* Transform::root()
{
  if (m_parent) {
    return m_parent->root();
  } else {
    return this;
  }
}

void Transform::rotate(const Quat& q)
{
  // TODO
}

void Transform::scale(const Vec3& s)
{
  // TODO
}

void Transform::translate(const Vec3& t)
{
  // TODO
}

void Transform::rotate(const Quat& q, ESpace relativeTo)
{
  if (m_has_changed) {
    instant_update();
    m_has_changed = false;
  }
  switch (relativeTo) {
    case ESpace::Local: {
      m_local_rotation = q.normalized() * m_rotation;
      m_rotation = m_local_rotation * m_parent->localRotation();
    } break;

    case ESpace::World: {
      rotate(q);
    } break;
  }
  set_has_changed();
}

void Transform::scale(const Vec3& s, ESpace relativeTo)
{
  if (m_has_changed) {
    instant_update();
    m_has_changed = false;
  }
  switch (relativeTo) {
    // TODO
    case ESpace::Local: {

    } break;

    case ESpace::World: {
      scale(s);
    } break;
  }
  set_has_changed();
}

void Transform::translate(const Vec3& t, ESpace relativeTo)
{
  if (m_has_changed) {
    instant_update();
    m_has_changed = false;
  }
  switch (relativeTo) {
    // TODO
    case ESpace::Local: {

    } break;

    case ESpace::World: {
      translate(t);
    } break;
  }
  set_has_changed();
}

void Transform::lookAt(const Vec3& worldPos, const Vec3& worldUp)
{
  setLocalRotation(Math::lookAtQuaternion(localPosition(), worldPos, worldUp));
}

void Transform::detachChildren()
{
  for (auto* child : m_children) {
    child->setParent(m_parent);
  }
}

Transform* Transform::find(const String& name)
{
  for (auto* child : m_children) {
    if (child->gameObject()->objectName() == name) {
      return child;
    }
  }
  return nullptr;
}

bool Transform::isChildOf(Transform* parent) const
{
  return m_parent == parent;
}

Transform* Transform::child(Int32 index)
{
  if (index >= m_children.size()) {
    // TODO log
    return nullptr;
  }
  return m_children[index];
}

UInt32 Transform::childCount() const
{
  return m_children.size();
}

void Transform::set_has_changed()
{
  if (!m_has_changed) {
    m_has_changed = true;
    for (auto* child : m_children) {
      child->set_has_changed();
    }
  }
}

void Transform::instant_update()
{
  if (m_parent) {
    m_rotation = m_local_rotation * m_parent->rotation();
    m_position = m_local_position + m_parent->position();
    m_scale = m_local_scale * m_parent->scale();
  } else {
    m_rotation = m_local_rotation;
    m_position = m_local_position;
    m_scale = m_local_scale;
    m_local_rotation = Math::QuatIdentity;
    m_local_position = Math::Zero;
    m_local_scale = Math::mkVec3(1);
  }
  m_local_to_world_matrix.setToIdentity();
  m_local_to_world_matrix.scale(m_scale);
  m_local_to_world_matrix.rotate(m_rotation);
  m_local_to_world_matrix.translate(m_position);
  m_world_to_local_matrix = m_local_to_world_matrix.inverted();

  if (!m_children.isEmpty()) {
    for (auto* child : m_children) {
      child->instant_update();
    }
  }
}

const Array<Transform*>& Transform::children() const
{
  return m_children;
}

Array<Transform*>& Transform::children()
{
  return m_children;
}
