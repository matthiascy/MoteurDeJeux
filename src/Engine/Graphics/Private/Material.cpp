#include "Graphics/Public/Material.hpp"

void Material::setColor(Vec3 color)
{
  m_color = color;
}

void Material::setAmbient(Real ambient)
{
  m_ambient = ambient;
}

void Material::setDiffuse(Real diffuse)
{
  m_diffuse = diffuse;
}

void Material::setSpecular(Real specular)
{
  m_specular = specular;
}

void Material::setShininess(Real shininess)
{
  m_shininess = shininess;
}

void Material::setDiffuseTexture(TextureHandle texture)
{
  m_diffuse_texture = texture;
}

void Material::setSpecularTexture(TextureHandle texture)
{
  m_specular_texture = texture;
}

void Material::setBumpTexture(TextureHandle texture)
{
  m_bump_texture = texture;
}

Vec3 Material::color() const
{
  return m_color;
}

Real Material::ambient() const
{
  return m_ambient;
}

Real Material::diffuse() const
{
  return m_diffuse;
}

Real Material::specular() const
{
  return m_specular;
}

Real Material::shininess() const
{
  return m_shininess;
}

TextureHandle Material::diffuseTexture() const
{
  return m_diffuse_texture;
}

TextureHandle Material::specularTexture() const
{
  return m_specular_texture;
}

TextureHandle Material::bumpTexture() const
{
  return m_bump_texture;
}
