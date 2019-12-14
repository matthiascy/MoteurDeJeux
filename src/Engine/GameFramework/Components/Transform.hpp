#ifndef MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_TRANSFORM_HPP
#define MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_TRANSFORM_HPP

#include <GameFramework/Forward.hpp>
#include <Core/Math/Math.hpp>
#include <Core/Core.hpp>
#include <GameFramework/Component.hpp>

class Transform : public Component<Transform>{
public:
  /**
   * Transformation space.
   */
  enum class ESpace {
    World, /**< Transformation happens in world space. */
    Local, /**< Transformation happens in local space (relative to parent). */
  };

private:

  ///@{
  /** World-space properties. */
  Vec3 m_world_position     { };
  Quat m_world_rotation     { };
  Vec3 m_world_scale        {1, 1, 1};
  Vec3 m_world_euler_angles { };
  ///@}

  ///@{
  /** Local-space properties of transform. */
  Vec3 m_local_position     { };
  Quat m_local_rotation     { };
  Vec3 m_local_scale        {1, 1, 1};
  Vec3 m_local_euler_angles { };
  ///@}

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
   * Get the world space rotation in Euler angle form. The order of the returned Euler angle is XYZ.
   * @return the world space rotation of the Transform in Euler angle form.
   */
  [[nodiscard]]
  Vec3 worldEulerAngles() const;

  /**
   * Get the local space rotation in Euler angle form. The order of the returned Euler angle is XYZ.
   * @return the local space rotation of the Transform in Euler angle form.
   */
  [[nodiscard]]
  Vec3 localEulerAngles() const;

  /**
   * Sets the local space rotation of Transform using euler angles in degrees.
   *
   * Eulers are interpreted in XYZ order.
   *
   * @param xAngle [in] rotation around x-axis of local space in degrees.
   * @param yAngle [in] rotation around y-axis of local space in degrees.
   * @param zAngle [in] rotation around z-axis of local space in degrees.
   */
  void setLocalEulerAngles(Real xAngle, Real yAngle, Real zAngle);

  /**
   * Sets the local space rotation of Transform using euler angles in degrees.
   *
   * @param angles [in] rotation containing the rotation of 3 axis in degrees.
   */
  void setLocalEulerAngles(const Vec3& angles);

  /**
   * Sets the world space rotation of Transform using euler angles in degrees.
   *
   * Eulers are interpreted in XYZ order.
   *
   * @param xAngle [in] rotation around x-axis of world space in degrees.
   * @param yAngle [in] rotation around y-axis of world space in degrees.
   * @param zAngle [in] rotation around z-axis of world space in degrees.
   */
  void setWorldEulerAngles(Real xAngle, Real yAngle, Real zAngle);

  /**
   * Sets the world space rotation of Transform using euler angles in degrees.
   *
   * @param angles [in] rotation containing the rotation of 3 axis in degrees.
   */
  void setWorldEulerAngles(const Vec3& angles);

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

  void rotateWorld(Real xAngle, Real yAngle, Real zAngle);

  void rotateWorld(const Quat& q);

  void rotateLocal(Real xAngle, Real yAngle, Real zAngle);

  void rotateLocal(const Quat& q);

  void setLocalRotation(const Quat& q);

  void setWorldRotation(const Quat& q);

  /**
   *
   * @param q
   * @param relativeTo
   */
  void setRotation(const Quat& q, ESpace relativeTo = ESpace::Local);

  /**
   *
   * @param xAngle
   * @param yAngle
   * @param zAngle
   * @param relativeTo
   */
  void setRotation(Real xAngle, Real yAngle, Real zAngle, ESpace relativeTo = ESpace::Local);

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
  void translate(const Vec3& t, ESpace relativeTo = ESpace::Local);

  /**
   *
   * @param x          [in]
   * @param y          [in]
   * @param z          [in]
   * @param relativeTo [in]
   */
  void translate(Real xTranslate, Real yTranslate, Real zTranslate, ESpace relativeTo = ESpace::Local);

  void translateWorld(const Vec3& t);

  void translateWorld(Real x, Real y, Real z);

  void translateLocal(const Vec3& t);

  void translateLocal(Real x, Real y, Real z);

  void setLocalPosition(const Vec3& p);

  void setWorldPosition(const Vec3& p);

  void setPosition(const Vec3& p, ESpace relativeTo);

  /**
   *
   * @param x          [in]
   * @param y          [in]
   * @param z          [in]
   * @param relativeTo [in]
   */
  void setPosition(Real x, Real y, Real z, ESpace relativeTo = ESpace::Local);

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
   * Unparents all children.
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
