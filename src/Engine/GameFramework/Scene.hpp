#ifndef MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_SCENE_HPP
#define MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_SCENE_HPP

#include <Core/Core.hpp>
#include "GameObject.hpp"
#include "NameGenerator.hpp"

// TODO vbo index for Creating and Destroying GameObject

class AssetManager;
class RenderSystem;
class PhysicsSystem;
class InputSystem;

class Scene : Object {
public:
  typedef Array<GameObject*> GameObjectsArray;
  typedef HashMap<String, GameObjectsArray*> TaggedGameObjects;
  typedef Array<Component*> ComponentsArray;

protected:
  UniquePtr<TaggedGameObjects> m_game_objects;
  UniquePtr<ComponentsArray>   m_components;
  Transform* m_root;
  Int32      m_vbo_idx;

public:
  Scene() = delete;
  explicit Scene(String name);
  ~Scene() override;

  virtual Transform* root();

  virtual GameObject* createGameObject(String name, const String& tag);

  virtual Array<GameObject*> gameObjects() const;

  virtual GameObject* mainCamera() const;
};

#endif  /* !MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_SCENE_HPP */
