#ifndef MOTEUR_DE_JEUX_SRC_PHYSICS_FORWARD_HPP
#define MOTEUR_DE_JEUX_SRC_PHYSICS_FORWARD_HPP

class btCollisionConfiguration;
class btCollisionShape;
class btBroadphaseInterface;
class btConstraintSolver;
class btDiscreteDynamicsWorld;
class btDispatcher;
class btPersistentManifold;
class btTypedConstraint;
class btRigidBody;
class btSequentialImpulseConstraintSolver;
class btCompoundShape;
class btDynamicsWorld;
class btIDebugDraw;

class PhysicsSystem;
class PhysicsWorld;
class ManifoldPair;
class RigidBody;
class Constraint;
class Collider;
class PhysicsDebugDraw;
class CollisionShape;
class PhysicsRayCastResult;
class PhysicsWorldConfiguration;
class PhysicsWorld;

enum class CollisionShapeType {
  Box,
  Sphere,
  Cylinder,
  Capsule,
  Cone,
  StaticPlane,
  ConvexHull,
  Terrain,
  TriangleMesh,
  GImpactMesh,
};

enum class ConstraintType {
  Point,
  Hinge,
  Slider,
  ConeTwist
};

#endif  /* !MOTEUR_DE_JEUX_SRC_PHYSICS_FORWARD_HPP */
