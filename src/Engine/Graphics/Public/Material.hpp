#ifndef MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_MATERIAL_COMPONENT_HPP
#define MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_MATERIAL_COMPONENT_HPP

#include <GameFramework/Public/ECS/Component.hpp>
#include <Graphics/Public/Forward.hpp>

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

class Material {
private:
  Vec3 m_base_color { };

  Real m_ambient   { 0.0f };
  Real m_diffuse   { 0.0f };
  Real m_specular  { 0.0f };
  Real m_shininess { 0.0f };

  HashMap<ETextureType, Array<TextureHandle>> m_textures;

public:
  [[nodiscard]]
  Vec3 color() const;

  void setColor(Vec3 color);

  [[nodiscard]]
  Real ambient() const;

  void setAmbient(Real ambient);

  [[nodiscard]]
  Real diffuse() const;

  void setDiffuse(Real diffuse);

  [[nodiscard]]
  Real specular() const;

  void setSpecular(Real specular);

  [[nodiscard]]
  Real shininess() const;

  void setShininess(Real shininess);

  [[nodiscard]]
  Array<TextureHandle> texturesOfType(ETextureType type) const;

  [[nodiscard]]
  Array<TextureHandle> textures() const;

  void addTextureOfType(ETextureType type, TextureHandle handle);

  void addTexturesOfType(ETextureType type, Array<TextureHandle>& textures);

public:
  Material() = default;
};


#endif  /* !MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_MATERIAL_COMPONENT_HPP */
