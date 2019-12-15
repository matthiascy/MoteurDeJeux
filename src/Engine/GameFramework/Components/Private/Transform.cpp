#include <GameFramework/Components/Transform.hpp>
#include <GameFramework/GameObject.hpp>

Transform::Transform(const String& name, GameObject* gameObject, Transform* parent, const Vec3& localPosition, const Quat& localRotation, const Vec3& localScale)
  : Component<Transform>(name, gameObject),
    m_local_position{localPosition}, m_local_rotation{localRotation}, m_local_scale{localScale},
    m_local_to_world_matrix{}, m_world_to_local_matrix{},
    m_parent{parent}, m_children{}
{
  _update();
}

Vec3 Transform::worldPosition() const
{
  return Math::extractTranslation(m_world_matrix);
}

Vec3 Transform::localPosition() const
{
  return m_local_position;
}

Quat Transform::worldRotation() const
{
  return Math::extractRotation(m_world_matrix);
}

Quat Transform::localRotation() const
{
  return m_local_rotation;
}

Vec3 Transform::worldScale() const
{
  return Math::extractScale(m_world_matrix);
}

Vec3 Transform::localScale() const
{
  return m_local_scale;
}

Mat4 Transform::worldMatrix() const
{
  return m_world_matrix;
}

Mat4 Transform::localMatrix() const
{
  return m_local_matrix;
}

Vec3 Transform::right() const
{
  return Math::extractAxisX(m_world_matrix).normalized();
  //return Vec3{worldMatrix().column(0)}.normalized();
  //return (m_world_rotation * Math::Right).normalized();
}

Vec3 Transform::up() const
{
  return Math::extractAxisY(m_world_matrix).normalized();
  //return Vec3{worldMatrix().column(1)}.normalized();
  //return (m_world_rotation * Math::Up).normalized();
}

Vec3 Transform::forward() const
{
  return Math::extractAxisZ(m_world_matrix).normalized();
  //return Vec3{worldMatrix().column(2)}.normalized();
  //return (m_world_rotation * Math::Forward).normalized();
}

Mat4 Transform::localToWorldMatrix() const
{
  return m_local_to_world_matrix;
}

Mat4 Transform::worldToLocalMatrix() const
{
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

  _update();
}

Transform* Transform::root()
{
  if (m_parent) {
    return m_parent->root();
  } else {
    return this;
  }
}

UInt32 Transform::depth() const
{
  return m_depth;
}

void Transform::rotate(const Quat& q, ESpace relativeTo)
{
  switch (relativeTo) {
    case ESpace::Local: {
      m_local_rotation = m_local_rotation * q;
    } break;

    case ESpace::World: {
      //if (m_parent) {
      //Quat inv_parent = parent()->worldRotation().inverted();
      //m_local_rotation = (inv_parent * q) * worldRotation();
      //} else {
      m_local_rotation = q * m_local_rotation;
      //}
    } break;
  }

  _update();
}

void Transform::rotate(const EulerAngles& eulerAngles, ESpace relativeTo)
{
  rotate(Quat::fromEulerAngles(eulerAngles), relativeTo);
}

void Transform::rotate(const Vec3& axis, Real angle, ESpace relativeTo)
{
  rotate(Quat::fromAxisAndAngle(axis, angle), relativeTo);
}

void Transform::rotateAround(const Vec3& target, const Vec3& axis, float angle)
{
  rotateAround(target, Quat::fromAxisAndAngle(axis, angle));
}

void Transform::rotateAround(const Vec3& target, const Quat& q)
{
  qFatal("Not implemented");
}

void Transform::rotateAround(const Vec3& target, EulerAngles& eulerAngles)
{
  rotateAround(target, Quat::fromEulerAngles(eulerAngles));
}

void Transform::setRotation(const Quat& q, ESpace relativeTo)
{
  switch (relativeTo) {
    case ESpace::Local: {
      m_local_rotation = q;
    } break;

    case ESpace::World: {
      if (!m_parent) {

        m_local_rotation = q;

      } else {

        Quat inv_parent = parent()->worldRotation().inverted();
        m_local_rotation = inv_parent * q;

      }
    } break;
  }

  _update();
}

void Transform::setRotation(const EulerAngles& eulerAngles, ESpace relativeTo)
{
  setRotation(Quat::fromEulerAngles(eulerAngles), relativeTo);
}

void Transform::setLocalScale(const Vec3& s)
{
  m_local_scale = s;

  _update();
}

void Transform::translate(const Vec3& t, ESpace relativeTo)
{
  switch (relativeTo) {
    case ESpace::Local: {
      m_local_position += m_local_rotation.rotatedVector(t);
    } break;

    case ESpace::World: {
      setPosition(t + worldPosition(), ESpace::World);
    } break;
  }

  _update();
}

void Transform::setPosition(const Vec3& p, ESpace relativeTo)
{
  switch (relativeTo) {
    case ESpace::Local: {
      m_local_position = worldRotation() * p;
    } break;

    case ESpace::World: {
      if (!m_parent) {
        m_local_position = p;
      } else {
        Mat4 inv_parent = parent()->worldMatrix().inverted();
        m_local_position = inv_parent * p;
      }
    } break;
  }

  _update();
}

void Transform::lookAt(Transform* target, const Vec3 &worldUp)
{
  lookAt(target->worldPosition(), worldUp);
}

void Transform::lookAt(const Vec3& worldPos, const Vec3& worldUp)
{
  Mat4 mat = Math::lookAtMatrix(worldPosition(), worldPos, worldUp);
  m_local_rotation = Math::extractRotation(mat);
  if (m_parent) {
    Quat q = Math::extractRotation(mat);
    m_local_rotation = q.inverted() * m_local_rotation;
  }
  //setRotation(Math::lookAtQuaternion(worldPosition(), worldPos, worldUp), ESpace::World);
}

Vec3 Transform::transformLocalPositionToWorld(const Vec3& pos) const
{
  return m_local_to_world_matrix * pos;
}

Vec3 Transform::transformWorldPositionToLocal(const Vec3& pos) const
{
  return m_world_to_local_matrix * pos;
}

void Transform::detachChildren()
{
  for (auto* child : m_children) {
    child->setParent(m_parent);
  }
}

Transform* Transform::find(const String& name) const
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

Transform* Transform::childAt(Int32 index) const
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

void Transform::_update()
{
  //m_local_matrix = Math::mat4FromRTS(m_local_position, m_local_rotation, m_local_scale);
  m_local_matrix = Math::mat4FromSRT(m_local_position, m_local_rotation, m_local_scale);

  if (m_parent) {

    m_world_matrix = parent()->worldMatrix() * m_local_matrix;

  } else {

    m_world_matrix = m_local_matrix;

  }

  if (!m_children.isEmpty()) {
    for (auto* child : m_children) {
      child->_update();
    }
  }
}

const Array<Transform*>& Transform::children() const
{
  return m_children;
}
