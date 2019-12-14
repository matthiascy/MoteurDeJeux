#include "HomelandBehaviors.hpp"
#include <Engine/GameFramework/Engine.hpp>
#include <Engine/GameFramework/GameObject.hpp>

void HomelandBehaviors::cameraBehavior(GameObject* self, Engine* engine, Real dt)
{
  if (engine->inputSystem()->isKeyPressed(Qt::Key_Up)) {
    self->transform()->translateWorld({0, dt, 0});
  }

  if (engine->inputSystem()->isKeyPressed(Qt::Key_Down)) {
    self->transform()->translateWorld({0, -dt, 0});
  }

  if (engine->inputSystem()->isKeyPressed(Qt::Key_Left)) {
    self->transform()->translateWorld({-dt, 0, 0});
  }

  if (engine->inputSystem()->isKeyPressed(Qt::Key_Right)) {
    self->transform()->translateWorld({dt, 0, 0});
  }

  if (engine->inputSystem()->isMouseButtonPressed(Qt::MouseButton::LeftButton)) {
    self->transform()->rotate(0, engine->inputSystem()->mouseDelta().x(), 0, Transform::ESpace::World);
  }

  //self->transform()->lookAt({0, 0, 0}, self->transform()->up());
  qDebug() << "Cam world pos : " << self->transform()->worldPosition();
}

void HomelandBehaviors::exampleBehavior(GameObject* self, Engine* engine, Real dt)
{
  if (engine->inputSystem()->isKeyPressed(Qt::Key_W)) {
    self->transform()->translateWorld({0, 10 * dt, 0});
  }

  if (engine->inputSystem()->isKeyPressed(Qt::Key_S)) {
    self->transform()->translateWorld({0, -10 * dt, 0});
  }

  if (engine->inputSystem()->isKeyPressed(Qt::Key_A)) {
    self->transform()->translateWorld({-dt * 10, 0, 0});
  }

  if (engine->inputSystem()->isKeyPressed(Qt::Key_D)) {
    self->transform()->translateWorld({dt * 10, 0, 0});
  }

  if (engine->inputSystem()->isKeyPressed(Qt::Key_E)) {
    self->transform()->rotateLocal(0, 0, 90 * dt);
    //self->transform()->rotateWorld(0, 90 * dt, 0);
  }

  if (engine->inputSystem()->isKeyPressed(Qt::Key_Q)) {
    self->transform()->rotateWorld(0, -90 * dt, 0);
  }
}
