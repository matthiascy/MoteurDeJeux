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

#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_PUBLIC_FORWARD_HPP */
