#include <GameFramework/Scene.hpp>
#include <GameFramework/Engine.hpp>
#include <GameFramework/Assets/AssetManager.hpp>
#include <GameFramework/Systems/RenderSystem.hpp>
#include <GameFramework/Component.hpp>
#include <GameFramework/Systems/PhysicsSystem.hpp>

Scene::Scene(const String& name, Object* parent)
  : Object(name, parent)
{
  m_game_objects = makeUnique<TaggedGameObjects>();
  m_game_objects->insert("Untagged", new GameObjectsArray());
  m_game_objects->insert("Camera", new GameObjectsArray());
  m_root = createGameObject("Root", "Untagged")->transform();
  m_is_active = false;
}

Scene::~Scene()
{
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

GameObject* Scene::createGameObject(const String& name, const String& tag)
{
  auto obj = new GameObject(name, this, tag, nullptr);

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

bool Scene::isActive() const
{
  return m_is_active;
}

void Scene::setActive(bool status)
{
  m_is_active = status;
}
