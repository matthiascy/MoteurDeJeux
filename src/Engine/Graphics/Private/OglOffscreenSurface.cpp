#include <Graphics/OglOffscreenSurface.hpp>
#include <QCoreApplication>
#include <QtGui/QImage>

OglOffscreenSurface::OglOffscreenSurface(QScreen* targetScreen, const QSize& size)
  : QOffscreenSurface(targetScreen),
    m_is_initialized{false}, m_is_update_pending{false}, m_is_gl_initialized{false},
    m_mutex{}, m_ctx{nullptr}, m_fns{nullptr}, m_fns40{nullptr}, m_dev{nullptr},
    m_fbo{nullptr}, m_resolve_fbo{nullptr}, m_size(size)
{ }

OglOffscreenSurface::~OglOffscreenSurface()
{
  m_ctx->makeCurrent(this);
  if (m_fbo)
    m_fbo->release();
  m_fbo.reset(nullptr);
  if (m_resolve_fbo)
    m_resolve_fbo->release();
  m_resolve_fbo.reset(nullptr);
  m_dev.reset(nullptr);
  destroy();
}

QOpenGLContext* OglOffscreenSurface::context() const
{
  return m_ctx.get();
}

QOpenGLFunctions* OglOffscreenSurface::fns() const
{
  return m_fns;
}

UInt32 OglOffscreenSurface::framebufferObjectId() const
{
  return (m_fbo ? m_fbo->handle() : 0);
}

const QOpenGLFramebufferObject* OglOffscreenSurface::framebufferObject() const
{
  return m_fbo.get();
}

QPaintDevice* OglOffscreenSurface::paintDevice() const
{
  return m_dev.get();
}

void OglOffscreenSurface::bindFramebuffer()
{
  if (m_fbo) {
    m_fbo->bind();
  } else {
    QOpenGLFramebufferObject::bindDefault();
  }
}

bool OglOffscreenSurface::isValid() const
{
  return m_is_initialized && m_ctx && m_fbo;
}

void OglOffscreenSurface::makeCurrent()
{
  if (isValid()) {
    m_ctx->makeCurrent(this);
  } else {
    qDebug() << "[Fatal]: OglOffscreenSurface::makeCurrent() - Window not yet properly initialized!";
  }
}

void OglOffscreenSurface::doneCurrent()
{
  if (m_ctx) {
    m_ctx->doneCurrent();
  }
}

QImage OglOffscreenSurface::grabFramebuffer() {
  std::lock_guard<std::mutex> locker(m_mutex);
  makeCurrent();
  // Blit framebuffer to resolve framebuffer first if needed
  if (m_fbo->format().samples() > 0) {
    // Check if we have glFrameBufferBlit support
    if (m_fns40) {
      // Only blit the color attachment
      m_fns40->glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo->handle());
      m_fns40->glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_resolve_fbo->handle());
      m_fns40->glBlitFramebuffer(0, 0, m_size.width(), m_size.height(),
          0, 0, m_size.width(), m_size.height(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
      m_fns40->glBindFramebuffer(GL_FRAMEBUFFER, 0);
    } else {
      // Unbind the FBO here, so we can use its texture and bind the default back-buffer
      m_fns->glBindFramebuffer(GL_FRAMEBUFFER, m_resolve_fbo->handle());
      // Now use its texture for drawing in the shader
      // --> bind shader and draw textured quad here
      // bind regular FBO again
      m_fns->glBindFramebuffer(GL_FRAMEBUFFER, m_fbo->handle());
    }

    if (GLenum error = m_fns->glGetError() != GL_NO_ERROR) {
      qDebug() << "OglOffscreenSurface::grabFramebuffer() - OpenGL error" << error;
    }

    // Now grab from back FBO
    return grab_framebuffer_internal(m_resolve_fbo.get());

  } else {
    // No multi-sampling, grab directly from FBO
    return grab_framebuffer_internal(m_fbo.get());
  }

}

QImage OglOffscreenSurface::grab_framebuffer_internal(QOpenGLFramebufferObject* fbo) {
  QImage image;

  m_fns->glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo->handle());
  if (m_fns40) {
    m_fns40->glReadBuffer(GL_COLOR_ATTACHMENT0);
  }
  GLenum internal_format = fbo->format().internalTextureFormat();
  bool has_alpha = internal_format == GL_RGBA || internal_format == GL_BGRA || internal_format == GL_RGBA8;

  if (internal_format == GL_BGRA) {
    image = QImage(fbo->size(), has_alpha ? QImage::Format_ARGB32 : QImage::Format_RGB32);
    m_fns->glReadPixels(0, 0, fbo->size().width(), fbo->size().height(),
        GL_BGRA, GL_UNSIGNED_BYTE, image.bits());
  } else if (internal_format == GL_RGBA || internal_format == GL_RGBA8) {
    image = QImage(fbo->size(), has_alpha ? QImage::Format_RGBA8888 : QImage::Format_RGBX8888);
    m_fns->glReadPixels(0, 0, fbo->size().width(), fbo->size().height(), GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
  } else {
    qDebug() << "OglOffscreenSurface::grabFramebuffer() - Unsupported framebuffer form"
             << internal_format << "!";
  }
  m_fns->glBindFramebuffer(GL_FRAMEBUFFER, m_fbo->handle());

  return image.mirrored();
}

void OglOffscreenSurface::swapBuffers()
{
  // Blit framebuffer to back buffer
  m_ctx->makeCurrent(this);
  // Make sure all paint operation have been processed.
  m_fns->glFlush();
  // Check if we have glFrameBufferBlit support
  if (m_fns40) {
    // If our framebuffer has multi-sampling, the resolve should be done automagically
    m_fns40->glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo->handle());
    m_fns40->glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    // Blit all buffers including depth buffer for further rendering
    m_fns40->glBlitFramebuffer(0, 0, m_size.width(), m_size.height(),
        0, 0, m_size.width(), m_size.height(),
        GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT,
        GL_NEAREST);
    m_fns40->glBindFramebuffer(GL_FRAMEBUFFER, m_fbo->handle());
  } else {
    // We must unbind the FBO here, so we can use its texture and bind the default back-buffer
    m_fns->glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // Now use its texture for drawing in the shader
    // --> bind shader and draw textured quad here
    // bind regular FBO again
    m_fns->glBindFramebuffer(GL_FRAMEBUFFER, m_fbo->handle());
  }

  if (GLenum error = m_fns->glGetError() != GL_NO_ERROR) {
    qDebug() << "OglOffscreenSurface::swapBuffers() - OpenGL error" << error;
  }

  m_ctx->swapBuffers(this);

  emit frameSwapped();
}

void OglOffscreenSurface::recreate_framebuffer_and_paint_device()
{
  if (m_ctx && ((m_fbo == nullptr) || (m_fbo->size() != m_size))) {
    m_ctx->makeCurrent(this);
    // Free old FBOs
    if (m_fbo) {
      m_fbo->release();
      m_fbo.reset(nullptr);
    }
    if (m_resolve_fbo) {
      m_resolve_fbo->release();
      m_resolve_fbo.reset(nullptr);
    }
    // Create new framebuffer
    QOpenGLFramebufferObjectFormat fbo_format;
    fbo_format.setSamples(QSurfaceFormat::defaultFormat().samples());
    m_fbo = makeUnique<QOpenGLFramebufferObject>(m_size, fbo_format);
    if (!m_fbo->isValid()) {
      throw ("OpenGlOffscreenSurface::recreateFbo() - Failed to create background FBO!");
    }
    // Clear framebuffer
    m_fbo->bind();
    m_fns->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    m_fbo->release();
    // If multi-sampling is requested and supported we need a resolve FBO
    if (fbo_format.samples() > 0) {
      // Create resolve framebuffer with only a color attachment
      fbo_format.setAttachment(QOpenGLFramebufferObject::NoAttachment);
      m_resolve_fbo = makeUnique<QOpenGLFramebufferObject>(m_size, fbo_format);
      if (!m_resolve_fbo->isValid())
        throw ("OpenGlOffscreenSurface::recreateFbo() - Failed to create background FBO!");

      // Clear resolve framebuffer
      m_resolve_fbo->bind();
      m_fns->glClear(GL_COLOR_BUFFER_BIT);
      m_resolve_fbo->release();
    }
  }
  // Create paint device for painting with QPainter if need
  if (!m_dev) {
    m_dev = makeUnique<QOpenGLPaintDevice>();
  }
  // Update paint device size if needed
  if (m_dev->size() != m_size) {
    m_dev->setSize(m_size);
  }
}

QSize OglOffscreenSurface::framebufferSize() const
{
  return m_size;
}

void OglOffscreenSurface::resize(const QSize& size)
{
  m_mutex.lock();
  makeCurrent();
  m_size = size;
  // Update FBO and paint device
  recreate_framebuffer_and_paint_device();
  m_mutex.unlock();
  resizeGL(m_size.width(), m_size.height());
}

void OglOffscreenSurface::init()
{
  if (!m_is_initialized.exchange(true)) {
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(4, 0);
    format.setDepthBufferSize(24);
    format.setSamples(16);
    QSurfaceFormat::setDefaultFormat(format);
    m_ctx = std::make_unique<QOpenGLContext>(this);
    m_ctx->setFormat(format);

    if (m_ctx->create()) {
      m_ctx->makeCurrent(this);
      m_fns = m_ctx->functions();
      m_fns->initializeOpenGLFunctions();
      m_fns40 = m_ctx->versionFunctions<QOpenGLFunctions_4_0_Core>();
      if (m_fns40)
        m_fns40->initializeOpenGLFunctions();
      recreate_framebuffer_and_paint_device();
    } else {
      m_is_initialized = false;
      m_ctx.reset(nullptr);
      throw ("Failed to create OpenGL context!");
    }
  }
}

void OglOffscreenSurface::update()
{
  // Only queue an update if there's not already an update pending
  if (!m_is_update_pending.exchange(true)) {
    QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
  }
}

bool OglOffscreenSurface::event(QEvent* e)
{
  switch (e->type()) {
    case QEvent::UpdateLater: {
      update();
      return true;
    }

    case QEvent::UpdateRequest: {
      render();
      return true;
    }

    default:
      return false;
  }
}

void OglOffscreenSurface::render()
{
  std::lock_guard<std::mutex> locker(m_mutex);
  if (!m_is_gl_initialized) {
    initializeGL();
    m_is_gl_initialized = true;
  }

  makeCurrent();
  bindFramebuffer();
  // call user paint function
  paintGL();
  doneCurrent();
  // mark that we're done with updating
  m_is_update_pending = false;
}

void OglOffscreenSurface::resizeEvent(QResizeEvent* e)
{
  resize(e->size());
  emit resized();
}

void OglOffscreenSurface::exposeEvent(QExposeEvent *e)
{
  render();
}
