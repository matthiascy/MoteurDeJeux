#ifndef MOTEUR_DE_JEUX_SRC_PHYSICS_PUBLIC_PHYSICS_WORLD_HPP
#define MOTEUR_DE_JEUX_SRC_PHYSICS_PUBLIC_PHYSICS_WORLD_HPP

#include <Core/Public/Core.hpp>
#include "Forward.hpp"
#include "BulletForward.hpp"

struct ManifoldPair {
  /** Manifold without the body pointers flipped. */
  btPersistentManifold* m_manifold         { nullptr };
  /** Manifold with the body pointers flipped. */
  btPersistentManifold* m_manifold_flipped { nullptr };
};

class PhysicsWorld {
private:
  UniquePtr<btCollisionConfiguration> m_configuration;
  UniquePtr<btDispatcher>             m_dispatcher;
  UniquePtr<btBroadphaseInterface>    m_broadphase;
  UniquePtr<btConstraintSolver>       m_constraint_solver;
  UniquePtr<btDiscreteDynamicsWorld>  m_dynamics_world;
  Array<RigidBody*>                   m_rigid_bodies;
  Array<CollisionShape*>              m_collision_shapes;
  Array<Constraint*>                  m_constraints;
  /**< Collision pairs on this frame. */
  HashMap<Pair<RigidBody*, RigidBody*>, ManifoldPair> m_curr_collisions;
  /**< Collision pairs on the previous frame. */
  HashMap<Pair<RigidBody*, RigidBody*>, ManifoldPair> m_prev_collisions;

public:

};


#endif  /* !MOTEUR_DE_JEUX_SRC_PHYSICS_PUBLIC_PHYSICS_WORLD_HPP */
