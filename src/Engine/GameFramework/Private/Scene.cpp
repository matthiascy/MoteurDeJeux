#include <Engine/GameFramework/Scene.hpp>
#include <Engine/Assets/AssetManager.hpp>
#include <Engine/GameFramework/Subsystems/RendererSystem.hpp>
#include <Engine/GameFramework/Subsystems/PhysicsSystem.hpp>
#include "Camera.hpp"
#include "Terrain.hpp"

Scene::Scene(String name)
  : Object(std::move(name)), m_vbo_idx{0}
{
  m_game_objects = makeUnique<TaggedGameObjects>();
  m_game_objects->insert("Untagged", makeUnique<GameObjectsArray>());
  m_game_objects->insert("Camera", makeUnique<GameObjectsArray>());
  m_root = createGameObject("Root", "Untagged")->transform();
}

Scene::~Scene()
{
  m_asset_manager.reset();
  m_renderer_sys.reset();
  m_physics_sys.reset();
  m_input_sys.reset();

  for (auto& comp : *m_components)
    comp.reset();
  m_components.reset();

  for (auto& objs : *m_game_objects) {
    for (auto& obj : *objs) {
      obj.reset();
    }
    objs.reset();
  }
  m_game_objects.reset();
}

Transform* Scene::root()
{
  return m_root;
}

GameObject* Scene::createGameObject(String name, String tag)
{
  auto obj = makeUnique<GameObject>(name, tag);

  if (!contains(*m_game_objects, tag)) {
    m_game_objects->operator[](tag) = makeUnique<GameObjectsArray>();
  }

  m_game_objects->operator[](tag)->push_back(obj);

  return obj.get();
}

Array<GameObject*> Scene::gameObjects() const
{
  Array<GameObject*> ret;
  for (auto& objs : *m_game_objects) {
    for (auto& obj : *objs) {
      ret.push_back(obj.get());
    }
  }
  return ret;
}
