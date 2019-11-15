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
  typedef Array<UniquePtr<GameObject>> GameObjectsArray;
  typedef HashMap<String, UniquePtr<GameObjectsArray>> TaggedGameObjects;
  typedef Array<UniquePtr<Component>> ComponentsArray;

protected:
  UniquePtr<TaggedGameObjects> m_game_objects;
  UniquePtr<ComponentsArray>   m_components;
  Transform* m_root;
  Int32      m_vbo_idx;

public:
  Scene() = delete;
  explicit Scene(String name);
  virtual ~Scene();

  virtual Transform* root();

  virtual GameObject* createGameObject(String name, String tag);

  virtual Array<GameObject*> gameObjects() const;
};

#endif  /* !MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_SCENE_HPP */
