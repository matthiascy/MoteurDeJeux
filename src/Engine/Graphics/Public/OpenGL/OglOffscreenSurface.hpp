#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_OGL_OFFSCREEN_SURFACE_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_OGL_OFFSCREEN_SURFACE_HPP

#include <QtGui/QOffscreenSurface>
#include <QtGui/QExposeEvent>
#include <QtGui/QResizeEvent>

#include <atomic>
#include <mutex>
#include <Core/Public/BasicTypes.hpp>
#include <Core/Public/Core.hpp>

#include <Graphics/Public/Types.hpp>

class RenderSystem;

/**
 * Constructor => .setFormat => .create => .init
 */

class OglOffscreenSurface : public QOffscreenSurface {

  Q_OBJECT

public:
  explicit OglOffscreenSurface(QScreen* targetScreen = nullptr, const QSize& size = QSize (1280, 720));

  ~OglOffscreenSurface() override;

  /// @brief Check if the window is initialized and can be used for rendering.
  /// @return Returns true if context, surface and FBO have been set up to start rendering.
  [[nodiscard]]
  bool isValid() const;

  /// @brief Return the context used in this window.
  /// @return The context used in this window or nullptr if it hasn't been created yet.
  [[nodiscard]]
  OglContext* context() const;

  /// @brief Return the OpenGL function object that can be used the issue OpenGL commands.
  /// @return The functions for the context or nullptr if it the context hasn't been created yet.
  [[nodiscard]]
  OglFns* fns() const;

  [[nodiscard]]
  OglFnsCore4_0* fnsCore40() const;

  OglFBO* framebufferObject();

  /// @brief Return the current contents of the FBO.
  /// @return FBO content as 32bit QImage. You might need to swap RGBA to BGRA or vice-versa.
  QImage grabFramebuffer();

  /// @brief Makes the OpenGL context current for rendering.
  /// @note Make sure to bindFramebufferObject() if you want to render to this widgets FBO.
  void makeCurrent();

  /// @brief Release the OpenGL context.
  void doneCurrent();

  /// @brief Copy content of framebuffer to back buffer and swap buffers if the surface is
  /// double-buffered.
  /// If the surface is not double-buffered, the frame buffer content is blitted to the front
  /// buffer.
  /// If the window is not exposed, only the OpenGL pipeline is glFlush()ed so the framebuffer can
  /// be read back.
  void swapBuffers();

  /// @brief Use bufferSize() instead size() for get a size of a surface buffer. We can't override size() as it is not virtual.
  [[nodiscard]]
  QSize bufferSize() const;

  void resize(const QSize& size);

public slots:
  /// @brief Lazy update routine like QWidget::update().
  void update();

signals:
  /// @brief Emitted when swapBuffers() was called and buffer swapping is done.
  void frameSwapped();

private:
  Q_DISABLE_COPY(OglOffscreenSurface)
  /// @brief Initialize the window.
  void init();

  /// @brief Internal method that does the actual swap work, NOT using a mutex.
  void _swap_buffers_internal();

  /// @brief Internal method to grab content of a specific framebuffer.
  QImage grab_framebuffer_internal_(OglFBO* fbo);

  /// @brief (Re-)allocate FBO and paint device if needed due to size changes etc.
  void recreate_fbo_and_paint_device_();

  /// @brief False before the window was first exposed OR render() was called.
  std::atomic_bool m_initialized;
  /// @brief False before the overridden initializeGL() was first called.
  bool m_initializedGL;
  /// @brief True when currently a window update is pending.
  std::atomic_bool m_updatePending;

  /// @brief OpenGL render context.
  OglContext* m_context;
  /// @brief The OpenGL 2.1 / ES 2.0 function object that can be used the issue OpenGL commands.
  OglFns* m_fns;
  /// @brief The OpenGL 3.0 function object that can be used the issue OpenGL commands.
  OglFnsCore4_0* m_fns4_0;

  /// @brief Background FBO for off-screen rendering when the window is not exposed.
  OglFBO* m_fbo;
  /// @brief Background FBO resolving a multi sampling frame buffer in m_fbo to a frame buffer
  /// that can be grabbed to a QImage.
//  OglFBO* m_resolvedFbo;

  QSize m_size;

  UniquePtr<OglDebugLogger> m_debug_logger;
};


#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_OGL_OFFSCREEN_SURFACE_HPP */
