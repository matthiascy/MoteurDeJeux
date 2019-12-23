#include <Engine/GameFramework/Engine.hpp>
#include <Engine/GameFramework/Components.hpp>
#include <Homeland/HomelandApp.hpp>
#include <Homeland/HomelandBehaviors.hpp>
#include <Engine/Physics/Public/Colliders/SphereCollider.hpp>
#include <Engine/GameFramework/Managers.hpp>
#include <Engine/Physics/Public/Colliders/BoxCollider.hpp>
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
  auto sphereHandle = m_engine->assetManager()->loadMesh("./assets/models/sphere.obj");
  m_assets.insert("Sphere", sphereHandle);
  auto* sphere = m_engine->assetManager()->getMesh(sphereHandle);
  Array<float>& data = sphere->dataArray();
  Real umax = -10.0;
  Real umin = 20.0;
  Real vmax = -10.0;
  Real vmin = 20.0;
  for (UInt32 i = 0; i < sphere->vertexCount(); ++i) {
    Vec3 n = Vec3(data[i*8], data[i*8+1], data[i*8+2]).normalized();
    Real u = qAtan2(n.x(), n.z()) / (2 * M_PI) + 0.5;
    Real v = n.y() * 0.5 + 0.5;
    data[i*8+6] = u;
    data[i*8+7] = v;
    if (umax < u) umax = u;
    if (umin > u) umin = u;
    if (vmax < v) vmax = v;
    if (vmin > v) vmin = v;
  }

  qDebug() << "u max:" << umax << "u min:" << umin;
  qDebug() << "v max:" << vmax << "v min:" << vmin;

  for (UInt32 i = 0; i < sphere->vertexCount(); ++i) {
    data[i*8+6] = (data[i*8+6] - umin) / (umax - umin);
    data[i*8+7] = (data[i*8+7] - vmin) / (vmax - vmin);
  }

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

  _init_camera();

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
  sun->transform()->setPosition({3, 6, 3}, ESpace::World);
  sun->setIsSimulated(true);
  auto* colliderSun = m_engine->addComponent<SphereCollider>("sphere-collider", sun, 1);
  auto* rigidBody = m_engine->addComponent<RigidBody>("rigid-body", sun, m_engine->physicsSystem(), 0.0001);
  m_engine->addComponent<Behavior>("", sun)->setUpdateFn([](GameObject* self, Engine* engine, Real dt){
    if (engine->isKeyPressed(Qt::Key_B)) {
      qDebug() << "Pousser";
      self->getComponent<RigidBody>()->applyForce(self->transform()->forward() * 100);
    }
  });

  auto* cube3 = m_main_scene->createGameObject("Sun", "default");
  auto* meshRenderercube3 = m_engine->addComponent<MeshRenderer>("mesh-renderer00", cube3, m_assets["Cube"]);
  cube3->transform()->setPosition({3, 1, 3}, ESpace::World);
  cube3->setIsSimulated(true);
  auto* colliderCube3 = m_engine->addComponent<BoxCollider>("box-collider", cube3, Vec3{0.5, 0.5, 0.5});
  auto* rigidBodyCube3 = m_engine->addComponent<RigidBody>("rigid-body", cube3, m_engine->physicsSystem(), 0);
  auto* behaviorCube3 = m_engine->addComponent<Behavior>("behavior", cube3);
  behaviorCube3->setUpdateFn([](GameObject* self, Engine* engine, Real dt) {
    if (engine->inputSystem()->isKeyPressed(Qt::Key_Left)) {
      self->transform()->translate(self->transform()->right() * -dt, ESpace::World);
    }
  });

  //auto* collider2Cube3 = m_engine->addComponent<SphereCollider>("sphere", cube3, 10);
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

void HomelandApp::_init_camera()
{
  auto* trackball = m_main_scene->createGameObject("trackball", "default");
  trackball->setStatic(false);
  m_engine->addComponent<Behavior>("trackball", trackball)->setUpdateFn(HomelandBehaviors::trackball);

  auto* camera = m_main_scene->createGameObject("MainCamera", "Camera");
  auto* cameraTransform = camera->transform();
  cameraTransform->setParent(trackball->transform());
  cameraTransform->setPosition({0, 0, 20}, ESpace::World);
  cameraTransform->lookAt(Math::Zero, cameraTransform->up());
  m_engine->addComponent<PerspectiveCamera>("", camera, 45, 1.77, 1, 10000);
  m_engine->addComponent<Behavior>("behavior", camera)->setUpdateFn(HomelandBehaviors::freeView);
}
