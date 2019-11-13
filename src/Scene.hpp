#ifndef SCENEMANAGER_HPP
#define SCENEMANAGER_HPP

#include "Types.hpp"
#include "GameObject.hpp"
#include "NameGenerator.hpp"

// TODO vbo index for Creating and Destroying GameObject

class Scene {
public:
  typedef HashMap<String, Camera*>        CameraMap;
  typedef HashMap<String, GameObject*>    GameObjectMap;
  typedef HashMap<String, HashMap<String, GameObject*>*> GameObjectCollectionMap;

protected:
  GameObject*             m_root;
  CameraMap               m_cameras;
  GameObjectCollectionMap m_collections;
  Int32                   m_vbo_index;

  UniquePtr<NameGenerator> m_name_generator;

public:
  Scene();
  virtual ~Scene();

  virtual void clearScene();

  virtual Camera* activatedCamera() const;
  virtual Camera* createCamera(const String& name);
  virtual Camera* camera(const String& name) const;
  virtual bool hasCamera(const String& name) const;
  virtual void removeCamera(Camera* cam);
  virtual void removeCamera(const String& name);
  virtual void removeAllCameras();

  virtual GameObject* root();

  virtual GameObject* createTerrain(UInt32 width, UInt32 height, const Image& hmap, Real baseAlt);
  virtual GameObject* createGameObject(const String& name, const String& type);
  virtual void removeGameObject(const String& name, const String& type);
  virtual void removeGameObject(GameObject* obj);
  virtual void removeAllGameObjectsByType(const String& type);
  virtual void removeAllGameObjects();
  virtual GameObject* gameObject(const String& name, const String& type);
  virtual bool hasGameObject(const String& name, const String& type);
  virtual Array<const GameObject*> gameObjects() const;

  virtual GameObjectMap* gameObjectsOfType(const String& type);
  virtual const GameObjectMap* gameObjectsOfType(const String& type) const;

public:
  virtual Array<GameObject*> toBeRenderedGameObjects() const;
};

#endif // SCENEMANAGER_HPP
