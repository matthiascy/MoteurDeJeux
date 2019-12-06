#include <Graphics/OglOffscreenSurface.hpp>
#include <QCoreApplication>
#include <QtGui/QImage>

OglOffscreenSurface::OglOffscreenSurface(QScreen* targetScreen, const QSize& size)
  : QOffscreenSurface(targetScreen),
    m_size(size),
    m_initializedGL(false),
    m_context(nullptr),
    m_fns(nullptr),
    m_fns4_0(nullptr),
    m_paintDevice(nullptr),
    m_resolvedFbo(nullptr),
    m_initialized{false},
    m_fbo{nullptr},
    m_updatePending{false}
{
  qInfo() << "Creation";
  setFormat(QSurfaceFormat::defaultFormat());
  create();  // Some platforms require this function to be called on the main (GUI) thread.
  init();
  qInfo() << "Creation[Finished]";
}


OglOffscreenSurface::~OglOffscreenSurface()
{
  qDebug() << "Shutting down...";
  m_context->makeCurrent(this);
  if (m_fbo) {
    m_fbo->release();
    delete m_fbo;
    m_fbo = nullptr;
  }
  if (m_resolvedFbo) {
    m_resolvedFbo->release();
    delete m_resolvedFbo;
    m_resolvedFbo = nullptr;
  }
  m_context->doneCurrent();
  delete m_context;
  m_context = nullptr;
  delete m_paintDevice;
  m_paintDevice = nullptr;
  m_initialized = false;
  m_updatePending = false;
  destroy();
  qDebug() << "Shutting down...[Finished]";
}


OglContext* OglOffscreenSurface::context() const
{
  return m_context;
}


OglFns* OglOffscreenSurface::fns() const
{
  return m_fns;
}


GLuint OglOffscreenSurface::framebufferObjectHandle() const
{
  return (m_fbo ? m_fbo->handle() : 0);
}


const OglFBO* OglOffscreenSurface::getFramebufferObject() const
{
  return (m_fbo);
}


QPaintDevice* OglOffscreenSurface::getPaintDevice() const
{
  return (m_paintDevice);
}


void OglOffscreenSurface::bindFramebufferObject()
{
  if (m_fbo) {
    m_fbo->bind();
  } else {
    OglFBO::bindDefault();
  }
}


bool OglOffscreenSurface::isValid() const
{
  return (m_initialized && m_context && m_fbo);
}


void OglOffscreenSurface::makeCurrent()
{
  if (isValid()) {
    m_context->makeCurrent(this);
  } else {
    qFatal("OglOffscreenSurface::makeCurrent() - Window not yet properly initialized!!!");
  }
}


void OglOffscreenSurface::doneCurrent()
{
  if (m_context) {
    m_context->doneCurrent();
  }
}


QImage OglOffscreenSurface::grabFramebuffer()
{
  std::lock_guard <std::mutex> locker(m_mutex);
  makeCurrent();
  // blit framebuffer to resolve framebuffer first if needed
  if (m_fbo->format().samples() > 0) {
    // check if we have glFrameBufferBlit support. this is true for desktop OpenGL 3.0+, but not
    // OpenGL ES 2.0
    if (m_fns4_0) {
      // only blit the color buffer attachment
      m_fns4_0->glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo->handle());
      m_fns4_0->glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_resolvedFbo->handle());
      /*
      m_fns4_0->glBlitFramebuffer(0, 0, bufferSize().width(),
                                  bufferSize().height(), 0, 0, bufferSize().width(),
                                  bufferSize().height(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
      */
      m_fns4_0->glBlitFramebuffer(0, 0, m_size.width(), m_size.height(), 0, 0, m_size.width(), m_size.height(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
      m_fns4_0->glBindFramebuffer(GL_FRAMEBUFFER, 0);
    } else {
      // we must unbind the FBO here, so we can use its texture and bind the default
      // back-buffer
      m_fns->glBindFramebuffer(GL_FRAMEBUFFER, m_resolvedFbo->handle());
      // now use its texture for drawing in the shader
      // --> bind shader and draw textured quad here
      // bind regular FBO again
      m_fns->glBindFramebuffer(GL_FRAMEBUFFER, m_fbo->handle());
    }
    // check if OpenGL errors happened
    if (GLenum error = m_fns->glGetError() != GL_NO_ERROR) {
      qDebug() << "OglOffscreenSurface::grabFramebuffer() - OpenGL error" << error;
    }

    // now grab from resolve FBO
    return (grab_framebuffer_internal_(m_resolvedFbo));
  } else {
    // no multi-sampling. grab directly from FBO
    return (grab_framebuffer_internal_(m_fbo));
  }
}


QImage OglOffscreenSurface::grab_framebuffer_internal_(OglFBO* fbo)
{
  QImage image;
  // bind framebuffer first
  m_fns->glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo->handle());
  if (m_fns4_0) {
    m_fns4_0->glReadBuffer(GL_COLOR_ATTACHMENT0);
  }
  GLenum internalFormat = fbo->format().internalTextureFormat();
  bool hasAlpha = internalFormat == GL_RGBA || internalFormat == GL_BGRA
                  || internalFormat == GL_RGBA8;
  if (internalFormat == GL_BGRA) {
    image = QImage(fbo->size(), hasAlpha ? QImage::Format_ARGB32 : QImage::Format_RGB32);
    m_fns->glReadPixels(0, 0, fbo->size().width(),
                        fbo->size().height(), GL_BGRA, GL_UNSIGNED_BYTE, image.bits());
  } else if ((internalFormat == GL_RGBA) || (internalFormat == GL_RGBA8)) {
    image = QImage(fbo->size(), hasAlpha ? QImage::Format_RGBA8888 : QImage::Format_RGBX8888);
    m_fns->glReadPixels(0, 0, fbo->size().width(),
                        fbo->size().height(), GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
  } else {
    qDebug() << "OglOffscreenSurface::grabFramebuffer() - Unsupported framebuffer format"
             << internalFormat << "!";
  }
  //m_fns->glBindFramebuffer(GL_FRAMEBUFFER, m_fbo->handle());

  //image.save("offscreen_debug.png");

  return (image.mirrored());
}


void OglOffscreenSurface::swapBuffers()
{
  swap_buffers_internal_();
  emit frameSwapped();
}


void OglOffscreenSurface::swap_buffers_internal_()
{
  // blit framebuffer to back buffer
  m_context->makeCurrent(this);
  // make sure all paint operation have been processed
  m_fns->glFlush();
  // check if we have glFrameBufferBlit support. this is true for desktop OpenGL 3.0+, but not
  // OpenGL ES 2.0
  if (m_fns4_0) {
    // if our framebuffer has multi-sampling, the resolve should be done automagically
    m_fns4_0->glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo->handle());
    m_fns4_0->glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    // blit all buffers including depth buffer for further rendering
    /*
    m_fns4_0->glBlitFramebuffer(0, 0, bufferSize().width(),
                                bufferSize().height(), 0, 0, bufferSize().width(),
                                bufferSize().height(), GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT,
                                GL_NEAREST);
                                */
    m_fns4_0->glBlitFramebuffer(0, 0, m_size.width(),
                                m_size.height(), 0, 0, m_size.width(),
                                m_size.height(),
                                GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT,
                                GL_NEAREST);
    m_fns4_0->glBindFramebuffer(GL_FRAMEBUFFER, m_fbo->handle());
  } else {
    // we must unbind the FBO here, so we can use its texture and bind the default back-buffer
    m_fns->glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // now use its texture for drawing in the shader
    // --> bind shader and draw textured quad here
    // bind regular FBO again
    m_fns->glBindFramebuffer(GL_FRAMEBUFFER, m_fbo->handle());
  }
  // check if OpenGL errors happened
  if (GLenum error = m_fns->glGetError() != GL_NO_ERROR) {
    qDebug() << "OglOffscreenSurface::swap_buffers_internal_() - OpenGL error" << error;
  }
  // now swap back buffer to front buffer
  m_context->swapBuffers(this);
}


void OglOffscreenSurface::recreate_fbo_and_paint_device_()
{
  if (m_context && ((m_fbo == nullptr) || (m_fbo->size() != bufferSize()))) {
    m_context->makeCurrent(this);
    if (m_fbo) {
      m_fbo->release();
      delete m_fbo;
      m_fbo = nullptr;
    }

    if (m_resolvedFbo) {
      m_resolvedFbo->release();
      delete m_resolvedFbo;
      m_resolvedFbo = nullptr;
    }

    OglFBOFormat format;

    m_fbo = new OglFBO(bufferSize(), format);
    if (!m_fbo->isValid()) {
      throw ("OglOffscreenSurface::recreateFbo() - Failed to create background FBO!");
    }
    // clear framebuffer
    m_fbo->bind();
    m_fns->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    m_fbo->release();
    // if multi sampling is requested and supported we need a resolve FBO
    if (format.samples() > 0) {
      qDebug() << "Create resolve FBO";
      // create resolve framebuffer with only a color attachment
      format.setAttachment(OglFBO::NoAttachment);
      //format.setSamples(0);
      m_resolvedFbo = new OglFBO(m_size, format);
      if (!m_resolvedFbo->isValid()) {
        throw ("OglOffscreenSurface::recreateFbo() - Failed to create resolve FBO!");
      }
      // clear resolve framebuffer
      m_resolvedFbo->bind();
      m_fns->glClear(GL_COLOR_BUFFER_BIT);
      m_resolvedFbo->release();
    }
    //m_context->doneCurrent();
  }
  // create paint device for painting with QPainter if needed
  if (!m_paintDevice) {
    m_paintDevice = new OglPaintDevice;
  }
  // update paint device size if needed
  if (m_paintDevice->size() != bufferSize()) {
    m_paintDevice->setSize(bufferSize());
  }
}


void OglOffscreenSurface::init()
{
  if (!m_initialized.exchange(true)) {
    qInfo() << qPrintable("Creation[OpenGL Context]: OpenGL Core Profile " +
               String::number(QSurfaceFormat::defaultFormat().majorVersion()) + "." +
               String::number(QSurfaceFormat::defaultFormat().minorVersion()));

    m_context = new OglContext(this);
    //m_context->setFormat(QSurfaceFormat::defaultFormat());

    if (m_context->create()) {

      m_context->makeCurrent(this);
      m_fns = m_context->functions();
      m_fns->initializeOpenGLFunctions();
      m_fns4_0 = m_context->versionFunctions<OglFnsCore4_0>();

      if (m_fns4_0)
        m_fns4_0->initializeOpenGLFunctions();

      recreate_fbo_and_paint_device_();

    } else {

      m_initialized = false;
      delete m_context;
      m_context = nullptr;

      qFatal("Failed to create OpenGL context 4.0");
    }
  }
}


void OglOffscreenSurface::update()
{
  // only queue an update if there's not already an update pending
  if (!m_updatePending.exchange(true)) {
    QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
  }
}


void OglOffscreenSurface::render()
{
  std::lock_guard <std::mutex> locker(m_mutex);

  makeCurrent();

  bindFramebufferObject();

  doneCurrent();

  m_updatePending = false;
}

void OglOffscreenSurface::resize(const QSize& size)
{
  m_mutex.lock();
  makeCurrent();
  m_size = QSize(size);
  recreate_fbo_and_paint_device_();
  fns()->glViewport(0, 0, size.width(), size.height());
  doneCurrent();
  m_mutex.unlock();
}


bool OglOffscreenSurface::event(QEvent* event)
{
  switch (event->type()) {
    case QEvent::UpdateLater: {
      update();
      return true;
    }

    case QEvent::UpdateRequest: {
      render();
      return true;
    }

    default: {
      return false;
    }
  }
}


QSize OglOffscreenSurface::bufferSize() const
{
  return m_size;
}
