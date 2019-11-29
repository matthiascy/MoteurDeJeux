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
#include <Core/Core.hpp>

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
  UniquePtr<QOpenGLContext>  m_ctx;
  QOpenGLFunctions*          m_fns;
  QOpenGLFunctions_4_0_Core* m_fns40;

  /** OpenGL paint device for painting with a QPainter. */
  UniquePtr<QOpenGLPaintDevice>       m_dev;

  /** Background FBO for off-screen rendering when the window is not exposed. */
  UniquePtr<QOpenGLFramebufferObject> m_fbo;

  /**
   * Background FBO resolving a multi-sampling frame buffer in m_fbo to a frame
   * buffer that can be grabbed to a QImage.
   */
  UniquePtr<QOpenGLFramebufferObject> m_resolve_fbo;

  QSize m_size;

private:
  Q_DISABLE_COPY(OglOffscreenSurface)

  /** Internal method to grab content of a framebuffer. */
  QImage grab_framebuffer_internal(QOpenGLFramebufferObject* fbo);

  /** Re-allocate FBO and paint device if needed du to size changes etc. */
  void recreate_framebuffer_and_paint_device();

public:
  explicit OglOffscreenSurface(QScreen* targetScreen = nullptr, const QSize& size = QSize(800, 600));

  ~OglOffscreenSurface() override;

  void init();

  bool isValid() const;

  [[nodiscard]]
  QOpenGLContext* context() const;

  [[nodiscard]]
  QOpenGLFunctions* fns() const;

  /** Return the OpenGL off-screen frame buffer object identifier. */
  [[nodiscard]]
  UInt32 framebufferObjectId() const;

  /** Return the OpenGL off-screen frame buffer object. */
  [[nodiscard]]
  const QOpenGLFramebufferObject* framebufferObject() const;

  /** Return the QPaintDevice for paint into. */
  [[nodiscard]]
  QPaintDevice* paintDevice() const;

  void bindFramebuffer();

  QImage grabFramebuffer();

  void makeCurrent();

  void doneCurrent();

  void swapBuffers();

  [[nodiscard]]
  QSize framebufferSize() const;

  void resize(const QSize& size);

public slots:
  void update();

  void render();

signals:
  void frameSwapped();
  void resized();

protected:
  virtual void exposeEvent(QExposeEvent* e);
  virtual void resizeEvent(QResizeEvent* e);
  bool event(QEvent* e) override;

  virtual void initializeGL() { }
  virtual void resizeGL(int width, int height) { }
  virtual void paintGL() {
    fns()->glClearColor(1.f, 0.f, 0.f, 1.f);
    fns()->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  };
};


#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_OGL_OFFSCREEN_SURFACE_HPP */
