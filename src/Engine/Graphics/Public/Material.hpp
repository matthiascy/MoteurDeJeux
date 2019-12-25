#ifndef MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_MATERIAL_COMPONENT_HPP
#define MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_MATERIAL_COMPONENT_HPP

#include <GameFramework/Public/ECS/Component.hpp>

class Material {
public:
  /**
   * Kd : Diffuse color coefficient
   * Ka : Ambient color coefficient
   * Ks : Specular color coefficient
   * d  : Dissolve factor
   * Ni : Refraction index
   * illum :
   *       0 - disable lighting,
   *       1 - ambient & diffuse (set specular color to black),
   *       2 - full lighting
   * sharpness
   * map_Kd : Diffuse color texture map
   * map_Ks : Specular color texture map
   * map_Ka : Ambient color texture map
   * map_Bump : Bump texture map
   * map_d : Opacity texture map
   * refl : reflection type
   */
  const Real Kd;
  const Real Ka;
  const Real Ks;
  const Real d;
  const Real Ni;
  const UInt32 illum;
  const UInt32 map_Kd;

public:
  Material(const String& name, GameObject* gameObject);
};


#endif  /* !MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_MATERIAL_COMPONENT_HPP */
