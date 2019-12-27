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
  Vec3 m_base_color { 1, 1, 1 };

  Vec3 m_ambient_color   { 1, 1, 1 };
  Vec3 m_diffuse_color   { 1, 1, 1 };
  Vec3 m_specular_color  { Math::Zero };
  Real m_shininess { 4 };

  HashMap<ETextureType, Array<TextureHandle>> m_textures;

  String m_name_path;

public:
  [[nodiscard]]
  Vec3 baseColor() const;

  void setBaseColor(const Vec3& color);

  [[nodiscard]]
  Vec3 ambient() const;

  void setAmbient(const Vec3& ambient);

  [[nodiscard]]
  Vec3 diffuse() const;

  void setDiffuse(const Vec3& diffuse);

  [[nodiscard]]
  Vec3 specular() const;

  void setSpecular(const Vec3& specular);

  [[nodiscard]]
  Real shininess() const;

  void setShininess(Real shininess);

  [[nodiscard]]
  Array<TextureHandle> texturesOfType(ETextureType type) const;

  [[nodiscard]]
  Array<TextureHandle> textures() const;

  void addTextureOfType(ETextureType type, TextureHandle handle);

  void addTexturesOfType(ETextureType type, Array<TextureHandle>& textures);

  [[nodiscard]]
  String namePath() const;

  void setNamePath(const String& namePath);

public:
  Material() = default;
};


#endif  /* !MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_MATERIAL_COMPONENT_HPP */
