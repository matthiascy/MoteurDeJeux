#ifndef SCENENODE_HPP
#define SCENENODE_HPP

#include "Types.hpp"
#include "Node.hpp"
#include "GameObject.hpp"
#include "Camera.hpp"
#include "Light.hpp"

class SceneNode : public Node {
public:
  typedef HashMap<String, GameObject*> ObjectMap;
  typedef HashMap<String, GameObject*>::iterator ObjectMapInterator;
  typedef HashMap<String, GameObject*>::const_iterator ConstObjectMapInterator;

protected:
  ObjectMap m_attached_objects;
  bool m_is_in_scene_graph;
  bool m_is_visible;

  void  _update() override;

  virtual void attachSceneGraph();
  virtual void detachSceneGraph();

public:
  SceneNode();
  explicit SceneNode(const String& name);
  ~SceneNode() override;

  void setParent(Node* parent) override;

  virtual bool isInSceneGraph() const;

  virtual bool isVisiable() const;

  virtual void setVisible(bool isVisible);

  virtual UInt32 attachedObjectsCount() const;

  // Obtenir l'objet attache
  virtual const ObjectMap& attachedObjects() const;

  virtual GameObject* attachedObject(UInt32 index);

  virtual GameObject* attachedObject(const String& name);

  // Attacher l'objet
  virtual void attachObject(GameObject* obj);

  // Dettacher l'objet
  virtual void detachObject(GameObject* obj);

  virtual GameObject* detachObject(UInt32 index);

  virtual GameObject* detachObject(const String& name);

  virtual void detachAllObjects();

  virtual void removeAndDestoryChild(const String& name);

  virtual void removeAndDestoryAllChildren();
};

#endif // SCENENODE_HPP
