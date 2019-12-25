#include "HomelandBehaviors.hpp"
#include <Engine/GameFramework/Public/Engine.hpp>
#include <Engine/GameFramework/Public/ECS/GameObject.hpp>

void HomelandBehaviors::freeView(GameObject* self, Engine* engine, Real dt)
{
  if (engine->inputSystem()->isMouseButtonPressed(Qt::MouseButton::MiddleButton)) {
    if (engine->inputSystem()->isKeyPressed(Qt::Key_Shift)) {

      // Pan
      self->transform()->translate(self->transform()->up() * dt * engine->inputSystem()->mouseDelta().y(), ESpace::World);
      self->transform()->translate(self->transform()->right() * dt * -engine->inputSystem()->mouseDelta().x(), ESpace::World);

    } else {

      // Free view
      self->transform()->rotate({static_cast<float>(-engine->inputSystem()->mouseDelta().y())*0.1f,static_cast<float>(-engine->inputSystem()->mouseDelta().x())*0.1f, 0}, ESpace::Local);

    }
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
