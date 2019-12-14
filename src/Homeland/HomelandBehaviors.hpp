#ifndef HOMELAND_HOMELAND_BEHAVIORS_HPP
#define HOMELAND_HOMELAND_BEHAVIORS_HPP

#include <Engine/Core/Core.hpp>

class GameObject;
class Engine;

struct HomelandBehaviors {

  static void cameraBehavior(GameObject* self, Engine* engine, Real dt);

  static void exampleBehavior(GameObject* self, Engine* engine, Real dt);

};


#endif  /* !HOMELAND_HOMELAND_BEHAVIORS_HPP */
