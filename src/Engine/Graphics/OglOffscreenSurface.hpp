#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_OGL_OFFSCREEN_SURFACE_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_OGL_OFFSCREEN_SURFACE_HPP

#include <QtGui/QOffscreenSurface>
#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QOpenGLFramebufferObject>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QOpenGLFunctions_4_0_Core>
#include <QtGui/QExposeEvent>
#include <QtGui/QResizeEvent>
#include <QtGui/QOpenGLShaderProgram>

#include <atomic>
#include <mutex>
#include <Core/BasicTypes.hpp>

/**
 * Constructor => .setFormat => .create => .init
 */

class OglOffscreenSurface : public QOffscreenSurface {
  Q_OBJECT

private:
  std::atomic_bool m_is_initialized;
  std::atomic_bool m_is_update_pending;
  std::atomic_bool m_is_gl_initialized;

  std::mutex m_mutex;

  /** OpenGL context */
  QOpenGLContext*            m_ctx;

  QOpenGLFunctions*          m_fns;
  QOpenGLFunctions_4_0_Core* m_fns40;

  /** OpenGL paint device for painting with a QPainter. */
  QOpenGLPaintDevice*       m_dev;

  /** Background FBO for off-screen rendering when the window is not exposed. */
  QOpenGLFramebufferObject* m_fbo;

  /**
   * Background FBO resolving a multi-sampling frame buffer in m_fbo to a frame
   * buffer that can be grabbed to a QImage.
   */
  QOpenGLFramebufferObject* m_resolved_fbo;


  /** Shader used for blitting m_fbo to screen if glBlitFrameBuffer is not available. */
  QOpenGLShaderProgram* m_blit_shader;

  QSize m_size;

private:
  Q_DISABLE_COPY(OglOffscreenSurface)

  /** Internal method to swap the buffer, not using mutex. */
  void swap_buffer_internal();

  /** Internal method to check state and make the context current, not using mutex. */
  void make_current_internal();

  /** Internal method to grab content of a framebuffer. */
  QImage grab_framebuffer_internal(QOpenGLFramebufferObject* fbo);

  /** Re-allocate FBO and paint device if needed du to size changes etc. */
  void recreate_framebuffer_and_paint_device();

public:
  explicit OglOffscreenSurface(QScreen* targetScreen = nullptr, const QSize& size = QSize(800, 600));
  virtual ~OglOffscreenSurface();

  void init();

  bool isValid() const;

  [[nodiscard]] QOpenGLContext* context() const;

  [[nodiscard]] QOpenGLFunctions* fns() const;

  /** Return the OpenGL off-screen frame buffer object identifier. */
  [[nodiscard]] UInt32 framebufferObjectId() const;

  /** Return the OpenGL off-screen frame buffer object. */
  const QOpenGLFramebufferObject* framebufferObject() const;

  /** Return the QPaintDevice for paint into. */
  QPaintDevice* paintDevice() const;

  void bindFramebuffer();

  QImage grabFramebuffer();

  void makeCurrent();

  void doneCurrent();

  void swapBuffers();

  QSize framebufferSize() const;

  void resize(const QSize& size);

  void resize(int w, int h);

public slots:
  void update();

  void render();

signals:
  void frameSwapped();

  void resized();

protected:
  virtual void exposeEvent(QExposeEvent* e);
  virtual void resizeEvent(QResizeEvent* e);
  virtual bool event(QEvent* e) override;
  virtual void initializeGL() = 0;
  virtual void resizeGL(int width, int height) = 0;
  virtual void paintGL() = 0;
};


#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_OGL_OFFSCREEN_SURFACE_HPP */
