#include <Graphics/OpenGL/OglOffscreenSurface.hpp>
#include <GameFramework/Systems/RenderSystem.hpp>
#include <QCoreApplication>
#include <QtGui/QImage>

OglOffscreenSurface::OglOffscreenSurface(QScreen* targetScreen, const QSize& size)
  : QOffscreenSurface(targetScreen),
    m_size(size),
    m_initializedGL(false),
    m_context(nullptr),
    m_fns(nullptr),
    m_fns4_0(nullptr),
//    m_resolvedFbo(nullptr),
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
  /*
  if (m_resolvedFbo) {
    m_resolvedFbo->release();
    delete m_resolvedFbo;
    m_resolvedFbo = nullptr;
  }
   */
  m_context->doneCurrent();
  delete m_context;
  m_context = nullptr;
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


OglFBO* OglOffscreenSurface::framebufferObject()
{
  return m_fbo;
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
  makeCurrent();
  if (m_fbo->format().samples() > 0) {
    qDebug() << "Multisample not supported yet.";
    /*
    if (m_fns4_0) {
      m_fns4_0->glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo->handle());
      //m_fns4_0->glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_resolvedFbo->handle());
      m_fns4_0->glBlitFramebuffer(0, 0, m_size.width(), m_size.height(), 0, 0, m_size.width(), m_size.height(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
      m_fns4_0->glBindFramebuffer(GL_FRAMEBUFFER, 0);
    } else {
      //m_fns->glBindFramebuffer(GL_FRAMEBUFFER, m_resolvedFbo->handle());
      m_fns->glBindFramebuffer(GL_FRAMEBUFFER, m_fbo->handle());
    }
    if (GLenum error = m_fns->glGetError() != GL_NO_ERROR) {
      qDebug() << "OglOffscreenSurface::grabFramebuffer() - OpenGL error" << error;
    }

    // now grab from resolve FBO
    qDebug() << "Resolve FBO";
    return (grab_framebuffer_internal_(m_resolvedFbo));
     */
  } else {
    // no multi-sampling.
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
  m_fns->glBindFramebuffer(GL_FRAMEBUFFER, m_fbo->handle());

  return (image.mirrored());
}


void OglOffscreenSurface::swapBuffers()
{
  swap_buffers_internal_();
  emit frameSwapped();
}


void OglOffscreenSurface::swap_buffers_internal_()
{
  m_context->makeCurrent(this);
  m_fns->glFlush();
  if (m_fns4_0) {
    m_fns4_0->glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo->handle());
    m_fns4_0->glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    m_fns4_0->glBlitFramebuffer(0, 0, m_size.width(),
                                m_size.height(), 0, 0, m_size.width(),
                                m_size.height(),
                                GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT,
                                GL_NEAREST);
    m_fns4_0->glBindFramebuffer(GL_FRAMEBUFFER, m_fbo->handle());
  } else {
    m_fns->glBindFramebuffer(GL_FRAMEBUFFER, 0);
    m_fns->glBindFramebuffer(GL_FRAMEBUFFER, m_fbo->handle());
  }

  if (GLenum error = m_fns->glGetError() != GL_NO_ERROR) {
    qDebug() << "OglOffscreenSurface::swap_buffers_internal_() - OpenGL error" << error;
  }

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

    OglFBOFormat format;
    //format.setSamples(16);
    format.setAttachment(OglFBO::CombinedDepthStencil);

    m_fbo = new OglFBO(bufferSize(), format);

    if (!m_fbo->isValid()) {
      throw ("OglOffscreenSurface::recreateFbo() - Failed to create background FBO!");
    }

    m_fbo->bind();
    m_fns->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    m_fbo->release();

    if (format.samples() > 0) {
      // TODO :: MSAA
    }
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
  if (!m_updatePending.exchange(true)) {
    QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
  }
}

void OglOffscreenSurface::resize(const QSize& size)
{
  makeCurrent();
  m_size = QSize(size);
  recreate_fbo_and_paint_device_();
  fns()->glViewport(0, 0, size.width(), size.height());
  doneCurrent();
}


QSize OglOffscreenSurface::bufferSize() const
{
  return m_size;
}
