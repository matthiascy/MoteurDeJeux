#ifndef MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_SYSTEM_HPP
#define MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_SYSTEM_HPP

#include <Engine/Core/Core.hpp>

class Scene;

class System : Object {
public:
  System(String name, Scene* scene);
  virtual void init() = 0;

protected:
  Scene* m_scene;
};


#endif  /* !MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_SYSTEM_HPP */
