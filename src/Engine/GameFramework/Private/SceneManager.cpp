#include "GameFramework/SceneManager.hpp"

SceneManager::SceneManager(String name)
  : Object(std::move(name)), m_scenes{}
{
  qInfo() << "Scene Manager creation =>" << m_name;
}

SceneManager::~SceneManager()
{
  for (auto& s : m_scenes) {
    delete s;
    s = nullptr;
  }
}

SceneHandle SceneManager::createScene(String name)
{
  auto* s = new Scene(std::move(name));
  m_scenes.push_back(s);
  return {static_cast<UInt32>(m_scenes.size() - 1) };
}
