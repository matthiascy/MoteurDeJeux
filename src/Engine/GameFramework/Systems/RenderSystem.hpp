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

  UniquePtr<OglOffscreenSurface>   m_surface;

  HashMap<GameObject*, RenderInfo> m_render_infos;

  /** Arrays own the memory. */
  Array<OglVAO*>     m_vaos;
  Array<OglBuffer*>  m_vbos;
  Array<OglBuffer*>  m_ebos;
  Array<OglProgram*> m_programs;

  Mat4 m_view_matrix;
  Mat4 m_model_matrix;
  Mat4 m_projection_matrix;

  OglProgram* program;
  OglVAO* vao;
  OglBuffer* vbo;
  OglBuffer* ebo;

public:
  RenderSystem(const String& name, Engine* engine, Object* parent = nullptr);
  ~RenderSystem() override;

  void init() override;

  void fixedUpdate(Real dt) override { };

  void preUpdate(Real dt) override { };

  void update(Real dt) override { };

  void postUpdate(Real dt) override { };

  void renderScene(Scene* scene);

  [[nodiscard]]
  QImage grabFramebuffer() const;

private:
  void render(GameObject* gameObject);

public slots:
  void resize(const QSize& size);
};

#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_SUBSYSTEMS_RENDERER_SYSTEM_HPP */
