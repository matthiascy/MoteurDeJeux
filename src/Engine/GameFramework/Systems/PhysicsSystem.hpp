#ifndef MOTEUR_DE_JEUX_SRC_WORLD_PHYSICS_HPP
#define MOTEUR_DE_JEUX_SRC_WORLD_PHYSICS_HPP

#include <Core/Core.hpp>
#include <GameFramework/System.hpp>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>

class Engine;
class RigidBody;
class DebugDrawer;

// TODO: separated physics world

class PhysicsSystem : public System {
private:
  Vec3 m_gravity {Math::Down};

protected:
  btDefaultCollisionConfiguration*     m_config;
  btCollisionDispatcher*               m_dispatcher;
  btBroadphaseInterface*               m_broad_phase;
  btSequentialImpulseConstraintSolver* m_solver;
  btDiscreteDynamicsWorld*             m_world;

  Array<btCollisionShape*> m_collision_shapes;
  btCollisionObjectArray   m_collision_objects;

  UniquePtr<DebugDrawer> m_debug_drawer;

public:
  PhysicsSystem(const String& name, Engine* engine, Object* parent = nullptr, const Vec3& gravity = Math::Down);
  ~PhysicsSystem() override;

  void init() override;

  [[nodiscard]]
  Vec3 gravity() const;

  void setGravity(const Vec3& gravity);

  void fixedUpdate(Real dt) override;

  void preUpdate(Real dt) override;

  void update(Real dt) override;

  void postUpdate(Real dt) override;

  [[nodiscard]]
  btDiscreteDynamicsWorld* physicsWorld() const;

  [[nodiscard]]
  DebugDrawer* debugDrawer() const;
};


#endif  /* !MOTEUR_DE_JEUX_SRC_WORLD_PHYSICS_HPP */
