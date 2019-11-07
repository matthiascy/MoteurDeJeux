#include "SceneManager.hpp"
#include "Terrain.hpp"

NameGenerator SceneManager::m_game_object_name_generator("GameObject");

SceneManager::SceneManager()
  : m_scene_root{nullptr}, m_cameras{}, m_scene_nodes{}, m_object_collections{}, m_vbo_index{0}
{
  m_object_collections["GameObject"] = new GameObjectMap;
  m_object_collections["Light"] = new GameObjectMap;
}

SceneManager::~SceneManager()
{
  destroyAllCameras();
  clearScene();
  {
    for (auto& i : m_object_collections) {
      delete i;
      i = nullptr;
    }
    m_object_collections.clear();
  }
}

void SceneManager::updateSceneGraph()
{
  rootSceneNode()->update();
}

void SceneManager::clearScene()
{
  destroyAllGameObjects();
  rootSceneNode()->removeAllChildren();
  rootSceneNode()->detachAllObjects();
  delete m_scene_root;

  for (auto& i : m_scene_nodes) {
    delete i;
    i = nullptr;
  }

  m_scene_nodes.clear();
}

Camera* SceneManager::activatedCamera() const
{
  // TODO
  return nullptr;
}

Camera* SceneManager::createCamera(const String& name)
{
  // camera de nom "name" existe
  if (m_cameras.find(name) != m_cameras.end()) {

    return nullptr;

  } else {

    Camera* cam = new Camera(name);
    m_cameras.insert(cam->name(), cam);
    return cam;

  }
}

Camera* SceneManager::camera(const String& name) const
{
  auto i = m_cameras.find(name);
  if (i == m_cameras.end()) return nullptr;
  else return i.value();
}

bool SceneManager::hasCamera(const String& name) const
{
  return (m_cameras.find(name) != m_cameras.end());
}

void SceneManager::destroyCamera(Camera *cam)
{
  destroyCamera(cam->name());
}

void SceneManager::destroyCamera(const String &name)
{
  auto i = m_cameras.find(name);
  if (i != m_cameras.end()) {
    delete i.value();
    i.value() = nullptr;
    m_cameras.erase(i);
  }
}

void SceneManager::destroyAllCameras()
{
  for (auto& i : m_cameras) {
    delete i;
    i = nullptr;
  }

  m_cameras.clear();
}

SceneNode* SceneManager::createSceneNode()
{
  auto* node = new SceneNode();
  m_scene_nodes[node->name()] = node;
  return node;
}

SceneNode* SceneManager::createSceneNode(const String& name)
{
  // Node de nom "name" existe deja
  auto i = m_scene_nodes.find(name);
  if (i != m_scene_nodes.end()) {
    return i.value();
  }

  auto* node = new SceneNode(name);
  m_scene_nodes[name] = node;
  return node;
}

SceneNode* SceneManager::createSceneNode(const String& name, Node* parent)
{
  auto* node = createSceneNode(name);
  node->setParent(parent);
  return node;
}

SceneNode* SceneManager::rootSceneNode()
{
  if (!m_scene_root) {
    m_scene_root = createSceneNode("SceneRoot");
  }

  return m_scene_root;
}

SceneNode* SceneManager::sceneNode(const String& name) const
{
  if (hasSceneNode(name)) {
    return m_scene_nodes[name];
  } else {
    return nullptr;
  }
}

bool SceneManager::hasSceneNode(const String &name) const
{
  return (m_scene_nodes.find(name) != m_scene_nodes.end());
}

void SceneManager::destroySceneNode(const String& name)
{
  auto iter = m_scene_nodes.find(name);
  if (iter != m_scene_nodes.end()) {
    iter.value()->detachAllObjects();

    Node* parent = iter.value()->parent();
    if (parent) {
      parent->removeChild(iter.value());
    }

    delete iter.value();
      m_scene_nodes.erase(iter);
  }
}

void SceneManager::destroySceneNode(SceneNode* node)
{
  destroySceneNode(node->name());
}

GameObject* SceneManager::gameObject(const String & name, const String & type)
{
  if (type == "Camera") {
    return camera(name);
  }

  const GameObjectMap* objectMap = gameObjectsOfType(type);

  {
    const auto& i = objectMap->find(name);
    if (i == objectMap->end()) {
      return nullptr;
    } else {
      return i.value();
    }
  }

}

GameObject* SceneManager::createGameObject(const String& name, const String& type)
{
  // TODO error
  qDebug() << "Object creation: \n" << "  name: " << name << "\n" << "  type: " << type;
  if (type == "Camera") {
    return createCamera(name);
  }

  GameObjectMap* objects = m_object_collections[type];

  if (objects == nullptr) {
    qDebug() << "Objects of type" << type << " doesn't exist. ::Creation";
    objects = new GameObjectMap();
  } else {
    qDebug() << "Objects of type- " << type << " does exist. ::FindObjectByName";
    if (objects->find(name) != objects->end()) {
      // name existe
      qFatal("Object already exists.");
      return nullptr;
    }
  }
  qDebug() << "Objects is null ?" << (objects == nullptr);
  // TODO GameObject factory
  GameObject* obj = new GameObject(name, type);
  qDebug() << "Null object ?" << (obj == nullptr);
  objects->insert(name, obj);
  qDebug() << "Insertion success.";
  return obj;
}

GameObject* SceneManager::createGameObject(const String& type)
{
  return createGameObject(m_game_object_name_generator.generate(), type);
}

bool SceneManager::hasGameObject(const String& name, const String& type)
{
  if (type == "Camera")
    return hasCamera(name);

  auto i = m_object_collections.find(type);
  if (i == m_object_collections.end())
    return false;

  return (i.value()->find(name) != i.value()->end());
}

Array<const GameObject*> SceneManager::gameObjects() const
{
  Array<const GameObject*> ret;
  for (const auto& objs : m_object_collections) {
    for (const auto& obj : *objs) {
      ret.push_back(obj);
    }
  }

  return ret;
}

void SceneManager::destroyGameObject(const String& name, const String& type)
{
  if (type == "Camera") {
    destroyCamera(name);
    return;
  }

  auto* objs = gameObjectsOfType(type);

  auto i = objs->find(name);
  if (i != objs->end()) {
    // TODO Factory destory
    delete i.value();
    i.value() = nullptr;
    m_object_collections[type]->erase(i);
  }
}

void SceneManager::destroyGameObject(GameObject* obj)
{
  destroyGameObject(obj->name(), obj->type());
}

void SceneManager::destroyAllGameObjectsByType(const String& type)
{
  if (type == "Camera") {
    destroyAllCameras();
    return;
  }

  auto* objs = gameObjectsOfType(type);

  for (auto i = objs->begin(); i != objs->end(); ++i) {
    // TODO factory destroy
    delete i.value();
    i.value() = nullptr;
  }
  objs->clear();
}

void SceneManager::destroyAllGameObjects()
{
  for (auto& i : m_object_collections) {
    // TODO factory destroy
    for (auto& j : *i) {
      delete j;
      j = nullptr;
    }
    i->clear();
  }
}

SceneManager::GameObjectMap* SceneManager::gameObjectsOfType(const String& type)
{
  const auto i = m_object_collections.find(type);
  if (i == m_object_collections.end()) {
    auto* objs = new GameObjectMap;
    m_object_collections[type] = objs;
    return objs;
  } else {
    return i.value();
  }
}

const SceneManager::GameObjectMap* SceneManager::gameObjectsOfType(const String& type) const
{
  const auto i = m_object_collections.find(type);
  if (i == m_object_collections.end()) {
    return nullptr;
  } else {
    return i.value();
  }
}

Light* SceneManager::createLight(const String& name, ELightType type)
{
  // TODO
  return nullptr;
}

Light* SceneManager::createLight(ELightType type)
{
  // TODO
  return nullptr;
}

Light* SceneManager::light(const String &name)
{
  // TODO
  return nullptr;
}

Array<GameObject*> SceneManager::toBeRenderedGameObjects() const
{
  Array<GameObject*> toBeDrew;
  for (const GameObjectMap* objs : m_object_collections) {
    for (const auto& obj : *objs) {
      //qDebug() << "Object: " << obj->name() << "\n" << "isAttached: " << obj->isSceneNodeAttached() << "\n";
      if (obj->isVisible() && obj->isSceneNodeAttached()) {
        toBeDrew.push_back(obj);
      }
    }
  }

  return toBeDrew;
}

void SceneManager::addGameObject(GameObject* obj)
{
  if (obj->type() == "Camera") {
    for (const auto& cam : m_cameras) {
      if (cam == obj) {
        return;
      } else {
        m_cameras.insert(obj->name(), dynamic_cast<Camera*>(obj));
      }
    }
  } else {
    const auto i = m_object_collections.find(obj->type());
    if (i != m_object_collections.end()) {
      for (const auto& o : *m_object_collections[obj->type()]) {
        if (o == obj) {
          return;
        } else {
          m_object_collections[obj->type()]->insert(obj->name(), obj);
          obj->setVboIndex(m_vbo_index++);
        }
      }
    } else {
      m_object_collections[obj->type()] = new GameObjectMap;
      m_object_collections[obj->type()]->insert(obj->name(), obj);
      obj->setVboIndex(m_vbo_index++);
    }
  }
}

void SceneManager::removeGameObject(GameObject* obj)
{
  if (obj->type() == "Camera") {
    m_cameras.remove(obj->name());
  } else {
    m_object_collections[obj->type()]->remove(obj->name());
    obj->setVboIndex(-1);
    m_vbo_index--;
  }
}

Array<SceneNode*> SceneManager::toBeRenderedSceneNodes() const
{
  Array<SceneNode*> toBeDrew;
  for (const auto& node : m_scene_nodes) {
    //qDebug() << "Node" << node->name() << "isVisible?" << node->isVisiable();
    if (node->isVisiable() && node->attachedObjectsCount() != 0) {
        toBeDrew.push_back(node);
    }
  }

  return toBeDrew;
}
