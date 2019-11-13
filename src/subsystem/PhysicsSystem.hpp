#ifndef MOTEUR_DE_JEUX_SRC_WORLD_PHYSICS_HPP
#define MOTEUR_DE_JEUX_SRC_WORLD_PHYSICS_HPP

#include "../Types.hpp"

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>

class PhysicsSystem {
protected:
  btDefaultCollisionConfiguration*     m_config;
  btCollisionDispatcher*               m_dispatcher;
  btBroadphaseInterface*               m_cache;
  btSequentialImpulseConstraintSolver* m_solver;
  btDiscreteDynamicsWorld*             m_world;

  Array<btCollisionShape*> m_collision_shapes;
  btCollisionObjectArray   m_collision_objects;

public:
  explicit PhysicsSystem(const Vec3& gravity = vec3::DirDown);
  virtual ~PhysicsSystem();

  void setGravity(const Vec3& gravity);
  const Vec3& gravity() const;

  void update(Real dt);
};


#endif  /* !MOTEUR_DE_JEUX_SRC_WORLD_PHYSICS_HPP */
