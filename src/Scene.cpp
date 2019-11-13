#include "Scene.hpp"
#include "Camera.hpp"
#include "Terrain.hpp"

Scene::Scene()
  : m_root{nullptr}, m_cameras{}, m_collections{}, m_vbo_index{0}
{
  m_name_generator = memory::mkUnique<NameGenerator>("SceneGameObject");
}

Scene::~Scene()
{
  removeAllCameras();
  clearScene();
  {
    for (auto& objs : m_collections) {
      for (auto& i : *objs) {
        delete i;
      }
      objs->clear();
      delete objs;
    }
    m_collections.clear();
  }
}

void Scene::clearScene()
{
  removeAllGameObjects();
  delete m_root;
}

Camera* Scene::activatedCamera() const
{
  // TODO
  return nullptr;
}

Camera* Scene::createCamera(const String& name)
{
  // camera de nom "name" existe
  if (m_cameras.find(name) != m_cameras.end()) {

    return nullptr;

  } else {

    auto* cam = new Camera(name);
    m_cameras.insert(cam->name(), cam);
    return cam;
  }
}

Camera* Scene::camera(const String& name) const
{
  auto i = m_cameras.find(name);
  if (i == m_cameras.end()) return nullptr;
  else return i.value();
}

bool Scene::hasCamera(const String& name) const
{
  return (m_cameras.find(name) != m_cameras.end());
}

void Scene::removeCamera(Camera *cam)
{
  removeCamera(cam->name());
}

void Scene::removeCamera(const String &name)
{
  auto i = m_cameras.find(name);
  if (i != m_cameras.end()) {
    delete i.value();
    i.value() = nullptr;
    m_cameras.erase(i);
  }
}

void Scene::removeAllCameras()
{
  for (auto& i : m_cameras) {
    delete i;
    i = nullptr;
  }

  m_cameras.clear();
}

GameObject* Scene::root()
{
  if (!m_root) {
    m_root = createGameObject("SceneRoot", "GameObject");
  }

  return m_root;
}

GameObject* Scene::gameObject(const String& name, const String& type) {
  if (type == "Camera") {
    return camera(name);
  }

  if (m_collections[type] == nullptr) {
    return nullptr;
  } else {
    for (GameObject* obj : *m_collections[type]) {
      if (obj->name() == name) {
        return obj;
      }
    }
    return nullptr;
  }
}

GameObject* Scene::createTerrain(UInt32 width, UInt32 height, const Image & hmap, Real baseAlt)
{
  GameObjectMap* objects = m_collections["Terrain"];
  if (objects == nullptr) {
    objects = new GameObjectMap();
  } else {
    if (objects->find("") != objects->end()) {
      return nullptr;
    }
  }
  auto* terrain = new Terrain(m_name_generator->generate(), "Terrain");
  terrain->create(1023, 1023, Image(":/Textures/heightmap"), false, 100.0f, 100.0f);
  objects->insert(terrain->name(), terrain);
  return terrain;
}

GameObject* Scene::createGameObject(const String& name, const String& type)
{
  // TODO error
  qDebug() << "Object creation: \n" << "  name: " << name << "\n" << "  type: " << type;
  if (type == "Camera") {
    return createCamera(name);
  }

  GameObjectMap* objects = m_collections[type];

  if (objects == nullptr) {
    qDebug() << "Objects of type" << type << " doesn't exist. ::Creation";
    objects = new GameObjectMap();
  } else {
    qDebug() << "Objects of type" << type << " does exist. ::FindObjectByName";
    if (objects->find(name) != objects->end()) {
      // name existe
      qFatal("Object already exists.");
      return nullptr;
    }
  }

  // TODO GameObject factory
  auto* obj = new GameObject(name, type);
  qDebug() << "Null object ?" << (obj == nullptr);
  objects->insert(name, obj);
  qDebug() << "Insertion success.";
  return obj;
}

bool Scene::hasGameObject(const String& name, const String& type)
{
  if (type == "Camera")
    return hasCamera(name);

  auto i = m_collections.find(type);
  if (i == m_collections.end())
    return false;

  return (i.value()->find(name) != i.value()->end());
}

Array<const GameObject*> Scene::gameObjects() const
{
  Array<const GameObject*> ret;
  for (const auto& objs : m_collections) {
    for (const auto& obj : *objs) {
      ret.push_back(obj);
    }
  }
  return ret;
}

void Scene::removeGameObject(const String& name, const String& type)
{
  if (type == "Camera") {
    removeCamera(name);
    return;
  }

  auto* objs = gameObjectsOfType(type);

  auto i = objs->find(name);
  if (i != objs->end()) {
    // TODO Factory destory
    delete i.value();
    i.value() = nullptr;
    m_collections[type]->erase(i);
    m_vbo_index--;
  }
}

void Scene::removeGameObject(GameObject* obj)
{
  removeGameObject(obj->name(), obj->type());
}

void Scene::removeAllGameObjectsByType(const String& type)
{
  if (type == "Camera") {
    removeAllCameras();
    return;
  }

  auto* objs = gameObjectsOfType(type);

  for (auto i = objs->begin(); i != objs->end(); ++i) {
    // TODO factory remove
    delete i.value();
    i.value() = nullptr;
  }
  objs->clear();
}

void Scene::removeAllGameObjects()
{
  for (auto& i : m_collections) {
    // TODO factory remove
    for (auto& j : *i) {
      delete j;
    }
    i->clear();
  }
}

Scene::GameObjectMap* Scene::gameObjectsOfType(const String& type)
{
  const auto i = m_collections.find(type);
  if (i == m_collections.end()) {
    auto* objs = new GameObjectMap;
    m_collections[type] = objs;
    return objs;
  } else {
    return i.value();
  }
}

const Scene::GameObjectMap* Scene::gameObjectsOfType(const String& type) const
{
  const auto i = m_collections.find(type);
  if (i == m_collections.end()) {
    return nullptr;
  } else {
    return i.value();
  }
}

Array<GameObject*> Scene::toBeRenderedGameObjects() const
{
  Array<GameObject*> toBeDrew;
  for (const GameObjectMap* objs : m_collections) {
    for (const auto& obj : *objs) {
      if (obj->isVisible()) {
        toBeDrew.push_back(obj);
      }
    }
  }

  return toBeDrew;
}
