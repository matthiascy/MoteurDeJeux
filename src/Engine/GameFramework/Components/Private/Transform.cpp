#include <GameFramework/Components/Transform.hpp>
#include <GameFramework/GameObject.hpp>

Transform::Transform(const String& name, GameObject* gameObject, Transform* parent, const Vec3& localPosition, const Quat& localRotation, const Vec3& localScale)
  : Component<Transform>(name, gameObject),
    m_world_position{}, m_world_rotation{}, m_world_scale{},
    m_local_position{localPosition}, m_local_rotation{localRotation}, m_local_scale{localScale},
    m_local_to_world_matrix{}, m_world_to_local_matrix{},
    m_parent{parent}, m_children{}
{
  QObject::connect(&m_private_signal, &TransformPrivateSignal::needToUpdate, [this](){
    _update();
  });
  _update();
}

Vec3 Transform::worldPosition() const
{
  return m_world_position;
}

Vec3 Transform::localPosition() const
{
  return m_local_position;
}

Quat Transform::worldRotation() const
{
  return m_world_rotation;
}

Quat Transform::localRotation() const
{
  return m_local_rotation;
}

Vec3 Transform::worldScale() const
{
  return m_world_scale;
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
  return Vec3{worldMatrix().column(0)}.normalized();
  //return (m_world_rotation * Math::Right).normalized();
}

Vec3 Transform::up() const
{
  return Vec3{worldMatrix().column(1)}.normalized();
  //return (m_world_rotation * Math::Up).normalized();
}

Vec3 Transform::forward() const
{
  return Vec3{worldMatrix().column(2)}.normalized();
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

  emit m_private_signal.needToUpdate();
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

Vec3 Transform::worldEulerAngles() const
{
  Vec3 out;
  m_world_rotation.getEulerAngles(&out[0], &out[1], &out[2]);
  return out;
}

Vec3 Transform::localEulerAngles() const
{
  Vec3 out;
  m_local_rotation.getEulerAngles(&out[0], &out[1], &out[2]);
  return out;
}

void Transform::setLocalEulerAngles(const Vec3 &angles)
{
  setLocalEulerAngles(angles.x(), angles.y(), angles.z());
}

void Transform::setLocalEulerAngles(Real xAngle, Real yAngle, Real zAngle)
{
  m_local_rotation = Quat::fromEulerAngles(xAngle, yAngle, zAngle);

  emit m_private_signal.needToUpdate();
}

void Transform::setWorldEulerAngles(const Vec3 &angles)
{
  setWorldEulerAngles(angles.x(), angles.y(), angles.z());
}

void Transform::setWorldEulerAngles(Real xAngle, Real yAngle, Real zAngle)
{
  m_local_rotation = Quat::fromEulerAngles(xAngle, yAngle, zAngle);

  if (m_parent) {
    Quat inv_parent = m_parent->worldRotation().inverted();
    m_local_rotation = inv_parent * m_local_rotation;
  }

  emit m_private_signal.needToUpdate();
}

void Transform::rotate(const Quat& q, ESpace relativeTo)
{
  switch (relativeTo) {
    case ESpace::Local: {
      rotateLocal(q);
    } break;

    case ESpace::World: {
      rotateWorld(q);
    } break;
  }

  emit m_private_signal.needToUpdate();
}

void Transform::rotate(Real xAngle, Real yAngle, Real zAngle, ESpace relativeTo)
{
  rotate(Quat::fromEulerAngles(xAngle, yAngle, zAngle), relativeTo);
}

void Transform::rotateWorld(Real xAngle, Real yAngle, Real zAngle)
{
  rotateWorld(Quat::fromEulerAngles(xAngle, yAngle, zAngle));
}

void Transform::rotateWorld(const Quat& q)
{
  if (m_parent) {

    Quat inv_parent = parent()->worldRotation().inverted();
    m_local_rotation = (inv_parent * q) * worldRotation();

  } else {

    m_local_rotation = q * m_local_rotation;
  }

  emit m_private_signal.needToUpdate();
}

void Transform::rotateLocal(Real xAngle, Real yAngle, Real zAngle)
{
  rotateLocal(Quat::fromEulerAngles(xAngle, yAngle, zAngle));
}

void Transform::rotateLocal(const Quat& q)
{
  m_local_rotation = m_local_rotation * q;

  emit m_private_signal.needToUpdate();
}

void Transform::setRotation(const Quat& q, ESpace relativeTo)
{
  switch (relativeTo) {
    case ESpace::Local: {
      setLocalRotation(q);
    } break;

    case ESpace::World: {
      setWorldRotation(q);
    } break;
  }
}

void Transform::setLocalRotation(const Quat& q)
{
  m_local_rotation = q;

  emit m_private_signal.needToUpdate();
}

void Transform::setWorldRotation(const Quat& q)
{
  if (!m_parent) {

    m_world_rotation = q;

  } else {

    Quat inv_parent = parent()->worldRotation().inverted();
    m_local_rotation = inv_parent * q;

  }

  emit m_private_signal.needToUpdate();
}

void Transform::setRotation(Real xAngle, Real yAngle, Real zAngle, ESpace relativeTo)
{
  setRotation(Quat::fromEulerAngles(xAngle, yAngle, zAngle), relativeTo);
}

void Transform::scale(const Vec3& s, ESpace relativeTo)
{
  switch (relativeTo) {
    case ESpace::Local: {
      m_local_scale *= m_world_rotation * s;
    } break;

    case ESpace::World: {
      m_local_scale *= s;
    } break;
  }

  emit m_private_signal.needToUpdate();
}

void Transform::scale(Real xScale, Real yScale, Real zScale, ESpace relativeTo)
{
  return scale({xScale, yScale, zScale}, relativeTo);
}

void Transform::setScale(const Vec3& s, ESpace relativeTo)
{
  switch (relativeTo) {
    case ESpace::Local: {
      m_local_scale = m_world_rotation * s;
    } break;

    case ESpace::World: {
      m_local_scale = s;
    } break;
  }

  emit m_private_signal.needToUpdate();
}

void Transform::setScale(Real xScale, Real yScale, Real zScale, ESpace relativeTo)
{
  return setScale({xScale, yScale, zScale}, relativeTo);
}

void Transform::translate(const Vec3& t, ESpace relativeTo)
{
  switch (relativeTo) {
    case ESpace::Local: {
      translateLocal(t);
    } break;

    case ESpace::World: {
      translateWorld(t);
    } break;
  }

  emit m_private_signal.needToUpdate();
}

void Transform::translate(Real xTranslate, Real yTranslate, Real zTranslate, ESpace relativeTo)
{
  return translate({xTranslate, yTranslate, zTranslate}, relativeTo);
}

void Transform::translateWorld(const Vec3& t)
{
  setWorldPosition(t + worldPosition());
}

void Transform::translateWorld(Real x, Real y, Real z)
{
  translateWorld({x, y, z});
}

void Transform::translateLocal(const Vec3& t)
{
  m_local_position += m_local_rotation.rotatedVector(t);

  emit m_private_signal.needToUpdate();
}

void Transform::translateLocal(Real x, Real y, Real z)
{
  translateLocal({x, y, z});
}

void Transform::setLocalPosition(const Vec3& p)
{
  m_local_position = p;

  emit m_private_signal.needToUpdate();
}

void Transform::setWorldPosition(const Vec3& p)
{
  m_world_position = p;

  if (!m_parent) {
    m_local_position = p;
  } else {
    Mat4 inv_parent = parent()->worldMatrix().inverted();
    m_local_position = inv_parent * p;
  }

  emit m_private_signal.needToUpdate();
}

void Transform::setPosition(const Vec3& p, ESpace relativeTo)
{
  switch (relativeTo) {
    case ESpace::Local: {
      m_local_position = m_world_rotation * p;
    } break;

    case ESpace::World: {
      m_local_position = p;
    } break;
  }

  emit m_private_signal.needToUpdate();
}

void Transform::setPosition(Real x, Real y, Real z, ESpace relativeTo)
{
  setPosition({x, y, z}, relativeTo);
}

void Transform::lookAt(Transform* target, const Vec3 &worldUp)
{
  lookAt(target->worldPosition(), worldUp);
}

void Transform::lookAt(const Vec3& worldPos, const Vec3& worldUp)
{
  setWorldRotation(Math::lookAtQuaternion(worldPosition(), worldPos, worldUp));
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
  m_local_matrix = Math::mat4FromTRS(m_local_position, m_local_rotation, m_local_scale);

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
