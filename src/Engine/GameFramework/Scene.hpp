#ifndef MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_SCENE_HPP
#define MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_SCENE_HPP

#include <Core/Core.hpp>

// TODO vbo index for Creating and Destroying GameObject

class AssetManager;
class RenderSystem;
class PhysicsSystem;
class InputSystem;
class GameObject;

class Scene : public Object {
public:
  typedef Array<GameObject*> GameObjectsArray;
  typedef HashMap<String, GameObjectsArray*> TaggedGameObjects;
  //typedef Array<Component*> ComponentsArray;

protected:
  TaggedGameObjects m_game_objects;
  //UniquePtr<ComponentsArray>   m_components;
  Transform* m_root;
  bool m_is_active;

public:
  Scene() = delete;
  explicit Scene(const String& name, Object* parent = nullptr);
  ~Scene() override;

  Transform* root();

  void setRoot(Transform* root);

  [[nodiscard]]
  GameObject* createGameObject(const String& name, const String& tag);

  //virtual Component* addComponent(GameObject* gameObject, Component* component);

  [[nodiscard]]
  Array<GameObject*> gameObjects() const;

  [[nodiscard]]
  GameObject* mainCamera() const;

  [[nodiscard]]
  bool isActive() const;

  void setActive(bool status);
};

#endif  /* !MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_SCENE_HPP */
