#include <GameFramework/Public/Managers/ComponentManager.hpp>
#include <GameFramework/Public/ECS/GameObject.hpp>

ComponentManager::ComponentManager(const String &name, Engine* engine, Object *parent)
  : Object(name, parent)
{
  qInfo() << "Creation =>" << objectName();
  m_engine = engine;
}

void ComponentManager::removeAllComponents(GameObject* gameObject)
{
  if (!gameObject)
    return;

  if (m_game_object_components.contains(gameObject)) {
    m_game_object_components[gameObject]->clear();
  }

  gameObject->getComponents().clear();

  ComponentTable::Iterator i = m_components.begin();
  for (; i != m_components.end(); ++i) {
    for (Component* c : *i.value()) {
      delete c;
      c = nullptr;
    }
    i.value()->removeAll(nullptr);
  }
}

ComponentManager::~ComponentManager()
{
  qDebug() << "Shutting down...";
  for (ComponentContainer* cs : m_components) {
    for (Component* c : *cs) {
      delete c; c = nullptr;
    }
  }
  qDebug() << "Shutting down...[Finished]";
}
