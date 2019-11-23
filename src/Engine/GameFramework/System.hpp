#ifndef MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_SYSTEM_HPP
#define MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_SYSTEM_HPP

#include <Core/Core.hpp>

//class Scene;
class Engine;

class System : public Object {
public:
  System(String name, Engine* scene);
  virtual void init() = 0;

protected:
  Engine* m_engine;
};


#endif  /* !MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_SYSTEM_HPP */
