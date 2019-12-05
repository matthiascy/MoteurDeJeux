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
  //camera->transform()->lookAt(Vec3{0, 0, 0}, Math::Up);

  auto sphereMeshHandle = m_engine->assetManager()->loadMesh("./assets/models/sphere.obj");
  auto* sphere = m_main_scene->createGameObject("Sphere", "default");
  auto* meshRender = m_engine->componentManager()->addComponent<MeshRenderer>(sphere, sphereMeshHandle);
  auto* transform = m_engine->componentManager()->addComponent<Transform>(sphere, nullptr);
}
