#include "SceneNode.hpp"

SceneNode::SceneNode()
  : Node(), m_attached_objects{}, m_is_in_scene_graph{false}, m_is_visible{false}
{ }

SceneNode::SceneNode(const String& name)
  : Node(name), m_attached_objects{}, m_is_in_scene_graph{false}, m_is_visible{false}
{ }

SceneNode::~SceneNode()
{
  m_attached_objects.clear();
}

void SceneNode::_update()
{
  /*
   if (m_parent) {

     // update orientation
     const Quat& parent_orientation = m_parent->orientation();
     if (m_is_inherited_orientation) {
       m_combined_orientation = parent_orientation * m_orientation;
     } else {
       m_combined_orientation = m_orientation;
     }

     // update scale
     const Vec3& parent_scale = m_parent->scale();
     if (m_is_inherited_scale) {
       m_combined_scale = parent_scale * m_scale; // component-wise mulitplication
     } else {
       m_combined_scale = m_scale;
     }

     // update position
     m_combined_position = parent_orientation * parent_scale * m_position;
     m_combined_position += m_parent->combinedPosition();

   } else {

     // sans parent, la racine
     m_combined_orientation = m_orientation;
     m_combined_position = m_position;
     m_combined_scale = m_scale;

   }

  qDebug() << "Scene Node update, combined scale" << m_combined_scale;
   */
}

void SceneNode::setParent(Node *parent)
{
  if (parent) {
    Node::setParent(parent);

    SceneNode* scene_parent = static_cast<SceneNode*>(parent);
    if (scene_parent->isInSceneGraph()) attachSceneGraph();
    else detachSceneGraph();

  } else {

    detachSceneGraph();

  }
  update(true);
}

bool SceneNode::isInSceneGraph() const
{
  return m_is_in_scene_graph;
}

bool SceneNode::isVisiable() const
{
  return m_is_visible;
}

void SceneNode::setVisible(bool isVisible)
{
  m_is_visible = isVisible;
  for (auto& i : m_attached_objects) {
    i->setVisible(isVisible);
  }
}

UInt32 SceneNode::attachedObjectsCount() const
{
  return static_cast<UInt32>(m_attached_objects.size());
}

void SceneNode::attachSceneGraph()
{
  m_is_in_scene_graph = true;
}

void SceneNode::detachSceneGraph()
{
  m_is_in_scene_graph = false;
}

GameObject* SceneNode::attachedObject(UInt32 index)
{
  if (index < static_cast<UInt32>(m_attached_objects.size())) {
    auto i = m_attached_objects.begin();
    while (index--) ++i;
    return i.value();
  } else {
    return nullptr;
  }
}

GameObject* SceneNode::attachedObject(const String& name)
{
  auto i = m_attached_objects.find(name);
  if (i != m_attached_objects.end()) {
    return i.value();
  } else {
    return nullptr;
  }
}

GameObject* SceneNode::detachObject(UInt32 index)
{
  GameObject* ret = nullptr;
  if (index < static_cast<UInt32>(m_attached_objects.size())) {
    auto i = m_attached_objects.begin();
    while (index--) ++i;
    ret = i.value();
    m_attached_objects.erase(i);
    // TODO udpate
  }

  return ret;
}

void SceneNode::detachObject(GameObject* obj)
{
  for (auto i = m_attached_objects.begin(); i != m_attached_objects.end(); ++i) {
    if (i.value() == obj) {
      obj->setSceneNodeAttached(false);
      m_attached_objects.erase(i);
      break;
    }
  }
  // TODO update
}

void SceneNode::attachObject(GameObject* obj)
{
  if (obj) {
    qDebug() << "SceneNode" << m_name << "attach node" << obj->name();
    if (!obj->isSceneNodeAttached() && (m_attached_objects.find(obj->name()) == m_attached_objects.end())) {
      m_attached_objects.insert(obj->name(), obj);
      obj->setSceneNodeAttached(true);
    }
  }
  //qDebug() << "SceneNode" << m_name << "attach nodes count" << m_attached_objects.size();
  // TODO update
}

GameObject* SceneNode::detachObject(const String& name)
{
  GameObject* ret = nullptr;
  auto i = m_attached_objects.find(name);
  if (i != m_attached_objects.end()) {
    ret = i.value();
    m_attached_objects.erase(i);
    // TODO update
  }

  return ret;
}

void SceneNode::detachAllObjects()
{
  m_attached_objects.clear();
  // TODO update
}

void SceneNode::removeAndDestoryChild(const String& name)
{
  auto* child_node = static_cast<SceneNode*>(child(name));
  child_node->removeAndDestoryAllChildren();
  removeChild(name);
  // TODO destroy scene node
}

void SceneNode::removeAndDestoryAllChildren()
{
  for (auto& i : m_children) {
    auto* node = static_cast<SceneNode*>(i);
    node->removeAndDestoryAllChildren();
    // TODO destroy scene nodes
  }
  m_children.clear();
  // TODO update
}

const SceneNode::ObjectMap& SceneNode::attachedObjects() const
{
  return m_attached_objects;
}

