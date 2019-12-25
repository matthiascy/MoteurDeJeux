#ifndef MOTEUR_DE_JEUX_SRC_WORLD_PHYSICS_HPP
#define MOTEUR_DE_JEUX_SRC_WORLD_PHYSICS_HPP

#include <Core/Public/Core.hpp>
#include <GameFramework/Public/ECS/System.hpp>
#include <Physics/Public/Forward.hpp>
#include <Thirdparty/bullet3/src/BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <BulletCollision/CollisionDispatch/btCollisionDispatcher.h>

class Engine;
class RigidBody;
class PhysicsDebugDraw;

// TODO: separated physics world

class PhysicsSystem : public System {
private:
  Vec3 m_gravity {Math::Down};

protected:
  UniquePtr<PhysicsWorld>     m_world;
  UniquePtr<PhysicsDebugDraw> m_debug_drawer;
  bool m_is_debug_draw_enabled;

public:
  PhysicsSystem(const String& name, Engine* engine, Object* parent = nullptr, const Vec3& gravity = Math::Down);
  ~PhysicsSystem() override;

  void init() override;

  void fixedUpdate(Real dt) override;

  void preUpdate(Real dt) override;

  void update(Real dt) override;

  void postUpdate(Real dt) override;

  [[nodiscard]]
  PhysicsWorld* physicsWorld() const;

  [[nodiscard]]
  PhysicsDebugDraw* debugDrawer() const;

  [[nodiscard]]
  bool isDebugDrawEnabled() const;
  void setIsDebugDrawEnabled(bool enabled);

private:
  /** Refresh collision only without updating dynamics. */
  void _update_collision();
};


#endif  /* !MOTEUR_DE_JEUX_SRC_WORLD_PHYSICS_HPP */
