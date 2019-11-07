#ifndef NODE_HPP
#define NODE_HPP

#include "Types.hpp"
#include "NameGenerator.hpp"

class Node {
public:
  typedef HashMap<String, Node*> ChildNodeMap;
  typedef HashMap<String, Node*>::iterator ChildNodeIterator;
  typedef HashMap<String, Node*>::const_iterator ConstChildNodeIterator;

protected:
  Node*        m_parent;
  ChildNodeMap m_children;

  String m_name;

  // Local information
  Quat m_orientation;
  Vec3 m_position;
  Vec3 m_scale;

  Quat m_combined_orientation;
  Vec3 m_combined_position;
  Vec3 m_combined_scale;

  // World information
  Mat4 m_world_matrix;
  Mat4 m_local_matrix;

  // Est-ce que ce node herite orientation/scale de son parent ?
  bool m_is_inherited_position;
  bool m_is_inherited_orientation;
  bool m_is_inherited_scale;

  static NameGenerator m_name_generator;

public:
  explicit Node(Node* parent = nullptr);
  explicit Node(const String& name, Node* parent = nullptr);
  virtual ~Node();

  String name() const { return m_name; }
  void setName(const String& name) { m_name = name; }

  void setInheritedTransformation(bool inheritedScale, bool inheritedPosition, bool inheritedOrientation);
  bool isScaleInherited() const;
  bool isPositionInherited() const;
  bool isOrientationInherited() const;

  virtual void setParent(Node* parent);
  virtual Node* parent() const;

  // Information about the transformation
  // Position/Orientation/Scale dans le repere du monde
  virtual const Vec3& worldPosition() const;
  virtual const Quat& worldOrientation() const;
  virtual const Vec3& worldScale() const;
  // Position/Orientation/Scale dans le repere de lui meme
  virtual const Vec3& localPosition() const;
  virtual const Quat& localOrientation() const;
  virtual const Vec3& localScale() const;
  // Gets the Position/Orientation/Scale dans le repere de son parent
  virtual const Vec3& position() const;
  virtual const Quat& orientation() const;
  virtual const Vec3& scale() const;

  virtual const Mat4& worldMatrix();
  virtual const Mat4& localMatrix();

  // Sets the Position/Orientation/Scale of the node relative to it's parent.
  virtual void setOrientation(const Quat& quat);
  virtual void setPosition(const Vec3& pos);
  virtual void setScale(const Vec3& scale);

  virtual void translate(const Vec3& t, ETransformSpace relativeTo);
  virtual void rotate(const Quat& q, ETransformSpace relativeTo);
  virtual void scale(const Vec3& s, ETransformSpace relativeTo);

  // Child information
  virtual Node* child(const String& name) const;
  virtual Node* child(UInt32 index) const;
  virtual bool hasChildren() const;
  virtual UInt32 childCount() const;
  virtual Node* removeChild(const String& name);
  virtual Node* removeChild(UInt32 index);
  virtual Node* removeChild(Node* child);
  virtual void removeAllChildren();
  virtual void addChild(Node* child);

  virtual void update(bool isUpdateChildren);

  virtual Vec3 localToWorldPosition(const Vec3& localPos);
  virtual Vec3 worldToLocalPosition(const Vec3& worldPos);
  virtual Quat localToWorldOrientation(const Quat& localOrientation);
  virtual Quat worldToLocalOrientation(const Quat& worldOrientation);

  virtual const Quat& combinedOrientation();
  virtual const Vec3& combinedPosition();
  virtual const Vec3& combinedScale();

protected:
  virtual void _update();
  virtual void _update_from_parent();
  virtual void _update_world_matrix();
  virtual void _update_local_matrix();
};

#endif // NODE_HPP
