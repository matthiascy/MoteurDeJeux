#include <GameFramework/Components/Transform.hpp>
#include <GameFramework/GameObject.hpp>

Transform::Transform(const String& name, GameObject* gameObject, Transform* parent, const Vec3& localPosition, const Quat& localRotation, const Vec3& localScale)
  : Component<Transform>(name, gameObject),
    m_world_position{}, m_world_rotation{}, m_world_scale{},
    m_local_position{localPosition}, m_local_rotation{localRotation}, m_local_scale{localScale},
    m_local_to_world_matrix{}, m_world_to_local_matrix{},
    m_has_changed{false}, m_parent{parent}, m_children{}
{
  instant_update();
}

Vec3 Transform::worldPosition()
{
  if (m_has_changed) {
    instant_update();
    m_has_changed = false;
  }
  return m_world_position;
}

Vec3 Transform::localPosition()
{
  if (m_has_changed) {
    instant_update();
    m_has_changed = false;
  }
  return m_local_position;
}

Quat Transform::worldRotation()
{
  if (m_has_changed) {
    instant_update();
    m_has_changed = false;
  }
  return m_world_rotation;
}

Quat Transform::localRotation()
{
  if (m_has_changed) {
    instant_update();
    m_has_changed = false;
  }
  return m_local_rotation;
}

Vec3 Transform::worldScale()
{
  if (m_has_changed) {
    instant_update();
    m_has_changed = false;
  }
  return m_world_scale;
}

Vec3 Transform::localScale()
{
  if (m_has_changed) {
    instant_update();
    m_has_changed = false;
  }
  return m_local_scale;
}

Vec3 Transform::forward()
{
  if (m_has_changed) {
    instant_update();
    m_has_changed = false;
  }
  return (m_world_rotation * Math::Forward).normalized();
}

Vec3 Transform::up()
{
  if (m_has_changed) {
    instant_update();
    m_has_changed = false;
  }
  return (m_world_rotation * Math::Up).normalized();
}

Vec3 Transform::right()
{
  if (m_has_changed) {
    instant_update();
    m_has_changed = false;
  }
  return (m_world_rotation * Math::Right).normalized();
}

Mat4 Transform::localToWorldMatrix()
{
  if (m_has_changed) {
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

void Transform::rotate(const Quat& q, ESpace relativeTo)
{
  if (m_has_changed) {
    instant_update();
    m_has_changed = false;
  }

  switch (relativeTo) {
    case ESpace::Local: {
      //m_local_rotation = q.normalized() * m_local_rotation;
      //m_world_rotation = m_local_rotation * m_parent->localRotation();
      // TODO::
    } break;

    case ESpace::World: {
      // TODO::
    } break;
  }
  set_has_changed();
}

void Transform::rotate(Real xAngle, Real yAngle, Real zAngle, ESpace relativeTo)
{
  rotate(Quat::fromEulerAngles(xAngle, yAngle, zAngle), relativeTo);
}

void Transform::scale(const Vec3& s, ESpace relativeTo)
{
  if (m_has_changed) {
    instant_update();
    m_has_changed = false;
  }
  switch (relativeTo) {
    case ESpace::Local: {
      m_local_scale = s;
    } break;

    case ESpace::World: {
      if (m_parent)
        m_local_scale = s / m_parent->worldScale();
      else
        m_local_scale = s;
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
    // TODO::
    case ESpace::Local: {

    } break;

    case ESpace::World: {
      translate(t);
    } break;
  }
  set_has_changed();
}

void Transform::rotateAround(const Vec3 &point, const Vec3 &axis, Real angle)
{
  // TODO::
}

void Transform::lookAt(const Transform &target, const Vec3 &worldUp)
{
  // TODO::
}

void Transform::lookAt(const Vec3& worldPos, const Vec3& worldUp)
{
  // TODO::
  setLocalRotation(Math::lookAtQuaternion(localPosition(), worldPos, worldUp));
}

Vec3 Transform::transformLocalPositionToWorld(const Vec3 &pos)
{
  // TODO::
}

Vec3 Transform::transformWorldPositionToLocal(const Vec3 &pos)
{
  // TODO::
  return Vec3();
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
  if (m_parent == nullptr)
    return false;
  else
    return m_parent == parent;
}

Transform* Transform::childAt(Int32 index)
{
  if (index >= m_children.size()) {
    qWarning("Index greater than the child count.");
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

    m_world_rotation = m_local_rotation * m_parent->worldRotation();
    m_world_position = m_local_position + m_parent->worldPosition();
    m_world_scale    = m_local_scale    * m_parent->worldScale();

    m_local_to_world_matrix.setToIdentity();
    m_local_to_world_matrix.scale(m_world_scale);
    m_local_to_world_matrix.rotate(m_world_rotation);
    m_local_to_world_matrix.translate(m_world_position);
    m_local_to_world_matrix = m_parent->localToWorldMatrix() * m_local_to_world_matrix;
    m_world_to_local_matrix = m_local_to_world_matrix.inverted();

  } else {

    m_world_rotation = m_local_rotation;
    m_world_position = m_local_position;
    m_world_scale    = m_local_scale;
    m_local_rotation = Math::QuatIdentity;
    m_local_position = Math::Zero;
    m_local_scale    = Math::mkVec3(1);

    m_local_to_world_matrix.setToIdentity();
    m_local_to_world_matrix.scale(m_world_scale);
    m_local_to_world_matrix.rotate(m_world_rotation);
    m_local_to_world_matrix.translate(m_world_position);
    m_world_to_local_matrix = m_local_to_world_matrix.inverted();
  }

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
