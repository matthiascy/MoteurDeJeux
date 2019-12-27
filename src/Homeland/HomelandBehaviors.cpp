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

void HomelandBehaviors::exampleBehavior000(GameObject* self, Engine* engine, Real dt)
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

  if (engine->inputSystem()->isKeyPressed(Qt::Key_Z)) {
    self->transform()->translate({0, 0, -dt*10}, ESpace::World);
  }

  if (engine->inputSystem()->isKeyPressed(Qt::Key_X)) {
    self->transform()->translate({0, 0, dt*10}, ESpace::World);
  }
}

void HomelandBehaviors::exampleBehavior001(GameObject* self, Engine* engine, Real dt)
{
  if (engine->inputSystem()->isKeyPressed(Qt::Key_Up)) {
    self->transform()->translate({0, 10 * dt, 0}, ESpace::World);
  }

  if (engine->inputSystem()->isKeyPressed(Qt::Key_Down)) {
    self->transform()->translate({0, -10 * dt, 0}, ESpace::World);
  }

  if (engine->inputSystem()->isKeyPressed(Qt::Key_Left)) {
    self->transform()->translate({-dt * 10, 0, 0}, ESpace::World);
  }

  if (engine->inputSystem()->isKeyPressed(Qt::Key_Right)) {
    self->transform()->translate({dt * 10, 0, 0}, ESpace::World);
  }

  if (engine->inputSystem()->isKeyPressed(Qt::Key_PageUp)) {
    self->transform()->translate({0, 0, -dt*10}, ESpace::World);
  }

  if (engine->inputSystem()->isKeyPressed(Qt::Key_PageDown)) {
    self->transform()->translate({0, 0, dt*10}, ESpace::World);
  }
}

void HomelandBehaviors::canonicalBehavior(GameObject* self, Engine* engine, Real dt)
{
    if (engine->inputSystem()->isKeyPressed(Qt::Key_W)) {
      self->transform()->translate(self->transform()->forward() * -40 * dt, ESpace::World);
    }

    if (engine->inputSystem()->isKeyPressed(Qt::Key_S)) {
      self->transform()->translate(self->transform()->forward() * 40 * dt, ESpace::World);
    }

    if (engine->inputSystem()->isKeyPressed(Qt::Key_A)) {
      self->transform()->translate(self->transform()->right() * -40 * dt, ESpace::World);
    }

    if (engine->inputSystem()->isKeyPressed(Qt::Key_D)) {
      self->transform()->translate(self->transform()->right() * 40 * dt, ESpace::World);
    }
}
