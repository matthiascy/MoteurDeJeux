#include <Engine/GameFramework/Engine.hpp>
#include <Engine/GameFramework/Scene.hpp>
#include <Engine/GameFramework/Components.hpp>
#include <Homeland/HomelandApp.hpp>
#include <Homeland/HomelandBehaviors.hpp>
#include <QApplication>

HomelandApp::HomelandApp(int argc, char** argv)
  : GameApp("家·Homeland", "Strategy island defense game.", {1024, 768}, argc, argv),
    m_main_scene{nullptr}
{
  QCursor cursor(QPixmap(":/Assets/cursor"), 0, 0);
  QApplication::setOverrideCursor(cursor);
}

void HomelandApp::init()
{
  _load_game_asset();
  _init_main_scene();
}

void HomelandApp::_load_game_asset()
{
  m_assets.insert("Sphere", m_engine->assetManager()->loadMesh("./assets/models/sphere.obj"));
  m_assets.insert("Klingon", m_engine->assetManager()->loadMesh("./assets/models/klingon.off"));
  m_assets.insert("TreeType001", m_engine->assetManager()->loadMesh("./assets/models/TreeType001.dae"));
  //m_assets.insert("TreeType001", m_engine->assetManager()->loadMesh("./assets/models/TreeType001.dae"));
  m_assets.insert("Cube", m_engine->assetManager()->loadMesh("./assets/models/cube.obj"));
  m_engine->assetManager()->getMesh(m_assets["Sphere"])->calculateSphericalUV();
}

bool HomelandApp::_init_main_scene()
{
  m_main_scene = m_engine->sceneManager()->sceneAt(m_engine->sceneManager()->createScene("MainScene"));

  auto* cameraOrbit = m_main_scene->createGameObject("CameraOrbit", "default");
  cameraOrbit->setStatic(false);
  auto* behaviroCameraOrbit = m_engine->componentManager()->addComponent<Behavior>("OrbitBehavior", cameraOrbit);
  behaviroCameraOrbit->setUpdateFn([](GameObject* self, Engine* engine, Real dt){
    if (engine->inputSystem()->isMouseButtonPressed(Qt::MouseButton::LeftButton)) {
      self->transform()->rotate(Math::Up, engine->inputSystem()->mouseDelta().x() * 0.1f,ESpace::Local);
    }
  });

  auto* camera = m_main_scene->createGameObject("MainCamera", "Camera");
  auto* cameraTransform = camera->transform();
  cameraTransform->setParent(cameraOrbit->transform());
  qDebug() << cameraTransform->worldPosition();
  cameraTransform->setPosition({0, 10, 10}, ESpace::World);
  cameraTransform->lookAt({-2, -2, -2}, cameraTransform->up());
  m_engine->componentManager()->addComponent<PerspectiveCamera>("", camera, 45, 1.77, 1, 10000);
  auto* behaviorCamera = m_engine->componentManager()->addComponent<Behavior>("behavior", camera);
  behaviorCamera->setUpdateFn(HomelandBehaviors::cameraBehavior);

  auto* cubeOrbit = m_main_scene->createGameObject("CubeOrbit", "default");
  auto* meshRendererCubeOrbit = m_engine->componentManager()->addComponent<MeshRenderer>("mesh-renderer00", cubeOrbit, m_assets["Cube"]);
  auto* behaviorCubeOrbit = m_engine->componentManager()->addComponent<Behavior>("behavior", cubeOrbit);
  //cubeOrbit->transform()->setLocalScale({0.5, 1, 1});
  behaviorCubeOrbit->setUpdateFn([](GameObject* self, Engine* engine, Real dt){
    self->transform()->rotate({0, 0, 10*dt}, ESpace::Local);
    //self->transform()->translate({0, 0, -dt * 0.1f}, ESpace::Local);
  });

  auto* cube = m_main_scene->createGameObject("Cube", "default");
  auto* meshRendererCube = m_engine->componentManager()->addComponent<MeshRenderer>("mesh-renderer00", cube, m_assets["Cube"]);
  cube->transform()->translate({2, 0, 0}, ESpace::Local);
  cube->transform()->rotate({90, 0, 0}, ESpace::Local);
  auto* behaviorCube = m_engine->componentManager()->addComponent<Behavior>("behavior", cube);
  //behaviorCube->setUpdateFn(HomelandBehaviors::exampleBehavior);
  qDebug() << "Local scale" << cube->transform()->localScale();
  qDebug() << "World scale" << cube->transform()->worldScale();
  qDebug() << "Local pos" << cube->transform()->localPosition();
  qDebug() << "World pos" << cube->transform()->worldPosition();
  behaviorCube->setUpdateFn([](GameObject* self, Engine* engine, Real dt){
    if (engine->inputSystem()->isKeyPressed(Qt::Key_E)) {
      self->transform()->rotate({0, 0, 90 * dt}, ESpace::Local);
      //self->transform()->rotateWorld(0, 0, 90 * dt);
      //self->transform()->setLocalRotation(self->transform()->localRotation() * Quat::fromAxisAndAngle(Math::Up, 60 * dt));
      //self->transform()->rotateWorld(0, 90 * dt, 0);
    }

    if (engine->inputSystem()->isKeyPressed(Qt::Key_Q)) {
      self->transform()->rotate({0, -90 * dt, 0}, ESpace::Local);
    }

    if (engine->inputSystem()->isKeyPressed(Qt::Key_O)) {
      qDebug() << "Set Rotate local";
      self->transform()->setRotation({0, 0, 40}, ESpace::Local);
      qDebug() << self->transform()->localRotation();
      qDebug() << self->transform()->worldRotation();
      //self->transform()->rotate({0, 0, 60 * dt}, ESpace::Local);
    }

    if (engine->inputSystem()->isKeyPressed(Qt::Key_P)) {
      qDebug() << "Set Rotate world";
      //self->transform()->rotate({0, 0, 60 * dt}, ESpace::World);
      self->transform()->setRotation({0, 0, 60}, ESpace::World);
      qDebug() << self->transform()->localRotation();
      qDebug() << self->transform()->worldRotation();
    }

    if (engine->inputSystem()->isKeyPressed(Qt::Key_K)) {
      qDebug() << "Translate local";
      self->transform()->translate({0, 0, 0.5f * dt}, ESpace::Local);
    }

    if (engine->inputSystem()->isKeyPressed(Qt::Key_L)) {
      qDebug() << "Translate world";
      self->transform()->translate({0, 0, -0.5f * dt}, ESpace::World);
    }

    if (engine->inputSystem()->isKeyTriggered(Qt::Key_U)) {
      qDebug() << "Scale";
      self->transform()->setLocalScale({1.2f, 1.2f, 1.2f});
    }

    if (engine->inputSystem()->isKeyTriggered(Qt::Key_I)) {
      qDebug() << "Reset scale";
      self->transform()->setLocalScale({1, 1, 1});
    }
  });
  cube->transform()->setParent(cubeOrbit->transform());


  /*
  auto* sun = m_main_scene->createGameObject("Sun", "default");
  auto* meshRendererSun = m_engine->componentManager()->addComponent<MeshRenderer>("mesh-renderer00", sun, m_assets["Sphere"]);
  auto* behaviorSun = m_engine->componentManager()->addComponent<Behavior>("behavior", sun);
  behaviorSun->setUpdateFn(HomelandBehaviors::exampleBehavior);
  sun->transform()->setWorldPosition({0, 0, 0});

  auto* earth = m_main_scene->createGameObject("Earth", "default");
  earth->transform()->setParent(sun->transform());
  earth->transform()->translateWorld({2, 2, 2});
  auto* meshRendererEarth = m_engine->componentManager()->addComponent<MeshRenderer>("mesh-renderer", earth, m_assets["Sphere"]);
  auto* behaviorEarth = m_engine->componentManager()->addComponent<Behavior>("behavior", earth);
  behaviorEarth->setUpdateFn([](GameObject* self, Engine* engine, Real dt){
    self->transform()->rotateLocal(0, 90 * dt, 0);
  });

  auto* moon = m_main_scene->createGameObject("Moon", "default");
  moon->transform()->setParent(earth->transform());
  moon->transform()->translateWorld({5, 5, 5});
  auto* meshRendererMoon = m_engine->componentManager()->addComponent<MeshRenderer>("mesh-renderer01", moon, m_assets["Sphere"]);
   */
}
