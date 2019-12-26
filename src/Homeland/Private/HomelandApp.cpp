#include <Engine/GameFramework/Public/Engine.hpp>
#include <Engine/GameFramework/Public/Components.hpp>
#include <Homeland/HomelandApp.hpp>
#include <Homeland/HomelandBehaviors.hpp>
#include <Physics/Public/Colliders/SphereCollider.hpp>
#include <GameFramework/Public/Managers.hpp>
#include <Physics/Public/Colliders/BoxCollider.hpp>
#include <Physics/Public/PhysicsWorld.hpp>

#include <Graphics/Public/SpotLight.hpp>
#include <Graphics/Public/PointLight.hpp>
#include <Graphics/Public/DirectionalLight.hpp>
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
  m_assets.insert("Klingon", m_engine->assetManager()->loadMesh("./assets/models/klingon.off"));
  //m_assets.insert("Tree000", m_engine->assetManager()->loadMesh("./assets/models/tree.fbx"));
  m_assets.insert("TreeType001", m_engine->assetManager()->loadMesh("./assets/models/TreeType001.dae"));
  m_assets.insert("Cube", m_engine->assetManager()->loadMesh("./assets/models/cube.obj"));
  m_assets.insert("Plane", m_engine->assetManager()->loadMesh("./assets/models/plane.obj"));
  //m_assets.insert("Terrain000", m_engine->assetManager()->loadMesh("./src/Homeland/Assets/models/background.obj"));
  //m_assets.insert("Terrain001", m_engine->assetManager()->loadMesh("./src/Homeland/Assets/models/ground.obj"));
  //m_assets.insert("Terrain002", m_engine->assetManager()->loadMesh("./src/Homeland/Assets/models/ground2.obj"));
  //m_assets.insert("Terrain003", m_engine->assetManager()->loadMesh("./src/Homeland/Assets/models/cube1.fbx"));
  m_engine->assetManager()->getMesh(m_assets["Sphere"])->calculateSphericalUV();
}

bool HomelandApp::_init_main_scene()
{
  m_main_scene = m_engine->sceneManager()->sceneAt(m_engine->sceneManager()->createScene("MainScene"));
  m_main_scene->setActive(true);

  _init_camera();
  _init_terrain();
  _init_lights();

  auto* plane = m_main_scene->createGameObject("Plane", "default");
  auto* planeMeshRenderer = m_engine->componentManager()->addComponent<MeshRenderer>("mesh-plane", plane, m_assets["Cube"]);
  plane->transform()->setLocalScale({20, 20, 1});
  plane->transform()->setPosition({0, 0, -10}, ESpace::World);

  auto* cubeOrbit = m_main_scene->createGameObject("CubeOrbit", "default");
  auto* meshRendererCubeOrbit = m_engine->componentManager()->addComponent<MeshRenderer>("mesh-renderer00", cubeOrbit, m_assets["Cube"]);
  auto* behaviorCubeOrbit = m_engine->componentManager()->addComponent<Behavior>("behavior", cubeOrbit);
  behaviorCubeOrbit->setUpdateFn([](GameObject* self, Engine* engine, Real dt){
    self->transform()->rotate({0, 10*dt, 0*dt}, ESpace::Local);
  });

  auto* cube = m_main_scene->createGameObject("Cube", "default");
  auto* meshRendererCube = m_engine->componentManager()->addComponent<MeshRenderer>("mesh-renderer00", cube, m_assets["Cube"]);
  cube->transform()->translate({2, 0, 0}, ESpace::Local);
  cube->transform()->rotate({90, 0, 0}, ESpace::Local);
  auto* behaviorCube = m_engine->componentManager()->addComponent<Behavior>("behavior", cube);
  behaviorCube->setUpdateFn([](GameObject* self, Engine* engine, Real dt){
    if (engine->isKeyPressed(Qt::Key_E)) {
      self->transform()->rotate({0, 0, 90 * dt}, ESpace::Local);
    }

    if (engine->isKeyPressed(Qt::Key_Q)) {
      self->transform()->rotate({0, -90 * dt, 0}, ESpace::Local);
    }
  });
  cube->transform()->setParent(cubeOrbit->transform());


  auto* sun = m_main_scene->createGameObject("Sun", "default");
  auto* meshRendererSun = m_engine->componentManager()->addComponent<MeshRenderer>("mesh-renderer00", sun, m_assets["Sphere"]);
  //auto* behaviorSun = m_engine->componentManager()->addComponent<Behavior>("behavior", sun);
  //behaviorSun->setUpdateFn(HomelandBehaviors::exampleBehavior);
  sun->transform()->setPosition({3, 6, 3}, ESpace::World);
  sun->setIsSimulated(true);
  auto* colliderSun = m_engine->componentManager()->addComponent<SphereCollider>("sphere-collider", sun, 1);
  auto* rigidBody = m_engine->componentManager()->addComponent<RigidBody>("rigid-body", sun, 0.00000000000000000000001);

  auto* cube3 = m_main_scene->createGameObject("Cube3", "default");
  auto* meshRenderercube3 = m_engine->componentManager()->addComponent<MeshRenderer>("mesh-renderer00", cube3, m_assets["Cube"]);
  cube3->transform()->setPosition({3, -4, 3}, ESpace::World);
  cube3->setIsSimulated(true);
  auto* colliderCube3 = m_engine->componentManager()->addComponent<BoxCollider>("box-collider", cube3, Vec3{1.5, 1.5, 1.5});
  auto* rigidBodyCube3 = m_engine->componentManager()->addComponent<RigidBody>("rigid-body", cube3, 0);
  auto* behaviorCube3 = m_engine->componentManager()->addComponent<Behavior>("behavior", cube3);
  behaviorCube3->setUpdateFn([](GameObject* self, Engine* engine, Real dt) {
    if (engine->inputSystem()->isKeyPressed(Qt::Key_Left)) {
      self->transform()->translate(self->transform()->right() * -dt, ESpace::World);
    }

    auto bodies = engine->physicsSystem()->physicsWorld()->collidingRigidBodies(self->getComponent<RigidBody>());
    if (!bodies.isEmpty()) {
      for (auto body : bodies) {
        if (body->gameObject()->name() == "Sun") {
          //qDebug() << "BLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLl";
        }
      }
    }

  });
}

void HomelandApp::_init_camera()
{
  auto* trackball = m_main_scene->createGameObject("trackball", "default");
  trackball->setStatic(false);
  m_engine->componentManager()->addComponent<Behavior>("trackball", trackball)->setUpdateFn(HomelandBehaviors::trackball);

  auto* camera = m_main_scene->createGameObject("MainCamera", "Camera");
  auto* cameraTransform = camera->transform();
  cameraTransform->setParent(trackball->transform());
  cameraTransform->setPosition({0, 0, 20}, ESpace::World);
  cameraTransform->lookAt(Math::Zero, cameraTransform->up());
  m_engine->componentManager()->addComponent<PerspectiveCamera>("", camera, 45, 1.77, 1, 10000);
  m_engine->componentManager()->addComponent<Behavior>("behavior", camera)->setUpdateFn(HomelandBehaviors::freeView);
}

void HomelandApp::_init_terrain()
{
  /*
auto* terrain000 = m_main_scene->createGameObject("terrain0", "default");
m_engine->componentManager()->addComponent<MeshRenderer>("terrain001-mesh-renderer", terrain000, m_assets["Terrain001"]);
terrain000->transform()->setLocalScale({10, 10 ,10});
terrain000->transform()->setPosition({0, 0, 0}, ESpace::World);

auto* terrain001 = m_main_scene->createGameObject("terrain0", "default");
m_engine->componentManager()->addComponent<MeshRenderer>("terrain002-mesh-renderer", terrain0, m_assets["Terrain002"]);
terrain0->transform()->setLocalScale({10, 10 ,10});
terrain0->transform()->setPosition({0, 0, 0}, ESpace::World);
 */

  /*
  auto* terrain003 = m_main_scene->createGameObject("terrain3", "default");
  m_engine->componentManager()->addComponent<MeshRenderer>("terrain001-mesh-renderer", terrain003, m_assets["Terrain003"]);
  terrain003->transform()->setLocalScale({1, 1 ,1});
  terrain003->transform()->setPosition({0, 0, 0}, ESpace::World);
   */
}

void HomelandApp::_init_lights()
{
  auto* orbit = m_main_scene->createGameObject("Light000Orbit", "default");
  orbit->transform()->setPosition({0, 0, 0}, ESpace::World);
  //m_engine->componentManager()->addComponent<Behavior>("behavior", orbit)->setUpdateFn([](GameObject* self, Engine* engine, Real dt){
    //self->transform()->rotate(Math::AxisY, 40*dt, ESpace::Local);
  //});

  // Directional
  auto* light000 = m_main_scene->createGameObject("light000", "default");
  light000->transform()->setPosition({0, 0, 10}, ESpace::World);
  m_engine->componentManager()->addComponent<MeshRenderer>("m", light000, m_assets["Cube"]);
  light000->transform()->setParent(orbit->transform());
  auto* lightComp000 = m_engine->componentManager()->addComponent<DirectionalLight>("DirectionalLight", light000, Vec3{1.0f, 0.01f, 0.01f}, 1.0f);
  qDebug() << light000->transform()->forward();

  // Spot
  auto* light001 = m_main_scene->createGameObject("light001", "default");
  light001->transform()->setPosition({5, 5, 10}, ESpace::World);
  //light000->transform()->setParent(orbit->transform());
  m_engine->componentManager()->addComponent<MeshRenderer>("mm", light001, m_assets["Cube"]);
  auto* lightComp001 = m_engine->componentManager()->addComponent<SpotLight>("SpotLight", light001, Vec3{0.11f, 1.0f, 0.01f},
      1.0f, 10, 45, Attenuation{0.1, 0.1, 0.1});
  m_engine->componentManager()->addComponent<Behavior>("behavior", light001)->setUpdateFn(HomelandBehaviors::exampleBehavior000);

  // Point
  auto* light002 = m_main_scene->createGameObject("light002", "default");
  light002->transform()->setPosition({-5, 5, 0}, ESpace::World);
  //light000->transform()->setParent(orbit->transform());
  m_engine->componentManager()->addComponent<MeshRenderer>("m", light002, m_assets["Cube"]);
  auto* lightComp002 = m_engine->componentManager()->addComponent<PointLight>("PointLight", light002, Vec3{0.11f, 0.12f, 1.0f},
      1.0f, 10, Attenuation{0.1, 0.1, 0.1});
  m_engine->componentManager()->addComponent<Behavior>("behavior", light002)->setUpdateFn(HomelandBehaviors::exampleBehavior001);

  // Point
  auto* light003 = m_main_scene->createGameObject("light003", "default");
  light003->transform()->setPosition({-5, 5, -15}, ESpace::World);
  //light000->transform()->setParent(orbit->transform());
  m_engine->componentManager()->addComponent<MeshRenderer>("m", light003, m_assets["Cube"]);
  auto* lightComp003 = m_engine->componentManager()->addComponent<PointLight>("PointLight", light003, Vec3{0.11f, 0.12f, 1.0f},
                                                                              1.0f, 10, Attenuation{0.1, 0.1, 0.1});
  //m_engine->componentManager()->addComponent<Behavior>("behavior", light002)->setUpdateFn(HomelandBehaviors::exampleBehavior001);
}

