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
  m_assets.insert("Cube", m_engine->assetManager()->loadMesh("./Models/cube"));
  m_engine->assetManager()->getMesh(m_assets["Sphere"])->calculateSphericalUV();
}

bool HomelandApp::_init_main_scene()
{
  m_main_scene = m_engine->sceneManager()->sceneAt(m_engine->sceneManager()->createScene("MainScene"));

  auto* camera = m_main_scene->createGameObject("MainCamera", "Camera");
  auto* cameraTransform = camera->transform();
  qDebug() << cameraTransform->worldPosition();
  cameraTransform->setPosition({0, 0, 30}, Transform::ESpace::World);
  cameraTransform->lookAt({0, 0, 0}, cameraTransform->up());
  m_engine->componentManager()->addComponent<PerspectiveCamera>("", camera, 45, 1.77, 1, 10000);
  //auto* behaviorCamera = m_engine->componentManager()->addComponent<Behavior>("behavior", camera);
  //behaviorCamera->setUpdateFn(HomelandBehaviors::cameraBehavior);

  auto* cube = m_main_scene->createGameObject("Cube", "default");
  auto* meshRendererCube = m_engine->componentManager()->addComponent<MeshRenderer>("mesh-renderer00", cube, m_assets["Sphere"]);
  auto* behaviorCube = m_engine->componentManager()->addComponent<Behavior>("behavior", cube);
  behaviorCube->setUpdateFn(HomelandBehaviors::exampleBehavior);
  cube->transform()->setWorldPosition({4, 0, 4});


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
