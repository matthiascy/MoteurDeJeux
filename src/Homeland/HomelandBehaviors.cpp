#include "HomelandBehaviors.hpp"
#include <Engine/GameFramework/Engine.hpp>
#include <Engine/GameFramework/GameObject.hpp>

void HomelandBehaviors::cameraBehavior(GameObject *self, Engine *engine, Real dt)
{
  if (engine->inputSystem()->isKeyPressed(Qt::Key_Up)) {
    self->transform()->translateWorld(0, 0, -dt);
  }

  if (engine->inputSystem()->isKeyPressed(Qt::Key_Down)) {
    self->transform()->translateWorld(0, 0, dt);
  }

  if (engine->inputSystem()->isKeyPressed(Qt::Key_Left)) {
    self->transform()->translateWorld(-dt, 0, 0);
  }

  if (engine->inputSystem()->isKeyPressed(Qt::Key_Right)) {
    self->transform()->translateWorld(dt, 0, 0);
  }

  self->transform()->lookAt({0, 0, 0}, Math::Up);
}

void HomelandBehaviors::exampleBehavior(GameObject* self, Engine *engine, Real dt)
{
  if (engine->inputSystem()->isKeyPressed(Qt::Key_I)) {
    self->transform()->translateWorld({0, dt, 0});
  }

  if (engine->inputSystem()->isKeyPressed(Qt::Key_W)) {
    self->transform()->translateWorld(0, 0, -2 * dt);
  }

  if (engine->inputSystem()->isKeyPressed(Qt::Key_S)) {
    self->transform()->translateWorld(0, 0, 2 * dt);
  }

  if (engine->inputSystem()->isKeyPressed(Qt::Key_A)) {
    self->transform()->translateWorld(-dt * 2, 0, 0);
  }

  if (engine->inputSystem()->isKeyPressed(Qt::Key_D)) {
    self->transform()->translateWorld(dt * 2, 0, 0);
  }

  if (engine->inputSystem()->isKeyPressed(Qt::Key_R)) {
    self->transform()->rotateWorld(0, 0, 30*dt);
  }
}
