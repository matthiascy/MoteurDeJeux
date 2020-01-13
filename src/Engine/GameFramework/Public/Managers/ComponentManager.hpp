#ifndef MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_MANAGERS_COMPONENT_MANAGER_HPP
#define MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_MANAGERS_COMPONENT_MANAGER_HPP

#include <Core/Public/Core.hpp>
#include <GameFramework/Public/Engine.hpp>
#include <GameFramework/Public/ECS/Component.hpp>
#include <GameFramework/Public/ECS/GameObject.hpp>
#include <GameFramework/Public/Types.hpp>
#include <Physics/Public/Collider.hpp>
#include <Physics/Public/RigidBody.hpp>
#include <Physics/Public/Colliders.hpp>
#include <GameFramework/Public/Components/Transform.hpp>

// TODO::using handle to retrieve components

class ComponentManager : public Object {

  Q_OBJECT

  /* Component Manager owns components. */
  using ComponentContainer = Array<Component*>;
  using ComponentTable     = HashMap<ComponentTypeID, ComponentContainer*>;
  using GameObjectComponentTable = HashMap<GameObject*, ComponentTable*>;

private:
  /* All components are stored here. */
  ComponentTable m_components;
  /* It contains only the reference to the components and gameobjects. */
  GameObjectComponentTable m_game_object_components;
  Engine* m_engine;

public:
  ComponentManager(const String& name, Engine* engine, Object* parent = nullptr);
  ~ComponentManager() override;

  template <typename T, typename... Args>
  T* addComponent(const String& name, GameObject* gameObject, Args&&... params);

  template <typename T>
  void removeComponent(GameObject* gameObject);

  void removeAllComponents(GameObject* gameObject);

  template <typename T>
  T* getComponent(GameObject* gameObject);
};

template <typename T, typename... Args>
T* ComponentManager::addComponent(const String& name, GameObject* gameObject, Args&&... params) {
  // TODO::specify names(name generator)
  T* component = nullptr;
  if constexpr (std::is_base_of_v<Collider, T>) {
    component = new T(name, gameObject, m_engine->physicsSystem()->physicsWorld(),
                      gameObject->getComponent<RigidBody>(), std::forward<Args>(params)...);
    gameObject->setHasCollider(true);
  } else if constexpr (std::is_base_of_v<RigidBody, T>) {
    component = new T(name, gameObject, m_engine->physicsSystem()->physicsWorld(),
                      std::forward<Args>(params)...);
  } else {
    component = new T(name, gameObject, std::forward<Args>(params)...);
  }

  gameObject->addComponent(component);

  ComponentTypeID type = Component::family::type<T>;

  if (m_components.contains(type)) {
    m_components[type]->push_back(component);
  } else {
    m_components.insert(type, new ComponentContainer);
    m_components[type]->push_back(component);
  }

  if (m_game_object_components.contains(gameObject)) {
    auto* table = m_game_object_components[gameObject];
    if (table->contains(type)) {
      (*table)[type]->push_back(component);
    } else {
      table->insert(type, new ComponentContainer);
      (*table)[type]->push_back(component);
    }

  } else {

    m_game_object_components.insert(gameObject, new ComponentTable);
    m_game_object_components[gameObject]->insert(type, new ComponentContainer);
    (*m_game_object_components[gameObject])[type]->push_back(component);

  }

  return component;
}

template <typename T>
void ComponentManager::removeComponent(GameObject* gameObject)
{
  if (!gameObject) return;

  const ComponentTypeID type = T::componentTypeID();

  if (!m_components.contains(type)) return;
  if (!m_game_object_components.contains(gameObject)) return;

  ComponentContainer* components = m_components[type];
  int i = 0;
  for (; i < components->size(); ++i) {
    if ((*components)[i]->m_game_object == gameObject) {
      delete (*components)[i];
      (*components)[i] = nullptr;
      break;
    }
  }
  components->remove(i);

  if (m_game_object_components[gameObject]->contains(type)) {
    ComponentContainer* comps = (*m_game_object_components[gameObject])[type];
    for (i = 0; i < components->size(); ++i) {
      if ((*comps)[i]->m_game_object == gameObject) {
        break;
      }
    }
    comps->remove(i);
  }
}

template <typename T>
T* ComponentManager::getComponent(GameObject* gameObject)
{
  if (!gameObject) return nullptr;

  ComponentTypeID type = T::componentTypeID();

  if (m_game_object_components.contains(gameObject)) {
    if (m_game_object_components[gameObject]->contains(type)) {
      return (*m_game_object_components[gameObject])[type]->first();
    } else {
      return nullptr;
    }
  }
  return nullptr;
}


#endif  /* !MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_MANAGERS_COMPONENT_MANAGER_HPP */
