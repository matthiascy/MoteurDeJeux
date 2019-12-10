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
  init_main_scene_();
}

bool HomelandApp::init_main_scene_()
{
  m_main_scene = m_engine->sceneManager()->sceneAt(m_engine->sceneManager()->createScene("MainScene"));

  auto* camera = m_main_scene->createGameObject("MainCamera", "Camera");
  auto* cameraTransform = camera->transform();
  cameraTransform->setPosition(0, 0, 10, Transform::ESpace::World);
  cameraTransform->lookAt({0, 0, 0}, cameraTransform->up());
  m_engine->componentManager()->addComponent<PerspectiveCamera>("", camera, 45, 1.77, 0.01, 1000);

  auto sphereMeshHandle = m_engine->assetManager()->loadMesh("./assets/models/sphere.obj");
  auto* sphere = m_main_scene->createGameObject("Sphere", "default");
  sphere->transform()->translateWorld(-4, 0, 0);
  auto* meshRender = m_engine->componentManager()->addComponent<MeshRenderer>("mesh-renderer", sphere, sphereMeshHandle);

  auto* sphere2 = m_main_scene->createGameObject("Sphere2", "default");
  sphere2->transform()->translateWorld(4, 0, 0);
  auto* meshRender2 = m_engine->componentManager()->addComponent<MeshRenderer>("mesh-renderer2", sphere2, sphereMeshHandle);
}
