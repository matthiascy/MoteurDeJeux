#include "Graphics/Public/Material.hpp"

UInt32 qHash(ETextureType key, uint seed)
{
  return ::qHash(static_cast<UInt32>(key), seed);
}

void Material::setColor(Vec3 color)
{
  m_base_color = color;
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

Vec3 Material::color() const
{
  return m_base_color;
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

Array<TextureHandle> Material::texturesOfType(ETextureType type) const
{
  if (m_textures.contains(type)) {

    return m_textures.value(type);

  } else {

    return {};

  }
}

void Material::addTextureOfType(ETextureType type, TextureHandle handle)
{
  if (m_textures.contains(type)) {

    if (!m_textures.value(type).contains(handle)) {
      m_textures[type].push_back(handle);
    }

  } else {

    m_textures.insert(type, {});
    m_textures[type].push_back(handle);

  }
}

Array<TextureHandle> Material::textures() const
{
  Array<TextureHandle> ts;

  for (const auto& textures : m_textures) {
    for (auto t : textures)
      ts.push_back(t);
  }

  return ts;
}

void Material::addTexturesOfType(ETextureType type, Array<TextureHandle>& textures)
{
  if (!textures.isEmpty()) {
    for (auto& t : textures) {
      addTextureOfType(type, t);
    }
  }
}
