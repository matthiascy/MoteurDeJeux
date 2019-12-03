#include <GameFramework/Managers/SceneManager.hpp>

SceneManager::SceneManager(const String& name, Object* parent)
  : Object(name, parent), m_scenes{}
{
  qInfo() << "Scene Manager creation =>" << objectName();
}

SceneManager::~SceneManager()
{
  for (auto& s : m_scenes) {
    delete s;
    s = nullptr;
  }
}

SceneHandle SceneManager::createScene(const String& name)
{
  auto* s = new Scene(name);
  m_scenes.push_back(s);
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
  qWarning("SceneManager::loadScene not implemented!");
  return {};
}

void SceneManager::setActiveScene(SceneHandle handle)
{
  for (int i = 0; i < m_scenes.size(); ++i) {
    if (handle.idx == i)
      m_scenes[i]->setActive(true);
    else
      m_scenes[i]->setActive(false);
  }
}
