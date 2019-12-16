#ifndef MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_TRANSFORM_HPP
#define MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_TRANSFORM_HPP

#include <Core/Math/Math.hpp>
#include <Core/Core.hpp>
#include <GameFramework/Component.hpp>
#include <GameFramework/Types.hpp>

// TODO: lookAt fix, not always correct.

class Transform : public Component {
private:
  /** World-space properties. */
  /** Local-space properties of transform. (relative to parent). */
  Vec3 m_local_position     { };
  Quat m_local_rotation     { };
  Vec3 m_local_scale        {1, 1, 1};

  Mat4 m_local_matrix  { };
  Mat4 m_world_matrix  { };
  Mat4 m_normal_matrix { };

  Mat4 m_local_to_world_matrix { };
  Mat4 m_world_to_local_matrix { };

  Transform*        m_parent   { };
  Array<Transform*> m_children { };

  UInt32 m_depth { };

  bool m_is_scale_compensated   { false };

  ///@{ Normalized local space basis vectors.
  Vec3 m_right    { };
  Vec3 m_up       { };
  Vec3 m_forward  { };
  ///@}

private:
  void _update();

public:
  Transform(const String& name, GameObject* gameObject, Transform* parent, const Vec3& localPosition = Math::Zero,
            const Quat& localRotation = Math::QuatIdentity, const Vec3& localScale = Math::mkVec3(1));

  ~Transform() override = default;

  [[nodiscard]]
  UInt64 typeID() const override {
    return family::type<Transform>;
  }

  /**
   * The position of the transform in world space.
   * @return transform's world position.
   */
  [[nodiscard]]
  Vec3 worldPosition() const;

  /**
   * The position of the transform relative to the parent transform.
   * @return local position.
   */
  [[nodiscard]]
  Vec3 localPosition() const;

  /**
   * The rotation of the transform in world space.
   * @return Transform's rotation (in quaternion) in world space.
   */
  [[nodiscard]]
  Quat worldRotation() const;

  /**
   * The rotation of the transform relative to the parent transform.
   * @return Transform's local rotation.
   */
  [[nodiscard]]
  Quat localRotation() const;

  /**
   * The world scale of the transform.
   * @return Transform's world scale.
   */
  [[nodiscard]]
  Vec3 worldScale() const;

  /**
   * The scale of the transform relative to the parent transform.
   * @return Transform's local scale.
   */
  [[nodiscard]]
  Vec3 localScale() const;

  /**
   * The world transformation matrix.
   * @return Transform's world transformation matrix.
   */
  [[nodiscard]]
  Mat4 worldMatrix() const;

  /**
   * The local transformation matrix representing the transform relative to the node's
   * parent's world transformation matrix.
   * @return Transform's local transformation matrix.
   */
  [[nodiscard]]
  Mat4 localMatrix() const;

  /**
   * The normalized local space X-axis vector of the Transform in world space.
   * @return a normalized vector in the world space.
   */
  [[nodiscard]]
  Vec3 right() const;

  /**
   * The normalized local space Y-axis vector of the Transform in world space.
   * @return a normalized vector in the world space.
   */
  [[nodiscard]]
  Vec3 up() const;

  /**
   * The normalized local space Z-axis vector of the Transform in world space.
   * @return a normalized vector in world space.
   */
  [[nodiscard]]
  Vec3 forward() const;

  /**
   * Matrix that transforms a point from local space into world space.
   *
   * p' = T * R * S * p
   *
   * Global -=|=- Local
   *
   * @return a matrix transforming a point from local space to world space.
   */
  [[nodiscard]]
  Mat4 localToWorldMatrix() const;

  /**
   * Matrix that transforms a point from world space into local space.
   * @return a matrix transforming a point from local space to world space.
   *
   * p' = T * R * S * p
   * Global -=|=- Local
   *
   */
  [[nodiscard]]
  Mat4 worldToLocalMatrix() const;

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
   * The depth of this child within the hierarchy.
   * @return the depth in UInt32.
   */
  [[nodiscard]]
  UInt32 depth() const;

  /**
   * Rotate a GameObject by a quaternion.
   * @param q          [in] the rotation represented by a quaternion.
   * @param relativeTo [in] determines whether to rotate the GameObject either locally to the GameObject or relative
   *                        to the Scene in world space.  When rotating in world space, GameObject's
   *                         x, y, and z axes are aligned with the x, y and z world axes.
   */
  void rotate(const Quat& q, ESpace relativeTo);

  /**
   * Rotate a GameObject by EulerAngles(XYZ).
   * @param eulerAngles [in] rotation angles in different axes.
   * @param relativeTo  [in] determines whether to rotate the GameObject either locally to the GameObject
   *                         or relative to the Scene in world space. When rotating in world space, GameObject's
   *                         x, y, and z axes are aligned with the x, y and z world axes.
   */
  void rotate(const EulerAngles& eulerAngles, ESpace relativeTo);

  /**
   * Rotate a GameObject by an axis and an angle.
   * @param axis       [in] rotation axis.
   * @param angle      [in] rotation angle.
   * @param relativeTo [in] determines whether to rotate the GameObject either locally to the GameObject
   *                        or relative to the Scene in world space. When rotating in world space, GameObject's
   *                        x, y, and z axes are aligned with the x, y and z world axes.
   */
  void rotate(const Vec3& axis, Real angle, ESpace relativeTo);

  /**
   * Rotates the transform about axis passing through point in world coordinates by angle degrees.
   * This modifies both the position and the rotation of the transform.
   * @param target [in]
   * @param axis   [in]
   * @param angle  [in]
   */
  void rotateAround(const Vec3& target, const Vec3& axis, float angle);

  void rotateAround(const Vec3& target, const Quat& q);

  void rotateAround(const Vec3& target, EulerAngles& eulerAngles);

  /**
   * Set directly Transform's rotation.
   * @param q          [in] rotation in Quaternion representation.
   * @param relativeTo [in] determines whether to rotate the GameObject either locally to the GameObject
   *                        or relative to the Scene in world space. When rotating in world space, GameObject's
   *                        x, y, and z axes are aligned with the x, y and z world axes.
   */
  void setRotation(const Quat& q, ESpace relativeTo);

  /**
   * Set directly Transform's rotation.
   * @param xAngle
   * @param yAngle
   * @param zAngle
   * @param relativeTo
   */
  void setRotation(const EulerAngles& eulerAngles, ESpace relativeTo);

  /**
   * Translate a GameObject by t.
   *
   * If the translation is relative to local space, the translation respect the local orientation.
   *
   * If the translation is relative to world space, the translation will be applied directly.
   *
   * @param t          [in] translation in Vector3
   * @param relativeTo [in] determines whether to translate the GameObject either locally to the GameObject or relative
   *                        to the Scene in world space.
   */
  void translate(const Vec3& t, ESpace relativeTo);

  void setPosition(const Vec3& p, ESpace relativeTo);

  void setLocalScale(const Vec3& s);

  /**
   * Rotates the transform so the forward vector points at target's current position.
   * @param target  [in]
   * @param worldUp [in]
   */
  void lookAt(Transform* target, const Vec3& worldUp);

  /**
   * Rotates the transform so the forward vector points at worldPosition.
   * @param worldPos [in]
   * @param worldUp  [in]
   */
  void lookAt(const Vec3& worldPos, const Vec3& worldUp);

  /**
   * Transforms a position from local space to world space.
   * @param pos [in] position to be transformed.
   * @return a position transformed to world space.
   */
  [[nodiscard]]
  Vec3 transformLocalPositionToWorld(const Vec3& pos) const;

  /**
   * Transforms a position from world space to local space.
   * @param pos [in] position to be transformed.
   * @return a position transformed to local space.
   */
  [[nodiscard]]
  Vec3 transformWorldPositionToLocal(const Vec3& pos) const;

  /**
   * Un-parents all children.
   */
  void detachChildren();

  /**
   * Finds a child by name and returns it.
   * @param name [in] The name of the child to be found.
   * @return the child Transform pointer or nullptr if child is not found.
   */
  [[nodiscard]]
  Transform* find(const String& name) const;

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
  [[nodiscard]]
  Transform* childAt(Int32 index) const;

  /**
   * The number of children the parent Transform has.
   * @return Transform's children count.
   */
  [[nodiscard]]
  UInt32 childCount() const;

  [[nodiscard]]
  const Array<Transform*>& children() const;
};


#endif  /* !MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_TRANSFORM_HPP */
