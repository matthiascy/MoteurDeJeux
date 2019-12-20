#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_CONE_COLLIDER_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_CONE_COLLIDER_HPP

#include <Physics/Collider.hpp>

class ConeCollider : public Collider {
public:
  ConeCollider(const String& name, GameObject* gameObject, Real radius, Real height);
  ~ConeCollider() override = default;

  [[nodiscard]]
  UInt64 typeID() const override {
    return family::type<ConeCollider>;
  }
};


#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_CONE_COLLIDER_HPP */
