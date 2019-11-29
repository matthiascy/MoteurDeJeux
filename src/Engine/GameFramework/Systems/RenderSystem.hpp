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
#include <Graphics/OglOffscreenSurface.hpp>

// TODO forward
class GameObject;
class Engine;
class QOpenGLPaintDevice;
class Scene;

class RenderSystem : public System {
private:
  struct RenderInfo {
    Int32 vboIdx {};
    Int32 eboIdx {};
  };

private:
  UniquePtr<OglOffscreenSurface>      m_surface;
  /*
  UniquePtr<QOpenGLVertexArrayObject> m_vao;
  UniquePtr<Array<QOpenGLBuffer*>>    m_vbos;
  UniquePtr<Array<QOpenGLBuffer*>>    m_ebos;
  UniquePtr<QOpenGLFramebufferObject> m_fbo;
  UniquePtr<QOpenGLShaderProgram>     m_program;
  QSize m_size;
  Mat4 m_projection;
  Mat4 m_view;
  Mat4 m_model;

  QOpenGLPaintDevice* m_device;
  QPainter*           m_painter;
   */

  HashMap<GameObject*, RenderInfo> m_render_map;

  QOpenGLFunctions* m_fns;

public:
  RenderSystem(String name, Engine* engine);
  ~RenderSystem() override;

  void init() override;

  void renderScene(Scene* scene);

  [[nodiscard]]
  QImage grabFramebuffer() const;

private:
  void render(GameObject* gameObject);

};

#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_SUBSYSTEMS_RENDERER_SYSTEM_HPP */
