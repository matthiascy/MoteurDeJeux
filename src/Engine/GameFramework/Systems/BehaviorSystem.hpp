#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_SYSTEMS_BEHAVIOR_SYSTEM_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_SYSTEMS_BEHAVIOR_SYSTEM_HPP

#include <GameFramework/System.hpp>

class BehaviorSystem : public System {

public:
  BehaviorSystem(const String& name, Engine* engine, Object* parent = nullptr);
  ~BehaviorSystem() override = default;

  void init() override;

  void fixedUpdate(Real dt) override;

  void preUpdate(Real dt) override;

  void update(Real dt) override;

  void postUpdate(Real dt) override;


};


#endif  /** !MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_SYSTEMS_BEHAVIOR_SYSTEM_HPP */
