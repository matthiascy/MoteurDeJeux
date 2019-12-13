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
  cameraTransform->setPosition(0, 0, 10, Transform::ESpace::World);
  cameraTransform->lookAt({0, 0, 0}, cameraTransform->up());
  m_engine->componentManager()->addComponent<PerspectiveCamera>("", camera, 45, 1.77, 0.01, 1000);

  auto* sphere = m_main_scene->createGameObject("Sphere", "default");
  sphere->transform()->translateWorld(-4, 0, 0);
  auto* meshRenderer00 = m_engine->componentManager()->addComponent<MeshRenderer>("mesh-renderer00", sphere, m_assets["Sphere"]);
  //sphere->setVisible(false);
  qDebug() << sphere->transform()->worldPosition();

  auto* sphere2 = m_main_scene->createGameObject("Sphere2", "default");
  sphere2->transform()->translateWorld(4, 0, 0);
  auto* meshRenderer01 = m_engine->componentManager()->addComponent<MeshRenderer>("mesh-renderer01", sphere2, m_assets["Sphere"]);
  //sphere2->setVisible(false);
}

void HomelandApp::onUpdate()
{
  if (engine()->inputSystem()->isKeyPressed(Qt::Key_I)) {
    qDebug() << "I pressed.";
    m_engine->renderSystem()->grabFramebuffer().save("grabbed_image.png");
    auto sphere = m_main_scene->find("Sphere");
    if (sphere != nullptr) {
      sphere->transform()->translateWorld(-1, -1, -1);
      qDebug() << sphere->transform()->worldPosition();
    }
  }

  if (engine()->inputSystem()->isKeyPressed(Qt::Key_P)) {
    m_engine->renderSystem()->offscreenSurface()->framebufferObject()->toImage().save("P.png");
  }

  if (engine()->inputSystem()->isKeyPressed(Qt::Key_Left)) {
    m_main_scene->mainCamera()->transform()->translateWorld(-1, 0, 0);
  }

  if (engine()->inputSystem()->isKeyPressed(Qt::Key_Right)) {
    m_main_scene->mainCamera()->transform()->translateWorld(1, 0, 0);
  }
}
