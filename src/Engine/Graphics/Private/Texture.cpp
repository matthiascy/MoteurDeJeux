#include <Graphics/Public/Texture.hpp>

Texture::Texture(const String& path, ETextureType type)
{
  m_path = path;
  m_type = type;
  m_ogl_texture = makeUnique<OglTexture>(Image(path).mirrored());
}

Texture::~Texture()
{
  m_ogl_texture.reset(nullptr);
}

bool Texture::isSamePathWith(const Texture& other)
{
  return m_path == other.m_path;
}

OglTexture* Texture::oglTexture() const
{
  return m_ogl_texture.get();
}

ETextureType Texture::type() const
{
  return m_type;
}

String Texture::path() const
{
  return m_path;
}
