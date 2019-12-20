#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_SUBSYSTEMS_RENDERER_SYSTEM_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_SUBSYSTEMS_RENDERER_SYSTEM_HPP

#include <Core/Core.hpp>
#include <GameFramework/System.hpp>
#include <QOpenGLContext>
#include <QOffscreenSurface>
#include <QOpenGLBuffer>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLFramebufferObject>
#include <QtGui/QOpenGLVertexArrayObject>
#include <Graphics/OpenGL/OglOffscreenSurface.hpp>

// TODO forward
class GameObject;
class Engine;
class QOpenGLPaintDevice;
class Scene;

class RenderSystem : public System {
private:
  struct RenderInfo {
    Int32 vboIdx {};

    struct ibo_ {
      Int32  idx {};
      Int32 size {};
    } ibo {};

    Array<Int32> texIds;
  };

  UniquePtr<OglOffscreenSurface>   m_surface;

  HashMap<GameObject*, RenderInfo> m_render_graph;

  /** Arrays own the memory. */
  Array<OglVAO*>     m_vaos;
  Array<OglBuffer*>  m_vbos;
  Array<OglBuffer*>  m_ibos;
  Array<OglProgram*> m_programs;
  Array<OglTexture*> m_textures;

  Mat4 m_view_matrix;
  Mat4 m_model_matrix;
  Mat4 m_projection_matrix;

  OglFns* m_fns;

public:
  RenderSystem(const String& name, Engine* engine, Object* parent = nullptr);
  ~RenderSystem() override;

  void init() override;

  void fixedUpdate(Real dt) override { };

  void preUpdate(Real dt) override { };

  void update(Real dt) override;

  void postUpdate(Real dt) override { };

  /**
   * Grab the rendered frame buffer from offscreen surface.
   * @return Frame buffer as an image.
   */
  [[nodiscard]]
  QImage grabFramebuffer() const;

  OglOffscreenSurface* offscreenSurface() {
    return m_surface.get();
  }

private:
  /**
   * Render the game object with the corresponding render info and shader program.
   * @param gameObject [in] GameObject to be rendered.
   * @param info       [in] gameObject's RenderInfo.
   * @param program    [in] shader program used to render gameObject.
   */
  void _render(const GameObject* gameObject, const RenderInfo& info, OglProgram* program);

  /***
   * Render the game scene.
   * @param scene [in] Scene to be rendered.
   */
  void _render_scene(Scene* scene);

  /* Used only for destroy render system arrays. */
  template <typename T>
  void _destroy_array(Array<T*>& array);

public slots:
  void resize(const QSize& size);
};

template<typename T>
void RenderSystem::_destroy_array(Array<T*>& array)
{
  for (auto* ptr : array) {
    ptr->release();
    delete ptr;
    ptr = nullptr;
  }
}

#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_SUBSYSTEMS_RENDERER_SYSTEM_HPP */
