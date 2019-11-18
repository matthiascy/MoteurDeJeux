#ifndef MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_TRANSFORM_HPP
#define MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_TRANSFORM_HPP

#include <Engine/GameFramework/Forward.hpp>
#include <Engine/Core/Math/Math.hpp>
#include <Engine/Core/CoreTypes.hpp>
#include <Engine/GameFramework/Component.hpp>
//#include <Engine/GameFramework/GameObject.hpp>

class Transform : public Component {
public:
  enum class ESpace {
    World,
    Local,
  };

private:
  /* Relative to world space. */
  Vec3 m_position;
  Quat m_rotation;
  Vec3 m_scale;

  /* Relative to parent space. */
  Vec3 m_local_position;
  Quat m_local_rotation;
  Vec3 m_local_scale;

  Mat4 m_local_to_world_matrix;
  Mat4 m_world_to_local_matrix;

  /* Whether the transform needs to be updated. */
  bool m_has_changed;

  Transform*        m_parent;
  Array<Transform*> m_children;

public:
  Transform(String name, GameObject* gameObject, Transform* parent, const Vec3& localPosition = vec3::Zero, const Quat& localRotation = quat::Identity, const Vec3& localScale = vec3::mkVec3(1));
  ~Transform() override = default;

  /* The position of the transform in world space. */
  const Vec3& position();
  void setPosition(const Vec3& position);

  /* The position of the transform relative to the parent transform. */
  const Vec3& localPosition();
  void setLocalPosition(const Vec3& position);

  /* The rotation of the transform in world space. */
  const Quat& rotation();
  void setRotation(const Quat& rotation);

  /* The rotation fo the transform relative to the parent transform. */
  const Quat& localRotation();
  void setLocalRotation(const Quat& rotation);

  /* The scale of the transform in world space. */
  const Vec3& scale();
  void setScale(const Vec3& scale);

  /* The scale of the transform relative to the parent transform. */
  const Vec3& localScale();
  void setLocalScale(const Vec3& scale);

  Vec3 forward();
  Vec3 up();
  Vec3 right();

  Mat4 localToWorldMatrix();
  Mat4 worldToLocalMatrix();

  Transform* parent() const;
  void setParent(Transform* parent);

  Transform* root();

  void rotate(const Quat& q);
  void scale(const Vec3& s);
  void translate(const Vec3& t);

  void rotate(const Quat& q, ESpace relativeTo);
  void translate(const Vec3& t, ESpace relativeTo);
  void scale(const Vec3& s, ESpace relativeTo);

  /* Rotates the transform about axis passing through point in world coordinates by angle degrees */
  void rotateAround(const Vec3& point, const Vec3& axis, float angle);

  /* Rotates the transform so the forward vector points at target's current position. */
  void lookAt(const Transform& target, const Vec3& worldUp);

  /* Rotates the transform so the forward vector points at worldPosition. */
  void lookAt(const Vec3& worldPos, const Vec3& worldUp);

  Vec3 transformLocalPositionToWorld(const Vec3& pos);

  Vec3 transformWorldPositionToLocal(const Vec3& pos);

  /* Detach all children*/
  void detachChildren();

  /* Finds a child by name and returns it. */
  Transform* find(const String& name);

  bool isChildOf(Transform* parent) const;

  Transform* child(Int32 index);

  UInt32 childCount() const;

private:
  void set_has_changed();
  void instant_update();
};


#endif  /* !MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_TRANSFORM_HPP */
