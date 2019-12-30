#include <Graphics/Public/AnimationSystem.hpp>
#include <GameFramework/Public/Managers.hpp>
#include <GameFramework/Public/Engine.hpp>
#include <Graphics/Public/AnimatedMeshRenderer.hpp>
#include <Graphics/Public/Animator.hpp>

AnimationSystem::AnimationSystem(const String& name, Engine* engine, Object* parent)
  : System(name, engine, parent)
{
  qInfo() << "Creation =>" << this->name();
}

AnimationSystem::~AnimationSystem()
{
  qDebug() << "Shutting down...";
  qDebug() << "Shutting down...[Finished]";
}

void AnimationSystem::init()
{

}

void AnimationSystem::fixedUpdate(Real dt)
{

}

void AnimationSystem::preUpdate(Real dt)
{
  qDebug() << "Prepare data";
  auto* scene = m_engine->sceneManager()->sceneAt(m_engine->sceneManager()->activatedScene());
  for (auto* gameObject : scene->gameObjects()) {
    auto* animatedMeshRenderer = gameObject->getComponent<AnimatedMeshRenderer>();
    if (animatedMeshRenderer) {
      auto* animator = animatedMeshRenderer->animator();
      if (!m_enabled_animators.contains(animator)) {
        m_enabled_animators.push_back(animatedMeshRenderer->animator());
        //animatedMeshRenderer->init(m_engine->renderSystem(), m_engine->assetManager());
      }
    }
  }
}

void AnimationSystem::update(Real dt)
{
  for (auto & m_enabled_animator : m_enabled_animators) {
    if (m_enabled_animator)
      m_enabled_animator->update(dt);
  }
}

void AnimationSystem::postUpdate(Real dt)
{

}
