#include "HomelandBehaviors.hpp"
#include <QTimer>
#include <Engine/GameFramework/Public/Scene.hpp>
#include <Engine/GameFramework/Public/Engine.hpp>
#include <Engine/GameFramework/Public/ECS/GameObject.hpp>
#include <Engine/GameFramework/Public/Managers/SceneManager.hpp>
#include <Engine/GameFramework/Public/Managers/ComponentManager.hpp>
#include <Engine/Physics/Public/Colliders/SphereCollider.hpp>
#include <Engine/Graphics/Public/MeshRenderer.hpp>
#include <Engine/GameFramework/Public/Components/Behavior.hpp>
#include <Engine/Physics/Public/PhysicsWorld.hpp>

QTimer* HomelandBehaviors::timer = new QTimer(nullptr);

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
    qDebug() << "delta x : " << engine->inputSystem()->mouseDelta().x();
    qDebug() << "delta y : " << engine->inputSystem()->mouseDelta().y();
    self->transform()->rotate(Math::Up, engine->inputSystem()->mouseDelta().x() * dt * 10, ESpace::Local);
    self->transform()->rotate(Math::Right, engine->inputSystem()->mouseDelta().y() * dt * 10, ESpace::Local);
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

void HomelandBehaviors::fpsView(GameObject* self, Engine* engine, Real dt)
{
  // Free view
  auto delta = engine->inputSystem()->mouseDelta();

  self->transform()->rotate({-delta.y() * 15 * dt, 0, 0}, ESpace::Local);

  self->transform()->rotate({0, static_cast<float>(-delta.x()) * 20 * dt, 0}, ESpace::Local);

  if (engine->inputSystem()->isKeyPressed(Qt::Key_Space)) {
    self->transform()->setRotation(Math::QuatIdentity, ESpace::Local);
    self->transform()->rotate(Math::AxisY, -90, ESpace::World);
  }
}

void HomelandBehaviors::bodyBehavior(GameObject* self, Engine* engine, Real dt)
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

void HomelandBehaviors::handGun(GameObject* self, Engine* engine, Real dt) {
  static int i = 0;
  if (engine->inputSystem()->isMouseButtonTriggered(Qt::MouseButton::LeftButton)) {
    String name = QStringLiteral("bullet%1").arg(i++);
    qDebug() << "New bullet" << name;
    auto bullet = engine->sceneManager()->sceneAt(engine->sceneManager()->activatedScene())->createGameObject(name, "Bullet");
    auto bulletRenderer = engine->componentManager()->addComponent<MeshRenderer>(name+"-renderer", bullet, engine->assetManager()->getModel("Sphere"));
    bullet->transform()->translate(-self->transform()->right(), ESpace::Local);
    bullet->transform()->setLocalScale({5, 5, 5});
    bullet->setIsSimulated(true);
    auto* bulletCollider  = engine->componentManager()->addComponent<SphereCollider>(name+"-collider", bullet, 2.5);
    auto* bulletRigidBody = engine->componentManager()->addComponent<RigidBody>(name+"rigid-body", bullet, 0.1);
    bullet->transform()->setPosition(self->transform()->worldPosition() + Vec3{1, 1, 1}, ESpace::World);

    bullet->transform()->setRotation(self->transform()->localRotation(), ESpace::Local);
    bulletRigidBody->applyForce(800 * self->transform()->forward());
    bullet->setTag("Bullet");

    auto bulletBehavior = engine->componentManager()->addComponent<Behavior>("behaviro", bullet);
    bulletBehavior->addUpdateFunction([](GameObject* self, Engine* engine, Real dt){
      auto bodies = engine->physicsSystem()->physicsWorld()->collidingRigidBodies(self->getComponent<RigidBody>());
      if (!bodies.isEmpty()) {
        self->setVisible(false);
        self->setIsSimulated(false);
      }
    });
  }
}

void HomelandBehaviors::crateMgrBehavior(GameObject* self, Engine* engine, Real dt)
{
  std::srand(time(nullptr));

  int n = rand() % 1000;

  static int i = 0;

  float proba = (float)n / 1000.f;

  //if ((float)n / 1000.f > 0.85) {
  if (engine->isKeyTriggered(Qt::Key_B)) {
    qDebug() << "DingDong";
    Vec3 pos = {300 * proba - 300, 500, 100 * proba - 50};
    self->transform()->setPosition(pos, ESpace::World);

    String name = QStringLiteral("crate%1").arg(i++);
    auto* crate = engine->sceneManager()->sceneAt(engine->sceneManager()->activatedScene())->createGameObject(name,
                                                                                                              "default");
    auto* crateRenderer = engine->componentManager()->addComponent<MeshRenderer>("mesh-crate", crate,
                                                                                 engine->assetManager()->getModel(
                                                                                     "Crate"));
    crate->transform()->setLocalScale({20, 20, 20});
    crate->transform()->setPosition(self->transform()->worldPosition(), ESpace::World);
    auto* bulletCollider = engine->componentManager()->addComponent<BoxCollider>(name + "-collider", crate,
                                                                                 Vec3{20, 20, 20});
    auto* bulletRigidBody = engine->componentManager()->addComponent<RigidBody>(name + "rigid-body", crate, 100);
    bulletRigidBody->setRollingFriction(4000);
    crate->setIsSimulated(true);
    auto crateBehavior = engine->componentManager()->addComponent<Behavior>("behaviro", crate);
    crateBehavior->addUpdateFunction([](GameObject* self, Engine* engine, Real dt) {
      auto bodies = engine->physicsSystem()->physicsWorld()->collidingRigidBodies(self->getComponent<RigidBody>());
      if (!bodies.isEmpty()) {
        for (auto* body : bodies) {
          if (body->gameObject()->tag() == "Bullet") {
            qDebug() << "Destroy Crate";
            self->setVisible(false);
            self->setIsSimulated(false);
          }
          if (body->gameObject()->tag() == "Ground") {
            qDebug() << "Ground";
            self->getComponent<RigidBody>()->body()->setActivationState(DISABLE_DEACTIVATION);
          }
        }
      }
    });
  }
}
