#include <Engine/GameFramework/Engine.hpp>
#include <Engine/GameFramework/Scene.hpp>
#include <Engine/GameFramework/Components.hpp>
#include <Homeland/HomelandApp.hpp>

HomelandApp::HomelandApp(int argc, char** argv)
  : GameApp("家·Homeland", "Strategy island defense game.", argc, argv),
    m_main_scene{nullptr}
{ }

void HomelandApp::run()
{
  m_engine->renderSystem()->renderScene(nullptr);
}

void HomelandApp::init()
{
  auto sphereMeshHandle = m_engine->assetManager()->loadMesh(":/Models/Sphere");

  //m_main_scene = new Scene("HomelandMainScene", m_engine.get());
  //auto sphere = m_main_scene->createGameObject("Sphere", "default");
  //auto meshRenderer = new MeshRenderer("SphereMeshRenderer", sphere, sphereMeshHandle);
}

HomelandApp::~HomelandApp()
{
  delete m_main_scene;
}
