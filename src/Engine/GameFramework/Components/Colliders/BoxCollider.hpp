#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_BOX_COLLIDER_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_BOX_COLLIDER_HPP

#include "GameFramework/Components/Collider.hpp"

class BoxCollider : public Collider {
public:
  BoxCollider(const String& name, GameObject* gameObject, const Vec3& size, Real mass);
  ~BoxCollider() override = default;
};


#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_BOX_COLLIDER_HPP */
