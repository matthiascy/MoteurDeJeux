#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_ANIMATION_SYSTEM_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_ANIMATION_SYSTEM_HPP

#include <GameFramework/Public/ECS/System.hpp>
#include <Graphics/Public/Forward.hpp>


class AnimationSystem : public System {
private:
  Array<Animator*> m_enabled_animators;

public:
  AnimationSystem(const String& name, Engine* engine, Object* parent = nullptr);

  ~AnimationSystem() override;

  void init() override;

  void fixedUpdate(Real dt) override;

  void preUpdate(Real dt) override;

  void update(Real dt) override;

  void postUpdate(Real dt) override;
};


#endif /* !MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_ANIMATION_SYSTEM_HPP */
