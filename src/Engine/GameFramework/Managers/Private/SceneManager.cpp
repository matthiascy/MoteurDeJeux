#include <GameFramework/Managers/SceneManager.hpp>
#include <GameFramework/Scene.hpp>
#include <GameFramework/Engine.hpp>
#include <GameFramework/GameObject.hpp>

SceneManager::SceneManager(const String& name, Engine* engine, Object* parent)
  : Object(name, parent), m_scenes{}, m_engine{engine}
{
  qInfo() << "Creation =>" << objectName();
}

SceneManager::~SceneManager()
{
  qDebug() << "Shutting down...";
  for (auto* scene : m_scenes)
      delete scene;
  qDebug() << "Shutting down...[Finished]";
}

SceneHandle SceneManager::createScene(const String& name)
{
  auto* scene = new Scene(name, m_engine);
  auto* rootGameObject = scene->createGameObject("Root", "Untagged");
  scene->setRoot(rootGameObject->transform());
  m_scenes.push_back(scene);
  return {static_cast<UInt32>(m_scenes.size() - 1) };
}

SceneHandle SceneManager::activatedScene()
{
  for (int i = 0; i < m_scenes.size(); ++i) {
    if (m_scenes[i]->isActive())
      return {(UInt32)i};
  }
}

Scene *SceneManager::sceneAt(SceneHandle handle)
{
  if (handle.idx < m_scenes.size())
    return m_scenes[handle.idx];
}

SceneHandle SceneManager::loadScene(const String& name)
{
  // TODO::load scene from file
  qWarning("Not implemented!");
  return {};
}

void SceneManager::setActiveScene(SceneHandle handle)
{
  for (int i = 0; i < m_scenes.size(); ++i) {
    m_scenes[i]->setActive(handle.idx == i);
  }
}
