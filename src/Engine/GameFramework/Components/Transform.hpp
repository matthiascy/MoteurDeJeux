#ifndef MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_TRANSFORM_HPP
#define MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_TRANSFORM_HPP

#include <GameFramework/Forward.hpp>
#include <Core/Math/Math.hpp>
#include <Core/Core.hpp>
#include <GameFramework/Component.hpp>


class Transform : public Component<Transform> {
public:
  /**
   * Transformation space.
   */
  enum class ESpace {
    World, /**< Transformation happens in world space. */
    Local, /**< Transformation happens in local space. */
  };

private:
  /* Relative to world space. */
  Vec3 m_world_position;
  Quat m_world_rotation;
  Vec3 m_world_scale;

  /* Relative to parent space. */
  Vec3 m_local_position;
  Quat m_local_rotation;
  Vec3 m_local_scale;

  Mat4 m_local_to_world_matrix;
  Mat4 m_world_to_local_matrix;

  /**< Whether the transform needs to be updated. */
  bool m_has_changed;

  Transform*        m_parent;
  Array<Transform*> m_children;

public:
  Transform(const String& name, GameObject* gameObject, Transform* parent, const Vec3& localPosition = Math::Zero,
            const Quat& localRotation = Math::QuatIdentity, const Vec3& localScale = Math::mkVec3(1));

  ~Transform() override = default;

  /**
   * The position of the transform in world space.
   * @return transform's world position.
   */
  [[nodiscard]]
  Vec3 worldPosition();

  /**
   * The position of the transform relative to the parent transform.
   * @return local position.
   */
  Vec3 localPosition();

  /**
   * The rotation of the transform in world space.
   * @return Transform's rotation (in quaternion) in world space.
   */
  Quat worldRotation();

  /**
   * The rotation of the transform relative to the parent transform.
   * @return Transform's local rotation.
   */
  Quat localRotation();

  /**
   * The world scale of the transform.
   * @return Transform's world scale.
   */
  Vec3 worldScale();

  /**
   * The scale of the transform relative to the parent transform.
   * @return Transform's local scale.
   */
  Vec3 localScale();

  /**
   * The forward direction.
   * @return a normalized vector representing the forward direction in the world space.
   */
  Vec3 forward();

  /**
   * The up direction.
   * @return a normalized vector representing the up direction in the world space.
   */
  Vec3 up();

  /**
   * The right direction.
   * @return a normalized vector representing the right direction in the world space.
   */
  Vec3 right();

  /**
   * Matrix that transforms a point from local space into world space.
   * @return a matrix transforming a point from local space to world space.
   */
  Mat4 localToWorldMatrix();

  /**
   * Matrix that transforms a point from world space into local space.
   * @return a matrix transforming a point from local space to world space.
   */
  Mat4 worldToLocalMatrix();

  /**
   * The parent which Transform is attached to.
   * @return the pointer to parent Transform.
   */
  [[nodiscard]]
  Transform* parent() const;

  /**
   * Attach the Transform to another Transform.
   * @param parent Transform attached to.
   */
  void setParent(Transform* parent);

  /**
   * The root transform (if has).
   * @return the root transform of the hierarchy.
   */
  Transform* root();

  /**
   * Rotate a GameObject by a quaternion.
   * @param q          [in] the rotation represented by a quaternion.
   * @param relativeTo [in] determines whether to rotate the GameObject either locally to the GameObject or relative
   *                        to the Scene in world space.
   */
  void rotate(const Quat& q, ESpace relativeTo = ESpace::Local);

  /**
   * Rotate a GameObject by angles of 3 axis.
   * @param xAngle     [in]
   * @param yAngle     [in]
   * @param zAngle     [in]
   * @param relativeTo [in] determines whether to rotate the GameObject either locally to the GameObject or relative to
   *                        the Scene in world space.
   */
  void rotate(Real xAngle, Real yAngle, Real zAngle, ESpace relativeTo = ESpace::Local);

  /**
   * Translate a GameObject by t.
   * @param t          [in] translation in Vector3
   * @param relativeTo [in] determines whether to translate the GameObject either locally to the GameObject or relative to
   *                        the Scene in world space.
   */
  void translate(const Vec3& t, ESpace relativeTo);

  /**
   * Scales a GameObject by s.
   * @param s          [in] scale in Vector3
   * @param relativeTo [in] determines whether to scale the GameObject either locally to the GameObject or relative to
   *                        the Scene in world space.
   */
  void scale(const Vec3& s, ESpace relativeTo);

  /**
   * Rotates the transform about axis passing through point in world coordinates by angle degrees.
   *
   * This modifies both the position and the rotation of the transform.
   *
   * @param point [in] the point we rotate around in world coordinates.
   * @param axis  [in] the axis passing through point in word coordinates.
   * @param angle [in] the rotation angle by degrees.
   */
  void rotateAround(const Vec3& point, const Vec3& axis, Real angle);

  /* Rotates the transform so the forward vector points at target's current position. */
  void lookAt(const Transform& target, const Vec3& worldUp);

  /* Rotates the transform so the forward vector points at worldPosition. */
  void lookAt(const Vec3& worldPos, const Vec3& worldUp);

  Vec3 transformLocalPositionToWorld(const Vec3& pos);

  Vec3 transformWorldPositionToLocal(const Vec3& pos);

  /**
   * Unparents all children.
   */
  void detachChildren();

  /**
   * Finds a child by name and returns it.
   * @param name [in] The name of the child to be found.
   * @return the child Transform pointer or nullptr if child is not found.
   */
  Transform* find(const String& name);

  /**
   * Is this transform a child of parent?
   * @param parent [in] the parent Transform.
   * @return a boolean value that indicates whether the transform is a child of a given transform. true if this
   *         transform is a child, otherwise false.
   */
  bool isChildOf(Transform* parent) const;

  /**
   * Child of the parent Transform at position `index'
   * @param index [in] the position of child.
   * @return the child at the specific position of the parent Transform.
   */
  Transform* childAt(Int32 index);

  /**
   * The number of children the parent Transform has.
   * @return Transform's children count.
   */
  [[nodiscard]]
  UInt32 childCount() const;

  [[nodiscard]]
  const Array<Transform*>& children() const;

private:
  void set_has_changed();
  void instant_update();
};


#endif  /* !MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_TRANSFORM_HPP */
