#include "Graphics/Public/Material.hpp"

UInt32 qHash(ETextureType key, uint seed)
{
  return ::qHash(static_cast<UInt32>(key), seed);
}

void Material::setBaseColor(const Vec3& color)
{
  m_base_color = color;
}

void Material::setAmbient(const Vec3& ambient)
{
  m_ambient_color = ambient;
}

void Material::setDiffuse(const Vec3& diffuse)
{
  m_diffuse_color = diffuse;
}

void Material::setSpecular(const Vec3& specular)
{
  m_specular_color = specular;
}

void Material::setShininess(Real shininess)
{
  m_shininess = shininess;
}

Vec3 Material::baseColor() const
{
  return m_base_color;
}

Vec3 Material::ambient() const
{
  return m_ambient_color;
}

Vec3 Material::diffuse() const
{
  return m_diffuse_color;
}

Vec3 Material::specular() const
{
  return m_specular_color;
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

String Material::namePath() const
{
  return m_name_path;
}

void Material::setNamePath(const String& namePath)
{
  m_name_path = namePath;
}
