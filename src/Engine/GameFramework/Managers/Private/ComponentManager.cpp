#include <GameFramework/Managers/ComponentManager.hpp>
#include <GameFramework/GameObject.hpp>

ComponentManager::ComponentManager(const String &name, Object *parent)
  : Object(name, parent)
{
  qInfo() << "Component Manager creation =>" << objectName();
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
    for (AbstractComponent* c : *i.value()) {
      delete c;
      c = nullptr;
    }
    i.value()->removeAll(nullptr);
  }
}
