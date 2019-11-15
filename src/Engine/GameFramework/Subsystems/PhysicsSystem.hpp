#ifndef MOTEUR_DE_JEUX_SRC_WORLD_PHYSICS_HPP
#define MOTEUR_DE_JEUX_SRC_WORLD_PHYSICS_HPP

#include <Engine/Core/Core.hpp>
#include <Engine/GameFramework/System.hpp>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>

class Scene;

class PhysicsSystem : System {
protected:
  btDefaultCollisionConfiguration*     m_config;
  btCollisionDispatcher*               m_dispatcher;
  btBroadphaseInterface*               m_cache;
  btSequentialImpulseConstraintSolver* m_solver;
  btDiscreteDynamicsWorld*             m_world;

  Array<btCollisionShape*> m_collision_shapes;
  btCollisionObjectArray   m_collision_objects;

public:
  PhysicsSystem(String name, Scene* scene, const Vec3& gravity = vec3::DirDown);
  virtual ~PhysicsSystem();

  void init() override;

  void setGravity(const Vec3& gravity);
  const Vec3& gravity() const;

  void update(Real dt);
};


#endif  /* !MOTEUR_DE_JEUX_SRC_WORLD_PHYSICS_HPP */
