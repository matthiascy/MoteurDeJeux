#ifndef MOTEUR_DE_JEUX_SRC_WORLD_PHYSICS_HPP
#define MOTEUR_DE_JEUX_SRC_WORLD_PHYSICS_HPP

#include <Core/Core.hpp>
#include <GameFramework/System.hpp>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>

class Engine;

class PhysicsSystem : public System {
protected:
  btDefaultCollisionConfiguration*     m_config;
  btCollisionDispatcher*               m_dispatcher;
  btBroadphaseInterface*               m_cache;
  btSequentialImpulseConstraintSolver* m_solver;
  btDiscreteDynamicsWorld*             m_world;

  Array<btCollisionShape*> m_collision_shapes;
  btCollisionObjectArray   m_collision_objects;

public:
  PhysicsSystem(const String& name, Engine* engine, Object* parent = nullptr, const Vec3& gravity = Math::Down);
  ~PhysicsSystem() override;

  void init() override;

  [[nodiscard]]
  Vec3 gravity() const;

  void setGravity(const Vec3& gravity);

  void update(Real dt);
};


#endif  /* !MOTEUR_DE_JEUX_SRC_WORLD_PHYSICS_HPP */
