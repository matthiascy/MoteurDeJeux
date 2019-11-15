#ifndef MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_SCENE_HPP
#define MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_SCENE_HPP

#include "Types.hpp"
#include "GameObject.hpp"
#include "NameGenerator.hpp"

// TODO vbo index for Creating and Destroying GameObject

class AssetManager;
class RendererSystem;
class PhysicsSystem;
class InputSystem;

class Scene : Object {
public:
  typedef Array<GameObject*> GameObjectsArray;
  typedef HashMap<String, GameObjectsArray> TaggedGameObjects;
  typedef Array<Component*> ComponentsArray;

protected:
  TaggedGameObjects m_game_objects;
  ComponentsArray   m_components;
  AssetManager*     m_asset_manager;
  RendererSystem*   m_renderer_sys;
  PhysicsSystem*    m_physics_sys;
  InputSystem*      m_input;
  Int32             m_vbo_index;


  //UniquePtr<NameGenerator> m_name_generator;

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

  virtual Transform* root();

  //virtual GameObject* createTerrain(UInt32 width, UInt32 height, const Image& hmap, Real baseAlt);
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

#endif  /* !MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_SCENE_HPP */
