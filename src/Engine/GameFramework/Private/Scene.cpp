#include <GameFramework/Scene.hpp>
#include <GameFramework/Assets/AssetManager.hpp>
#include <GameFramework/Systems/RenderSystem.hpp>
#include <GameFramework/Component.hpp>
#include <GameFramework/Systems/PhysicsSystem.hpp>
#include <utility>
#include "Camera.hpp"

Scene::Scene(String name)
  : Object(std::move(name)), m_vbo_idx{0}
{
  m_game_objects = makeUnique<TaggedGameObjects>();
  m_game_objects->insert("Untagged", new GameObjectsArray());
  m_game_objects->insert("Camera", new GameObjectsArray());
  m_root = createGameObject("Root", "Untagged")->transform();
}

Scene::~Scene()
{
  //m_asset_manager.reset();
  //m_renderer_sys.reset();
  //m_physics_sys.reset();
  //m_input_sys.reset();

  for (auto& comp : *m_components)
    delete comp;
  m_components.reset();

  for (auto& objs : *m_game_objects) {
    for (auto& obj : *objs) {
      delete obj;
    }
    objs->clear();
    delete objs;
  }
  m_game_objects->clear();
  m_game_objects.reset();
}

Transform* Scene::root()
{
  return m_root;
}

GameObject* Scene::createGameObject(String name, const String& tag)
{
  auto obj = new GameObject(std::move(name), this, tag);

  if (!contains(*m_game_objects, tag)) {
    m_game_objects->operator[](tag) = new GameObjectsArray();
  }

  m_game_objects->operator[](tag)->push_back(obj);

  return obj;
}

Array<GameObject*> Scene::gameObjects() const
{
  Array<GameObject*> ret;
  for (auto& objs : *m_game_objects) {
    for (auto& obj : *objs) {
      ret.push_back(obj);
    }
  }
  return ret;
}

GameObject* Scene::mainCamera() const
{
  return m_game_objects->operator[]("Camera")->operator[](0);
}
