#ifndef HOMELAND_HOMELAND_BEHAVIORS_HPP
#define HOMELAND_HOMELAND_BEHAVIORS_HPP

#include <Engine/Core/Public/Core.hpp>

class GameObject;
class Engine;

struct HomelandBehaviors {

  static QTimer *timer;

  /** Camera  behaviors */
  static void trackball(GameObject* self, Engine* engine, Real dt);

  static void freeView(GameObject* self, Engine* engine, Real dt);

  static void fpsView(GameObject* self, Engine* engine, Real dt);

  static void bodyBehavior(GameObject* self, Engine* engine, Real dt);

  static void canonicalBehavior(GameObject* self, Engine* engine, Real dt);

  static void exampleBehavior000(GameObject* self, Engine* engine, Real dt);

  static void exampleBehavior001(GameObject* self, Engine* engine, Real dt);

  static void handGun(GameObject* self, Engine* engine, Real dt);

  static void crateMgrBehavior(GameObject* self, Engine* engine, Real dt);

};


#endif  /* !HOMELAND_HOMELAND_BEHAVIORS_HPP */
