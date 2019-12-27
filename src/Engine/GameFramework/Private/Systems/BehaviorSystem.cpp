#include <GameFramework/Public/Systems/BehaviorSystem.hpp>
#include <GameFramework/Public/Components/Behavior.hpp>
#include <GameFramework/Public/Engine.hpp>
#include <GameFramework/Public/Managers/SceneManager.hpp>
#include <GameFramework/Public/ECS/GameObject.hpp>

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
          behavior->invokeFixedUpdates(gameObject, m_engine, dt);
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
        behavior->invokeUpdates(gameObject, m_engine, dt);
      }
    }
  }
}

void BehaviorSystem::postUpdate(Real dt)
{ }
