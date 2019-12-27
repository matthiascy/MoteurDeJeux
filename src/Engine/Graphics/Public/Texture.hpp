#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_TEXTURE_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_TEXTURE_HPP

#include <Core/Public/Core.hpp>
#include <Core/Public/Container.hpp>
#include <Graphics/Public/Forward.hpp>
#include <Graphics/Public/OpenGL/OglTypes.hpp>

class Texture {
private:
  String m_name_path;
  ETextureType m_type;
  UniquePtr<OglTexture> m_ogl_texture;

public:
  explicit Texture(const String& path, ETextureType type = ETextureType::None);
  ~Texture();

  [[nodiscard]]
  OglTexture* oglTexture() const;

  [[nodiscard]]
  ETextureType type() const;

  [[nodiscard]]
  String namePath() const;

  bool isSamePathWith(const Texture& other);
};


#endif //HOMELAND_TEXTURE_HPP
