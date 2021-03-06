#include <Engine/GameFramework/Public/Engine.hpp>
#include <Engine/GameFramework/Public/Components.hpp>
#include <Homeland/HomelandApp.hpp>
#include <Homeland/HomelandBehaviors.hpp>
#include <Physics/Public/Colliders/SphereCollider.hpp>
#include <GameFramework/Public/Managers.hpp>
#include <Physics/Public/Colliders/BoxCollider.hpp>
#include <Physics/Public/PhysicsWorld.hpp>
#include <Graphics/Public/Model.hpp>
#include <Graphics/Public/AnimatedMeshRenderer.hpp>
#include <Graphics/Public/Animator.hpp>
#include <Graphics/Public/SpotLight.hpp>
#include <Graphics/Public/PointLight.hpp>
#include <Graphics/Public/DirectionalLight.hpp>
#include <QApplication>

// TODO: multiple update functions

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
  m_engine->assetManager()->loadModel("./assets/models/sphere.obj", "Sphere");
  m_engine->assetManager()->loadModel("./src/Homeland/Assets/models/cube.obj", "Cube");
  m_engine->assetManager()->loadModel("./assets/models/sponza/sponza-m.obj", "Sponza");
  m_engine->assetManager()->loadModel("./assets/models/Handgun/HandGun.obj", "HandGun");
  m_engine->assetManager()->loadModel("./assets/models/Crate/Crate.obj", "Crate");
  m_engine->assetManager()->loadModel("./assets/models/Handgun/HandGun.obj", "Revolver");
  //m_assets.insert("MerdiqueFBXAModel", {m_engine->assetManager()->loadAnimatedModel("./assets/models/merdique.fbx").idx, EAssetType::AnimatedModel});
  //m_assets.insert("MerdiqueFBXModel", {m_engine->assetManager()->loadModel("./assets/models/merdique.fbx").idx, EAssetType::Model});
  //m_assets.insert("MerdiqueDAE", {m_engine->assetManager()->loadModel("./assets/models/merdique.dae").idx, EAssetType::AnimatedModel});
  //m_assets.insert("Terrain001", m_engine->assetManager()->loadModel("./src/Homeland/Assets/models/ground.obj"));
  //m_assets.insert("Terrain002", m_engine->assetManager()->loadModel("./src/Homeland/Assets/models/ground2.obj"));
  //m_assets.insert("Terrain003", m_engine->assetManager()->loadModel("./src/Homeland/Assets/models/cube1.fbx"));
  //m_engine->assetManager()->getMesh(m_engine->assetManager()->getModel(m_assets["Sphere"])->meshes()[0])->calculateSphericalUV();
  //m_assets.insert("M1", {m_engine->assetManager()->loadAnimatedModel("./assets/models/1.fbx").idx, EAssetType::AnimatedModel});
  //m_assets.insert("Sprite", {m_engine->assetManager()->loadAnimatedModel("./assets/models/sprite/cleared.fbx").idx, EAssetType::AnimatedModel});
  //m_assets.insert("FistFight", {m_engine->assetManager()->loadAnimatedModel("./assets/models/fist-fight-a/FistFightA.fbx").idx, EAssetType::AnimatedModel});
  //m_assets.insert("Sprite", {m_engine->assetManager()->loadModel("./assets/models/sprite/cleared.fbx").idx, EAssetType::Model});
}

bool HomelandApp::_init_main_scene()
{
  m_main_scene = m_engine->sceneManager()->sceneAt(m_engine->sceneManager()->createScene("MainScene"));
  m_main_scene->setActive(true);

  _init_camera();
  _init_terrain();
  _init_lights();

  /*
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

  auto* cube3 = m_main_scene->createGameObject("Cube3", "default");
  auto* meshRenderercube3 = m_engine->componentManager()->addComponent<MeshRenderer>("mesh-renderer00", cube3, m_assets["Cube"]);
  cube3->transform()->setPosition({3, -4, 3}, ESpace::World);
  cube3->setIsSimulated(true);
  auto* colliderCube3 = m_engine->componentManager()->addComponent<BoxCollider>("box-collider", cube3, Vec3{1.5, 1.5, 1.5});
  auto* rigidBodyCube3 = m_engine->componentManager()->addComponent<RigidBody>("rigid-body", cube3, 0);
  auto* behaviorCube3 = m_engine->componentManager()->addComponent<Behavior>("behavior", cube3);
  behaviorCube3->addUpdateFunction([](GameObject* self, Engine* engine, Real dt) {
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
  */

  /*
  auto* ball = m_main_scene->createGameObject("ball", "default");
  auto* meshRendererBall = m_engine->componentManager()->addComponent<MeshRenderer>("mesh-renderer00", ball, ModelHandle{m_assets["Cube"].idx});
  //auto* behaviorSun = m_engine->componentManager()->addComponent<Behavior>("behavior", sun);
  //behaviorSun->addUpdateFunction(HomelandBehaviors::exampleBehavior);
  ball->transform()->setPosition({0, 200, 0}, ESpace::World);
  ball->setIsSimulated(true);
  ball->transform()->setLocalScale({10, 10, 10});
  auto* colliderball = m_engine->componentManager()->addComponent<SphereCollider>("sphere-collider", ball, 10);
  auto* rigidBody = m_engine->componentManager()->addComponent<RigidBody>("rigid-body", ball, 0.00000000000000000000001);
   */

  /*
  auto* cube3 = m_main_scene->createGameObject("Cube3", "default");
  auto* meshRenderercube3 = m_engine->componentManager()->addComponent<MeshRenderer>("mesh-renderer00", cube3, m_assets["Cube"]);
  cube3->transform()->setPosition({0, 0, 0}, ESpace::World);
  cube3->setIsSimulated(true);
  auto* colliderCube3 = m_engine->componentManager()->addComponent<BoxCollider>("box-collider", cube3, Vec3{1.5, 1.5, 1.5});
  auto* rigidBodyCube3 = m_engine->componentManager()->addComponent<RigidBody>("rigid-body", cube3, 0);
  auto* behaviorCube3 = m_engine->componentManager()->addComponent<Behavior>("behavior", cube3);
  behaviorCube3->addUpdateFunction([](GameObject* self, Engine* engine, Real dt) {
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
   */

  /*
  auto* sibenik = m_main_scene->createGameObject("sibenik", "default");
  auto* sibenikMeshRenderer = m_engine->componentManager()->addComponent<MeshRenderer>("mesh-sponza", sibenik, m_assets["Sibenik"]);
  sibenik->transform()->setPosition({0, 0, -10}, ESpace::World);
   */

  //auto* knight = m_main_scene->createGameObject("Knight", "default");
  //auto* knightMeshRenderer = m_engine->componentManager()->addComponent<MeshRenderer>("mesh-knight", knight, m_assets["Knight"]);
  //knight->transform()->setPosition({20, 0, -20}, ESpace::World);
  //knight->transform()->setLocalScale({10, 10, 10});

  //auto* helico = m_main_scene->createGameObject("Helico", "default");
  //auto* helicoRenderer = m_engine->componentManager()->addComponent<MeshRenderer>("mesh-helico", helico, ModelHandle{m_assets["Helico"].idx});
  //helico->transform()->setPosition({0, 0, 0}, ESpace::World);

  auto* crateMgr = m_main_scene->createGameObject("CrateMgr", "default");
  auto* crateBehavior = m_engine->componentManager()->addComponent<Behavior>("behavior", crateMgr);
  m_engine->componentManager()->addComponent<MeshRenderer>("mesh-gun", crateMgr, m_engine->assetManager()->getModel("Cube"));
  crateMgr->transform()->setLocalScale({10, 10, 10});
  crateBehavior->addUpdateFunction(HomelandBehaviors::crateMgrBehavior);


  return true;
}

void HomelandApp::_init_camera()
{
  auto* camera = m_main_scene->createGameObject("MainCamera", "Camera");
  auto* cameraTransform = camera->transform();
  cameraTransform->setPosition({-400, 48, 0}, ESpace::World);
  m_engine->componentManager()->addComponent<PerspectiveCamera>("", camera, 45, 1.77, 1, 10000);
  auto* cameraBehavior = m_engine->componentManager()->addComponent<Behavior>("behavior", camera);
  cameraBehavior->addUpdateFunction(HomelandBehaviors::fpsView);
  cameraBehavior->addUpdateFunction(HomelandBehaviors::bodyBehavior);

  auto* handgun = m_main_scene->createGameObject("HandGun", "default");
  auto* handGunRenderer = m_engine->componentManager()->addComponent<MeshRenderer>("mesh-gun", handgun, m_engine->assetManager()->getModel("Revolver"));
  handgun->transform()->setParent(camera->transform());
  handgun->transform()->translate(cameraTransform->forward() * -1.2f, ESpace::Local);
  handgun->transform()->setLocalScale({1, 1, 1});
  //handgun->transform()->translate(cameraTransform->forward() * -2.2f, ESpace::Local);
  //handgun->transform()->setLocalScale({5, 5, 5});
  handgun->transform()->rotate(Math::AxisY, 180, ESpace::Local);
  handgun->transform()->rotate(Math::AxisX, -15, ESpace::Local);
  auto handGunBehavior = m_engine->componentManager()->addComponent<Behavior>("behavior", handgun);
  handGunBehavior->addUpdateFunction(HomelandBehaviors::handGun);

  cameraTransform->rotate(Math::AxisY, -90, ESpace::World);
}

void HomelandApp::_init_terrain()
{
  auto* sponza = m_main_scene->createGameObject("Sponza", "default");
  auto* sponzaMeshRenderer = m_engine->componentManager()->addComponent<MeshRenderer>("Sponza", sponza, m_engine->assetManager()->getModel("Sponza"));
  sponza->transform()->setPosition({0, 0, 0}, ESpace::World);

  auto* terrain = m_main_scene->createGameObject("Terrain", "Ground");
  terrain->transform()->setPosition({0, -5, 0}, ESpace::World);
  m_engine->componentManager()->addComponent<BoxCollider>("boxCollider", terrain, Vec3{1000, 5, 250});
  m_engine->componentManager()->addComponent<RigidBody>("rigidBody", terrain, 0);
  terrain->setIsSimulated(true);
}

void HomelandApp::_init_lights()
{
  auto* orbit = m_main_scene->createGameObject("Light000Orbit", "default");
  orbit->transform()->setPosition({0, 0, 0}, ESpace::World);
  m_engine->componentManager()->addComponent<Behavior>("behavior", orbit)->addUpdateFunction(
      [](GameObject* self, Engine* engine, Real dt) {
        self->transform()->rotate(Math::AxisX, 40 * dt, ESpace::Local);
      });

  // Directional
  auto* light000 = m_main_scene->createGameObject("light000", "default");
  light000->transform()->setPosition({0, 40, 40}, ESpace::World);
  light000->transform()->lookAt({0, 0, 0}, {40, 0, 40});
  auto* lightComp000 = m_engine->componentManager()->addComponent<DirectionalLight>("DirectionalLight", light000, Vec3{0.8f, 0.8f, 0.8f}, 1.0f);

  // Directional
  auto* light004 = m_main_scene->createGameObject("light004", "default");
  light004->transform()->setPosition({0, 40, 0}, ESpace::World);
  light004->transform()->rotate(Math::AxisX, 90, ESpace::World);
  auto* lightComp004 = m_engine->componentManager()->addComponent<DirectionalLight>("DirectionalLight", light004, Vec3{0.8f, 0.8f, 0.8f}, 1.0f);

  auto* light005 = m_main_scene->createGameObject("light005", "default");
  light005->transform()->setPosition({0, 40, -40}, ESpace::World);
  light005->transform()->rotate(Math::AxisY, 180, ESpace::World);
  light005->transform()->lookAt({0, 0, 0}, {40, 0, -40});
  auto* lightComp005 = m_engine->componentManager()->addComponent<DirectionalLight>("DirectionalLight", light005, Vec3{0.8f, 0.8f, 0.8f}, 1.0f);

  auto* light006 = m_main_scene->createGameObject("light006", "default");
  light006->transform()->setPosition({40, 40, 0}, ESpace::World);
  light006->transform()->rotate(Math::AxisY, 90, ESpace::World);
  light006->transform()->lookAt({0, 0, 0}, {40, 40, 0});
  auto* lightComp006 = m_engine->componentManager()->addComponent<DirectionalLight>("DirectionalLight", light006, Vec3{0.8f, 0.8f, 0.8f}, 1.0f);

  // Spot
  /*
  auto* light001 = m_main_scene->createGameObject("light001", "default");
  light001->transform()->setPosition({5, 5, 10}, ESpace::World);
  //light001->transform()->setParent(orbit->transform());
//  m_engine->componentManager()->addComponent<MeshRenderer>("mm", light001, m_assets["Helico"]);
  auto* lightComp001 = m_engine->componentManager()->addComponent<SpotLight>("SpotLight", light001, Vec3{0.11f, 1.0f, 0.01f},
      1.0f, 10, 45, Attenuation{0.1, 0.1, 0.1});
  //m_engine->componentManager()->addComponent<Behavior>("behavior", light001)->addUpdateFunction(
      //HomelandBehaviors::exampleBehavior000);

  // Point
  auto* light002 = m_main_scene->createGameObject("light002", "default");
  light002->transform()->setPosition({-5, 5, 0}, ESpace::World);
  //light000->transform()->setParent(orbit->transform());
  //m_engine->componentManager()->addComponent<MeshRenderer>("m", light002, m_assets["Cube"]);
  auto* lightComp002 = m_engine->componentManager()->addComponent<PointLight>("PointLight", light002, Vec3{0.11f, 0.12f, 1.0f},
      1.0f, 10, Attenuation{0.1, 0.1, 0.1});
  m_engine->componentManager()->addComponent<Behavior>("behavior", light002)->addUpdateFunction(
      HomelandBehaviors::exampleBehavior001);
  */

  /*
  // Point
  auto* light003 = m_main_scene->createGameObject("light003", "default");
  light003->transform()->setPosition({-5, 5, -15}, ESpace::World);
  //light000->transform()->setParent(orbit->transform());
  //m_engine->componentManager()->addComponent<MeshRenderer>("m", light003, m_assets["Cube"]);
  auto* lightComp003 = m_engine->componentManager()->addComponent<PointLight>("PointLight", light003, Vec3{0.11f, 0.12f, 1.0f},
                                                                              1.0f, 10, Attenuation{0.1, 0.1, 0.1});
  //m_engine->componentManager()->addComponent<Behavior>("behavior", light002)->addUpdateFunction(HomelandBehaviors::exampleBehavior001);
   */
}

