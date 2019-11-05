#ifndef SCENEMANAGER_HPP
#define SCENEMANAGER_HPP

#include "Types.hpp"
#include "SceneNode.hpp"

class SceneManager {
public:
  typedef HashMap<String, Camera*>        CameraMap;
  typedef HashMap<String, SceneNode*>     SceneNodeMap;
  typedef HashMap<String, GameObject*>    GameObjectMap;
  typedef HashMap<String, GameObjectMap*> GameObjectCollectionMap;

protected:
  SceneNode* m_scene_root;
  //OpenGLShader m_shadow_map_shader;
  //OpenGLShader m_z_pre_pass_shader;
  //OpenGLShader m_reflection_shader;
  Vec3 m_light_vector;
  // TODO SUN

  CameraMap               m_cameras;
  SceneNodeMap            m_scene_nodes;
  //GameObjectMap           m_objects;
  GameObjectCollectionMap m_object_collections;

  static NameGenerator m_game_object_name_generator;

public:
  SceneManager();
  virtual ~SceneManager();
  // TODO::renderSequence
  virtual void updateSceneGraph();
  virtual void clearScene();

  // Camera management
  virtual Camera* activatedCamera() const;
  virtual Camera* createCamera(const String& name);
  virtual Camera* camera(const String& name) const;
  virtual bool hasCamera(const String& name) const;
  virtual void destroyCamera(Camera* cam);
  virtual void destroyCamera(const String& name);
  virtual void destroyAllCameras();

  // Scene node management
  virtual SceneNode* createSceneNode();
  virtual SceneNode* createSceneNode(const String& name);
  virtual SceneNode* createSceneNode(const String& name, Node* parent);
  virtual SceneNode* rootSceneNode();

  virtual SceneNode* sceneNode(const String& name) const;
  virtual bool hasSceneNode(const String& name) const;

  virtual void destroySceneNode(const String& name);
  virtual void destroySceneNode(SceneNode* node);

  // Game object management
  virtual GameObject* createGameObject(const String& name, const String& type);
  virtual GameObject* createGameObject(const String& type);
  virtual void destroyGameObject(const String& name, const String& type);
  virtual void destroyGameObject(GameObject* obj);
  virtual void destroyAllGameObjectsByType(const String& type);
  virtual void destroyAllGameObjects();
  virtual GameObject* gameObject(const String& name, const String& type);
  virtual bool hasGameObject(const String& name, const String& type);

  virtual GameObjectMap* gameObjectsOfType(const String& type);
  virtual const GameObjectMap* gameObjectsOfType(const String& type) const;

  // Light management
  virtual Light* createLight(const String& name, ELightType type);
  virtual Light* createLight(ELightType type);
  virtual Light* light(const String& name);

public:
  virtual void addGameObject(GameObject* obj);

  //virtual UInt32 addedGameObjectsNum() const;

  virtual Array<GameObject*> toBeRenderedGameObjects() const;
  virtual Array<SceneNode*> toBeRenderedSceneNodes() const;
};

#endif // SCENEMANAGER_HPP
