#include "HomelandBehaviors.hpp"
#include <Engine/GameFramework/Engine.hpp>
#include <Engine/GameFramework/GameObject.hpp>

void HomelandBehaviors::cameraBehavior(GameObject* self, Engine* engine, Real dt)
{
  if (engine->inputSystem()->isKeyPressed(Qt::Key_Up)) {
    self->transform()->translate({0, dt, 0}, ESpace::World);
  }

  if (engine->inputSystem()->isKeyPressed(Qt::Key_Down)) {
    self->transform()->translate({0, -dt, 0}, ESpace::World);
  }

  if (engine->inputSystem()->isKeyPressed(Qt::Key_Left)) {
    self->transform()->translate({-dt, 0, 0}, ESpace::World);
  }

  if (engine->inputSystem()->isKeyPressed(Qt::Key_Right)) {
    self->transform()->translate({dt, 0, 0}, ESpace::World);
  }

  if (engine->inputSystem()->isMouseButtonPressed(Qt::MouseButton::MidButton)) {
    self->transform()->rotate({static_cast<float>(-engine->inputSystem()->mouseDelta().y())*0.1f, static_cast<float>(-engine->inputSystem()->mouseDelta().x())*0.1f, 0}, ESpace::Local);
  }

  //if (engine->inputSystem()->isMouseButtonPressed(Qt::MouseButton::LeftButton)) {
    //self->transform()->rotateAround({0, 0, 0}, Math::AxisY, dt);
    //self->transform()->lookAt({0, 0, 0}, self->transform()->up());
  //}

  if (engine->inputSystem()->isKeyPressed(Qt::Key_T)) {
    self->transform()->rotateAround({0, 0, 0}, Math::AxisY, dt);
    //self->transform()->lookAt({0, 0, 0}, self->transform()->up());
  }

  self->transform()->translate(self->transform()->forward() * -dt*engine->inputSystem()->verticalScroll(), ESpace::World);

  //self->transform()->lookAt({0, 0, 0}, self->transform()->up());
  //qDebug() << "Cam world pos : " << self->transform()->worldPosition();
  //qDebug() << "Wheel y " << engine->inputSystem()->verticalScroll();
}

void HomelandBehaviors::exampleBehavior(GameObject* self, Engine* engine, Real dt)
{
  if (engine->inputSystem()->isKeyPressed(Qt::Key_W)) {
    //qDebug() << "Pos local : " << self->transform()->localPosition();
    //qDebug() << "Pos world : " << self->transform()->worldPosition();
    self->transform()->translate({0, 10 * dt, 0}, ESpace::World);
  }

  if (engine->inputSystem()->isKeyPressed(Qt::Key_S)) {
    self->transform()->translate({0, -10 * dt, 0}, ESpace::World);
  }

  if (engine->inputSystem()->isKeyPressed(Qt::Key_A)) {
    self->transform()->translate({-dt * 10, 0, 0}, ESpace::World);
  }

  if (engine->inputSystem()->isKeyPressed(Qt::Key_D)) {
    self->transform()->translate({dt * 10, 0, 0}, ESpace::World);
  }
}
