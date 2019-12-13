#include <GameFramework/Scene.hpp>
#include <GameFramework/GameObject.hpp>
#include <GameFramework/Engine.hpp>
#include <GameFramework/Assets/AssetManager.hpp>
#include <GameFramework/Systems/RenderSystem.hpp>
#include <GameFramework/Component.hpp>
#include <GameFramework/Systems/PhysicsSystem.hpp>

Scene::Scene(const String& name, Engine* engine, Object* parent)
  : Object(name, parent), m_game_objects{}, m_root{}
{
  m_game_objects.insert("Untagged", new GameObjectsArray());
  m_game_objects.insert("Camera", new GameObjectsArray());
  //m_root = createGameObject("Root", "Untagged")->transform();
  m_is_active = false;
  m_engine = engine;
}

Scene::~Scene()
{
  for (auto& objs : m_game_objects) {
    for (auto& obj : *objs)
      delete obj;
    delete objs;
  }
}

Transform* Scene::root()
{
  return m_root;
}

void Scene::setRoot(Transform* root)
{
  m_root = root;
}

GameObject* Scene::createGameObject(const String& name, const String& tag)
{
  auto obj = new GameObject(name, this, tag, nullptr);

  m_engine->componentManager()->addComponent<Transform>(name + "-transform", obj, nullptr, Math::Zero, Math::QuatIdentity, Math::mkVec3(1));

  if (!m_game_objects.contains(tag)) {
    m_game_objects.insert(tag, new GameObjectsArray());
  }

  m_game_objects[tag]->push_back(obj);

  return obj;
}

Array<GameObject*> Scene::gameObjects() const
{
  Array<GameObject*> ret;
  for (auto& objs : m_game_objects) {
    for (auto& obj : *objs) {
      ret.push_back(obj);
    }
  }
  return ret;
}

GameObject* Scene::mainCamera() const
{
  return m_game_objects["Camera"]->at(0);
}

bool Scene::isActive() const
{
  return m_is_active;
}

void Scene::setActive(bool status)
{
  m_is_active = status;
}

GameObject* Scene::find(const String& name)
{
  for (auto i = m_game_objects.begin(); i != m_game_objects.end(); ++i) {
    for (auto j : *i.value()) {
      if (name == j->objectName())
        return j;
    }
  }
  return nullptr;
}
