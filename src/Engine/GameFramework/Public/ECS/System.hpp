#ifndef MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_SYSTEM_HPP
#define MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_SYSTEM_HPP

#include <Core/Public/Core.hpp>

//class Scene;
class Engine;

class System : public Object {

  Q_OBJECT

public:
  System(const String& name, Engine* scene, Object* parent = nullptr);

  ~System() override = default;

  virtual void init() = 0;

  virtual void fixedUpdate(Real dt) = 0;

  virtual void preUpdate(Real dt) = 0;

  virtual void update(Real dt) = 0;

  virtual void postUpdate(Real dt) = 0;

protected:
  Engine* m_engine;
};


#endif  /* !MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_SYSTEM_HPP */
