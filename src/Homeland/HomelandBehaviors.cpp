#include "HomelandBehaviors.hpp"
#include <Engine/GameFramework/Engine.hpp>
#include <Engine/GameFramework/GameObject.hpp>

void HomelandBehaviors::freeView(GameObject* self, Engine* engine, Real dt)
{
  // Pan - up
  if (engine->inputSystem()->isKeyPressed(Qt::Key_Up)) {
    self->transform()->translate(self->transform()->up() * dt, ESpace::World);
  }

  // Pan - down
  if (engine->inputSystem()->isKeyPressed(Qt::Key_Down)) {
    self->transform()->translate(self->transform()->up() * -dt, ESpace::World);
  }

  // Pan - left
  if (engine->inputSystem()->isKeyPressed(Qt::Key_Left)) {
    self->transform()->translate(self->transform()->right() * -dt, ESpace::World);
  }

  // Pan - right
  if (engine->inputSystem()->isKeyPressed(Qt::Key_Right)) {
    self->transform()->translate(self->transform()->right() * dt, ESpace::World);
  }

  // Free view
  if (engine->inputSystem()->isMouseButtonPressed(Qt::MouseButton::MidButton)) {
    self->transform()->rotate({static_cast<float>(-engine->inputSystem()->mouseDelta().y())*0.1f,static_cast<float>(-engine->inputSystem()->mouseDelta().x())*0.1f, 0}, ESpace::Local);
  }

  // Zoom
  self->transform()->translate(self->transform()->forward() * -dt*engine->inputSystem()->verticalScroll(), ESpace::World);
}

void HomelandBehaviors::trackball(GameObject* self, Engine* engine, Real dt)
{
  if (engine->inputSystem()->isMouseButtonPressed(Qt::MouseButton::LeftButton)) {
    self->transform()->rotate(Math::Up, engine->inputSystem()->mouseDelta().x() * 0.1f, ESpace::Local);
    self->transform()->rotate(Math::Right, engine->inputSystem()->mouseDelta().y() * 0.1f, ESpace::Local);
  }
}




void HomelandBehaviors::exampleBehavior(GameObject* self, Engine* engine, Real dt)
{
  if (engine->inputSystem()->isKeyPressed(Qt::Key_W)) {
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
