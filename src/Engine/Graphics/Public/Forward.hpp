#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_PUBLIC_FORWARD_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_PUBLIC_FORWARD_HPP

class Light;
class Mesh;
class Material;
class PointLight;
class Shader;
class MeshRenderer;
class Renderer;
class DirectionalLight;
class SpotLight;
class PointLight;
class SplashScreen;
class Model;
class Texture;

#define DECLARE_HANDLE(name)                     \
  struct name##Handle {                          \
    Int32 idx;                                   \
    friend bool operator==(const name##Handle& a, const name##Handle& b) {  \
        return a.idx == b.idx;  \
    }  \
  };

DECLARE_HANDLE(Texture)
DECLARE_HANDLE(Mesh)
DECLARE_HANDLE(Material)
DECLARE_HANDLE(Model)
DECLARE_HANDLE(Audio)

enum class ETextureType {
  /** The texture is combined with the result of the diffuse lighting equation. */
  Diffuse,
  /** The texture is combined with the result of the specular lighting equation. */
  Specular,
  /** The texture is combined with the result of the ambient lighting equation. */
  Ambient,
  /** The texture is added to the result of the lighting calculation. It isn't influenced by incoming light. */
  Emissive,
  /** The texture is a height map. */
  Height,
  /** The texture is a (tangent space) normal-map. */
  Normals,
  /** The texture defines the glossiness of the material. */
  Shininess,
  /** The texture defines per-pixel opacity. */
  Opacity,
  /** Displacement texture **/
  Displacement,
  /** Lightmap texture (aka Ambient Occlusion) **/
  LightMap,
  /** Reflection texture. **/
  Reflection,
  None,
  Unknown,
};

UInt32 qHash(ETextureType key, uint seed);

#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_PUBLIC_FORWARD_HPP */
