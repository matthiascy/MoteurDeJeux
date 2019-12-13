#include <Engine/GameFramework/Engine.hpp>
#include <Engine/GameFramework/Scene.hpp>
#include <Engine/GameFramework/Components.hpp>
#include <Homeland/HomelandApp.hpp>

HomelandApp::HomelandApp(int argc, char** argv)
  : GameApp("家·Homeland", "Strategy island defense game.", {1024, 768}, argc, argv),
    m_main_scene{nullptr}
{ }

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
  m_engine->assetManager()->getMesh(m_assets["Sphere"])->calculateSphericalUV();
}

bool HomelandApp::_init_main_scene()
{
  m_main_scene = m_engine->sceneManager()->sceneAt(m_engine->sceneManager()->createScene("MainScene"));

  auto* camera = m_main_scene->createGameObject("MainCamera", "Camera");
  auto* cameraTransform = camera->transform();
  cameraTransform->setPosition(0, 0, 50, Transform::ESpace::World);
  cameraTransform->lookAt({0, 0, 0}, cameraTransform->up());
  m_engine->componentManager()->addComponent<PerspectiveCamera>("", camera, 45, 1.77, 1, 1000);

  auto* sphere = m_main_scene->createGameObject("Sphere", "default");
  sphere->transform()->translateWorld(-4, 0, 0);
  auto* meshRenderer00 = m_engine->componentManager()->addComponent<MeshRenderer>("mesh-renderer00", sphere, m_assets["Sphere"]);
  qDebug() << sphere->transform()->worldPosition();

  auto* sphere2 = m_main_scene->createGameObject("Sphere2", "default");
  sphere2->transform()->setParent(sphere->transform());
  sphere2->transform()->translateWorld(4, 0, 0);
  auto* meshRenderer01 = m_engine->componentManager()->addComponent<MeshRenderer>("mesh-renderer01", sphere2, m_assets["Sphere"]);
}

void HomelandApp::_camera_control()
{
  auto camera = m_main_scene->mainCamera();

  if (engine()->inputSystem()->isKeyPressed(Qt::Key_Up)) {
    qDebug() << "Up";
    camera->transform()->translateWorld(0, 0, -m_dt);
  }

  if (engine()->inputSystem()->isKeyPressed(Qt::Key_Down)) {
    camera->transform()->translateWorld(0, 0, m_dt);
    qDebug() << "Down";
  }

  if (engine()->inputSystem()->isKeyPressed(Qt::Key_Left)) {
    camera->transform()->translateWorld(-m_dt, 0, 0);
  }

  if (engine()->inputSystem()->isKeyPressed(Qt::Key_Right)) {
    camera->transform()->translateWorld(m_dt, 0, 0);
  }

  camera->transform()->lookAt({0, 0, 0}, Math::Up);

  qDebug() << "Camera pos : " << camera->transform()->worldPosition();
}

void HomelandApp::onUpdate()
{
  _camera_control();

  auto sphere = m_main_scene->find("Sphere");
  if (engine()->inputSystem()->isKeyPressed(Qt::Key_W)) {
    if (sphere) {
      sphere->transform()->translateWorld(0, 0, -m_dt);
      qDebug() << sphere->transform()->worldPosition();
    }
  }

  if (engine()->inputSystem()->isKeyPressed(Qt::Key_S)) {
    if (sphere) {
      sphere->transform()->translateWorld(0, 0, m_dt);
      qDebug() << sphere->transform()->worldPosition();
    }
  }

  if (engine()->inputSystem()->isKeyPressed(Qt::Key_A)) {
    if (sphere) {
      sphere->transform()->translateWorld(-m_dt, 0, 0);
      qDebug() << sphere->transform()->worldPosition();
    }
  }

  if (engine()->inputSystem()->isKeyPressed(Qt::Key_D)) {
    if (sphere) {
      sphere->transform()->translateWorld(m_dt, 0, 0);
      qDebug() << sphere->transform()->worldPosition();
    }
  }

  if (engine()->inputSystem()->isKeyPressed(Qt::Key_R)) {
    if (sphere) {
      sphere->transform()->rotateWorld(0, 0, 30*m_dt);
    }
  }
}
