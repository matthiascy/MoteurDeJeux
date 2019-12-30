#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_SUBSYSTEMS_RENDERER_SYSTEM_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_SUBSYSTEMS_RENDERER_SYSTEM_HPP

#include <Core/Public/Core.hpp>
#include <GameFramework/Public/ECS/System.hpp>
#include <QOpenGLContext>
#include <QOffscreenSurface>
#include <QOpenGLBuffer>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLFramebufferObject>
#include <QtGui/QOpenGLVertexArrayObject>
#include <Graphics/Public/OpenGL/OglOffscreenSurface.hpp>
#include <GameFramework/Public/Forward.hpp>
#include <Graphics/Public/Forward.hpp>

class RenderSystem : public System {
public:
  struct physics_debug_draw_info_t {
    Int32 vboIdx        { -1 };
    Int32 vaoIdx        { -1 };
    Int32 programIdx    { -1 };
    Int32 vertShaderIdx { -1 };
    Int32 fragShaderIdx { -1 };
    Int32 size          {  0 };
  };

private:
  UniquePtr<OglOffscreenSurface>   m_surface;

  /** Arrays own the memory. */
  Array<OglVAO*>     m_vaos;
  Array<OglBuffer*>  m_vbos;
  Array<OglBuffer*>  m_ibos;
  Array<OglShader*>  m_shaders;
  Array<OglProgram*> m_programs;

  Mat4 m_view_matrix;
  Mat4 m_model_matrix;
  Mat4 m_projection_matrix;

  OglFns*        m_fns;
  OglFnsCore4_0* m_fns4_0;
  bool m_is_initialized;

  physics_debug_draw_info_t m_physics_debug_draw_info { };

  bool m_is_physics_debug_draw_enabled;

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

  OglOffscreenSurface* offscreenSurface() { return m_surface.get(); }

  void makeCurrent();

  void doneCurrent();

  Int32 createBufferObject(OglBuffer::Type type);

  VboHandle createVertexBufferObject();

  IboHandle createIndexBufferObject();

  Int32 createVertexArrayObject();

  Int32 createShaderProgram();

  Int32 createShader(OglShader::ShaderTypeBit type);

  [[nodiscard]]
  physics_debug_draw_info_t& physicsDebugDrawInfo();

  [[nodiscard]]
  const physics_debug_draw_info_t& physicsDebugDrawInfo() const;

  OglBuffer* vboAt(Int32 idx);

  OglVAO*    vaoAt(Int32 idx);

  OglBuffer* iboAt(Int32 idx);

  OglProgram* programAt(Int32 idx);

  [[nodiscard]]
  OglFnsCore4_0* fns();

private:
  void _render(const GameObject* gameObject, OglProgram* program, Real dt);

  /***
   * Render the game scene.
   * @param scene [in] Scene to be rendered.
   */
  void _render_scene(Scene* scene, Real dt);

  /* Used only for destroy render system arrays. */
  template <typename T>
  void _destroy_array(Array<T*>& array);

  void _init_physics_system_debug_draw();

  void _physics_system_debug_draw(Camera* camera);

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
