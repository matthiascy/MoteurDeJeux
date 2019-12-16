#include <GameFramework/Systems/BehaviorSystem.hpp>
#include <GameFramework/Components/Behavior.hpp>
#include <GameFramework/Engine.hpp>
#include <GameFramework/Managers/SceneManager.hpp>
#include <GameFramework/GameObject.hpp>

BehaviorSystem::BehaviorSystem(const String& name, Engine* engine, Object* parent)
  : System(name, engine, parent)
{
  qInfo() << "Creation =>" << objectName();
}

void BehaviorSystem::init()
{
  qInfo() << "Initialization...";
}

void BehaviorSystem::fixedUpdate(Real dt)
{
  auto scene = m_engine->sceneManager()->sceneAt(m_engine->sceneManager()->activatedScene());
  if (scene) {
    for (GameObject* gameObject : scene->gameObjects()) {
      if (!gameObject->isStatic()) {
        if (gameObject->hasComponent<Behavior>()) {
          auto *behavior = gameObject->getComponent<Behavior>();
          behavior->invokeFixedUpdate(gameObject, m_engine, dt);
        }
      }
    }
  }
}

void BehaviorSystem::preUpdate(Real dt)
{ }

void BehaviorSystem::update(Real dt)
{
  auto scene = m_engine->sceneManager()->sceneAt(m_engine->sceneManager()->activatedScene());
  if (scene) {
    for (GameObject* gameObject : scene->gameObjects()) {
      if (gameObject->hasComponent<Behavior>()) {
        auto* behavior = gameObject->getComponent<Behavior>();
        behavior->invokeUpdate(gameObject, m_engine, dt);
      }
    }
  }
}

void BehaviorSystem::postUpdate(Real dt)
{ }
