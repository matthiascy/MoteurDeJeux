#ifndef MOTEUR_DE_JEUX_SRC_PHYSICS_PUBLIC_BULLET_PHYSICS_WORLD_HPP
#define MOTEUR_DE_JEUX_SRC_PHYSICS_PUBLIC_PHYSICS_WORLD_HPP

#include <Core/Public/Core.hpp>
#include "Forward.hpp"

struct ManifoldPair {
  /** Manifold without the body pointers flipped. */
  btPersistentManifold* m_manifold         { nullptr };
  /** Manifold with the body pointers flipped. */
  btPersistentManifold* m_manifold_flipped { nullptr };
};

struct PhysicsRayCastResult {
  Vec3 hitPosition { };
  Vec3 hitNormal   { };
  Real hitDistance { };
  Real hitFraction { };
  RigidBody* body  { };
};

struct PhysicsWorldConfiguration {
  btCollisionConfiguration* collisionConfiguration { nullptr };
};

class PhysicsWorld {

  friend void _internal_pre_tick_callback(btDynamicsWorld* world, btScalar timeStep);

  friend void _internal_tick_callback(btDynamicsWorld* world, btScalar timeStep);

private:
  UniquePtr<PhysicsWorldConfiguration> m_configuration;
  UniquePtr<btDispatcher>              m_collision_dispatcher;
  UniquePtr<btBroadphaseInterface>     m_broadphase;
  UniquePtr<btConstraintSolver>        m_constraint_solver;
  UniquePtr<btDiscreteDynamicsWorld>   m_dynamics_world;
  Array<RigidBody*>                    m_rigid_bodies;
  Array<CollisionShape*>               m_collision_shapes;
  Array<Constraint*>                   m_constraints;
  PhysicsDebugDraw*                    m_debug_drawer;

  /**< Collision pairs on this frame. */
  HashMap<Pair<RigidBody*, RigidBody*>, ManifoldPair> m_curr_collisions;
  /**< Collision pairs on the previous frame. */
  HashMap<Pair<RigidBody*, RigidBody*>, ManifoldPair> m_prev_collisions;

public:
  constexpr static const Int32 kMaxSolverIterations = 256;
  constexpr static const Vec3  kDefaultGravity = Vec3(0, -9.81, 0);

public:
  PhysicsWorld();
  ~PhysicsWorld();

  void update(Real delta);

  void updateCollisions();

  void setGravity(const Vec3& gravity);

  [[nodiscard]]
  Vec3 gravity() const;

  void setConstraintSolverIterations(int num);

  [[nodiscard]]
  Int32 constraintSolverIterations() const;

  void setIsSplitImpulseModeEnabled(bool enable);

  [[nodiscard]]
  bool isSplitImpulseModeEnabled() const;

  /**
   * Add a rigid body to keep track of.
   * @param body RigidBody to be added.
   */
  void addRigidBody(RigidBody* body);

  /**
   * Remove a rigid body from world.
   * @param body RigidBody to be removed.
   */
  void removeRigidBody(RigidBody* body);

  void addCollisionShape(CollisionShape* shape);

  void removeCollisionShape(CollisionShape* shape);

  void addConstraint(Constraint* constraint);

  void removeConstraint(Constraint* constraint);

  void setDebugDrawer(PhysicsDebugDraw* debugDrawer);

  [[nodiscard]]
  PhysicsDebugDraw* debugDrawer() const;

  [[nodiscard]]
  btDiscreteDynamicsWorld* world() const;

  /**
   * Returns rigid bodies that have been in collision with the specified body on
   * the last simulation step.
   */
  Array<RigidBody*> collidingRigidBodies(const RigidBody* body);

  void preStep(Real delta);

  void postStep(Real delta);
};


#endif  /* !MOTEUR_DE_JEUX_SRC_PHYSICS_PUBLIC_PHYSICS_WORLD_HPP */
