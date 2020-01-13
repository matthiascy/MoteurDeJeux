#include <Graphics/Public/Texture.hpp>

Texture::Texture(const String& path, Image* image, ETextureType type)
{
  m_name_path = path;
  m_type = type;
  //qDebug() << "Create Texture : " << path;
  m_ogl_texture = makeUnique<OglTexture>(image->mirrored());
}

Texture::Texture(const String& path, ETextureType type)
{
  m_name_path = path;
  m_type = type;
  //qDebug() << "Create Texture : " << path;
  m_ogl_texture = makeUnique<OglTexture>(Image(path).mirrored());
}

Texture::~Texture()
{
  m_ogl_texture.reset(nullptr);
}

bool Texture::isSamePathWith(const Texture& other)
{
  return m_name_path == other.m_name_path;
}

OglTexture* Texture::oglTexture() const
{
  return m_ogl_texture.get();
}

ETextureType Texture::type() const
{
  return m_type;
}

String Texture::namePath() const
{
  return m_name_path;
}
