#include <Engine/GameFramework/Engine.hpp>
#include <Engine/GameFramework/Components.hpp>
#include <Homeland/HomelandApp.hpp>
#include <Homeland/HomelandBehaviors.hpp>
#include <Engine/Physics/Colliders/SphereCollider.hpp>
#include <Engine/GameFramework/Managers.hpp>
#include <Engine/Physics/Colliders/BoxCollider.hpp>
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
  m_engine->physicsSystem()->setGravity({0, -0.98f, 0});
  m_main_scene = m_engine->sceneManager()->sceneAt(m_engine->sceneManager()->createScene("MainScene"));
  m_main_scene->setActive(true);

  auto* cameraOrbit = m_main_scene->createGameObject("CameraOrbit", "default");
  cameraOrbit->setStatic(false);
  auto* behaviroCameraOrbit = m_engine->addComponent<Behavior>("OrbitBehavior", cameraOrbit);
  behaviroCameraOrbit->setUpdateFn([](GameObject* self, Engine* engine, Real dt){
    if (engine->inputSystem()->isMouseButtonPressed(Qt::MouseButton::LeftButton)) {
      self->transform()->rotate(Math::Up, engine->inputSystem()->mouseDelta().x() * 0.1f,ESpace::Local);
    }
  });

  auto* camera = m_main_scene->createGameObject("MainCamera", "Camera");
  auto* cameraTransform = camera->transform();
  cameraTransform->setParent(cameraOrbit->transform());
  cameraTransform->setPosition({0, 0, 10}, ESpace::World);
  cameraTransform->lookAt(Math::Zero, cameraTransform->up());
  m_engine->addComponent<PerspectiveCamera>("", camera, 45, 1.77, 1, 10000);
  auto* behaviorCamera = m_engine->addComponent<Behavior>("behavior", camera);
  behaviorCamera->setUpdateFn(HomelandBehaviors::cameraBehavior);

  auto* cubeOrbit = m_main_scene->createGameObject("CubeOrbit", "default");
  auto* meshRendererCubeOrbit = m_engine->addComponent<MeshRenderer>("mesh-renderer00", cubeOrbit, m_assets["Cube"]);
  auto* behaviorCubeOrbit = m_engine->addComponent<Behavior>("behavior", cubeOrbit);
  behaviorCubeOrbit->setUpdateFn([](GameObject* self, Engine* engine, Real dt){
    self->transform()->rotate({0, 0, 10*dt}, ESpace::Local);
  });

  auto* cube = m_main_scene->createGameObject("Cube", "default");
  auto* meshRendererCube = m_engine->addComponent<MeshRenderer>("mesh-renderer00", cube, m_assets["Cube"]);
  cube->transform()->translate({2, 0, 0}, ESpace::Local);
  cube->transform()->rotate({90, 0, 0}, ESpace::Local);
  //auto* rigidBodyCube = m_engine->addComponent<RigidBody>("rigid-body", cube, m_engine->physicsSystem(), 1.0);
  auto* behaviorCube = m_engine->addComponent<Behavior>("behavior", cube);
  behaviorCube->setUpdateFn([](GameObject* self, Engine* engine, Real dt){
    if (engine->isKeyPressed(Qt::Key_E)) {
      self->transform()->rotate({0, 0, 90 * dt}, ESpace::Local);
    }

    if (engine->isKeyPressed(Qt::Key_Q)) {
      self->transform()->rotate({0, -90 * dt, 0}, ESpace::Local);
    }

    if (engine->isKeyPressed(Qt::Key_O)) {
      self->transform()->setRotation({0, 0, 40}, ESpace::Local);
    }

    if (engine->isKeyPressed(Qt::Key_P)) {
      self->transform()->setRotation({0, 0, 60}, ESpace::World);
    }

    if (engine->isKeyPressed(Qt::Key_K)) {
      self->transform()->translate({0, 0, 0.5f * dt}, ESpace::Local);
    }

    if (engine->isKeyPressed(Qt::Key_L)) {
      self->transform()->translate({0, 0, -0.5f * dt}, ESpace::World);
    }

    if (engine->isKeyTriggered(Qt::Key_U)) {
      self->transform()->setLocalScale({1.2f, 1.2f, 1.2f});
    }

    if (engine->isKeyTriggered(Qt::Key_I)) {
      self->transform()->setLocalScale({1, 1, 1});
    }
  });
  cube->transform()->setParent(cubeOrbit->transform());


  auto* sun = m_main_scene->createGameObject("Sun", "default");
  auto* meshRendererSun = m_engine->addComponent<MeshRenderer>("mesh-renderer00", sun, m_assets["Sphere"]);
  auto* behaviorSun = m_engine->addComponent<Behavior>("behavior", sun);
  behaviorSun->setUpdateFn(HomelandBehaviors::exampleBehavior);
  sun->transform()->setPosition({3, 3, 3}, ESpace::World);
  sun->setIsSimulated(true);
  auto* colliderSun = m_engine->addComponent<SphereCollider>("sphere-collider", sun, 1);
  qDebug() << sun->isSimulated();
  qDebug() << sun->hasCollider();
  auto* rigidBody = m_engine->addComponent<RigidBody>("rigid-body", sun, m_engine->physicsSystem(), 1000000);

  auto* cube3 = m_main_scene->createGameObject("Sun", "default");
  auto* meshRenderercube3 = m_engine->addComponent<MeshRenderer>("mesh-renderer00", cube3, m_assets["Cube"]);
  cube3->transform()->setPosition({3, 1, 3}, ESpace::World);
  cube3->setIsSimulated(true);
  auto* colliderCube3 = m_engine->addComponent<BoxCollider>("box-collider", cube3, Vec3{1, 1, 1});
  //auto* collider2Cube3 = m_engine->addComponent<SphereCollider>("sphere", cube3, 10);
  auto* rigidBodyCube3 = m_engine->addComponent<RigidBody>("rigid-body", cube3, m_engine->physicsSystem(), 0);
  //rigidBodyCube3->setActive(false);
  //rigidBodyCube3->setGravity(Math::Zero);

  /*
  auto* earth = m_main_scene->createGameObject("Earth", "default");
  earth->transform()->setParent(sun->transform());
  earth->transform()->translateWorld({2, 2, 2});
  auto* meshRendererEarth = m_engine->addComponent<MeshRenderer>("mesh-renderer", earth, m_assets["Sphere"]);
  auto* behaviorEarth = m_engine->addComponent<Behavior>("behavior", earth);
  behaviorEarth->setUpdateFn([](GameObject* self, Engine* engine, Real dt){
    self->transform()->rotateLocal(0, 90 * dt, 0);
  });

  auto* moon = m_main_scene->createGameObject("Moon", "default");
  moon->transform()->setParent(earth->transform());
  moon->transform()->translateWorld({5, 5, 5});
  auto* meshRendererMoon = m_engine->addComponent<MeshRenderer>("mesh-renderer01", moon, m_assets["Sphere"]);
   */
}
