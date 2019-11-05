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

  // Orientation/Position/Scale de Parent et Fils combine
  Quat m_combined_orientation;
  Vec3 m_combined_position;
  Vec3 m_combined_scale;

  // Est-ce que ce node herite orientation/scale de son parent ?
  bool m_is_inherited_orientation;
  bool m_is_inherited_scale;

  static NameGenerator m_name_generator;

public:
  explicit Node(Node* parent = nullptr);
  explicit Node(const String& name, Node* parent = nullptr);
  virtual ~Node();

  String name() const { return m_name; }
  void setName(const String& name) { m_name = name; }

  virtual void setParent(Node* parent);
  virtual Node* parent() const;

  // Information about the transformation
  virtual const Quat& orientation() const;
  virtual const Quat& combinedOrientation() const;
  virtual void setOrientation(const Quat& q);

  virtual const Vec3& position() const;
  virtual const Vec3& combinedPosition() const;
  virtual void setPosition(const Vec3& pos);

  virtual const Vec3& scale() const;
  virtual const Vec3& combinedScale() const;
  virtual void setScale(const Vec3& scale);

  virtual void translate(const Vec3& t, ETransformSpace relativeTo);
  virtual void rotate(const Vec3& axis, float angle, ETransformSpace relativeTo);
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
  virtual Node* createChild(const Vec3& translation, const Quat& rotation);
  virtual Node* createChild(const String& name, const Vec3& translation, const Quat& rotation);
  virtual void addChild(Node* child);

  // Change space
  Vec3 convertWorldPositionToLocalPosition(const Vec3& pos);
  Vec3 convertLocalPositionToWorldPosition(const Vec3& pos);
  Quat convertWorldOrientationToLocalOrientation(const Quat& q);
  Quat convertLocalOrientationToLocalOrientation(const Quat& q);

  // Get position, orientation, scale dans le monde
  virtual const Vec3& worldPoistion() const;
  virtual const Quat& worldOrientation() const;
  virtual const Vec3& worldScale() const;

  virtual void update(bool isUpdateChildren);

protected:
  virtual void  _update();
};

#endif // NODE_HPP
