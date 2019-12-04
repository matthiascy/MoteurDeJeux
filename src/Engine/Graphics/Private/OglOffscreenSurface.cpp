#include <Graphics/OglOffscreenSurface.hpp>
#include <QCoreApplication>
#include <QtGui/QImage>

OglOffscreenSurface::OglOffscreenSurface(QScreen* targetScreen, const QSize& size)
  : QOffscreenSurface(targetScreen),
    m_vaos{}, m_vbos{}, m_ebos{}, m_programs{}
    , m_size(size)
    , m_initializedGL(false)
    , m_context(nullptr)
    , m_fns(nullptr)
    , m_fns4_0(nullptr)
    , m_paintDevice(nullptr)
    , m_fbo(nullptr)
    , m_resolvedFbo(nullptr)
{
  setFormat(QSurfaceFormat::defaultFormat());
  m_initialized = false;
  m_updatePending = false;
  create();  // Some platforms require this function to be called on the main (GUI) thread.
  initialize_internal_();
}


OglOffscreenSurface::~OglOffscreenSurface()
{
  m_context->makeCurrent(this);

  for(auto& vao : m_vaos) {
    if (vao) {
      vao->release();
      delete vao;
      vao = nullptr;
    }
  }

  for (auto& vbo : m_vbos) {
    if (vbo) {
      vbo->release();
      delete vbo;
      vbo = nullptr;
    }
  }

  for (auto& ebo : m_ebos) {
    if (ebo) {
      ebo->release();
      delete ebo;
      ebo = nullptr;
    };
  }

  for (auto& program : m_programs) {
    if (program) {
      program->release();
      delete program;
      program = nullptr;
    }
  }

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
  // destroy shader
  //delete m_blitShader;
  m_blitShader = nullptr;
  // free context
  m_context->doneCurrent();
  delete m_context;
  m_context = nullptr;
  // free paint device
  delete m_paintDevice;
  m_paintDevice = nullptr;
  m_initialized = false;
  m_updatePending = false;
  destroy();
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
      m_fns4_0->glBlitFramebuffer(0, 0, bufferSize().width(),
                                  bufferSize().height(), 0, 0, bufferSize().width(),
                                  bufferSize().height(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
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
    return (grabFramebufferInternal(m_resolvedFbo));
  } else {
    // no multi-sampling. grab directly from FBO
    return (grabFramebufferInternal(m_fbo));
  }
}  // OglOffscreenSurface::grabFramebuffer


QImage OglOffscreenSurface::grabFramebufferInternal(OglFBO* fbo)
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
}  // OglOffscreenSurface::grabFramebufferInternal


void OglOffscreenSurface::swapBuffers()
{
  swapBuffersInternal();
  emit frameSwapped();
}


void OglOffscreenSurface::swapBuffersInternal()
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
    m_fns4_0->glBlitFramebuffer(0, 0, bufferSize().width(),
                                bufferSize().height(), 0, 0, bufferSize().width(),
                                bufferSize().height(), GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT,
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
    qDebug() << "OglOffscreenSurface::swapBuffersInternal() - OpenGL error" << error;
  }
  // now swap back buffer to front buffer
  m_context->swapBuffers(this);
}  // OglOffscreenSurface::swapBuffersInternal


void OglOffscreenSurface::recreateFBOAndPaintDevice()
{
  if (m_context && ((m_fbo == nullptr) || (m_fbo->size() != bufferSize()))) {
    m_context->makeCurrent(this);
    // free old FBOs
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
    //format.setSamples(0);

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
      // create resolve framebuffer with only a color attachment
      format.setAttachment(OglFBO::NoAttachment);
      //format.setSamples(0);
      m_resolvedFbo = new OglFBO(bufferSize(), format);
      if (!m_resolvedFbo->isValid()) {
        throw ("OglOffscreenSurface::recreateFbo() - Failed to create resolve FBO!");
      }
      // clear resolve framebuffer
      m_resolvedFbo->bind();
      m_fns->glClear(GL_COLOR_BUFFER_BIT);
      m_resolvedFbo->release();
    }
  }
  // create paint device for painting with QPainter if needed
  if (!m_paintDevice) {
    m_paintDevice = new OglPaintDevice;
  }
  // update paint device size if needed
  if (m_paintDevice->size() != bufferSize()) {
    m_paintDevice->setSize(bufferSize());
  }
}  // OglOffscreenSurface::recreateFBOAndPaintDevice


void OglOffscreenSurface::initialize_internal_()
{
  if (!m_initialized.exchange(true)) {
    qInfo() << "\t\t- Context creation : OpenGL Core Profile "
            << qPrintable(String::number(QSurfaceFormat::defaultFormat().majorVersion())) << "."
            << qPrintable(String::number(QSurfaceFormat::defaultFormat().minorVersion()));

    m_context = new OglContext(this);
    //m_context->setFormat(format);

    if (m_context->create()) {
      m_context->makeCurrent(this);
      m_fns = m_context->functions();
      m_fns->initializeOpenGLFunctions();
      m_fns4_0 = m_context->versionFunctions<OglFnsCore4_0>();
      if (m_fns4_0) {
        m_fns4_0->initializeOpenGLFunctions();
      } else {
        // if we do not have OpenGL 3.0 functions, glBlitFrameBuffer is not available, so we
        // must do the blit
        // using a shader and the framebuffer texture, so we need to create the shader
        // here...
        // --> allocate m_blitShader, a simple shader for drawing a textured quad
        // --> build quad geometry, VBO, whatever
      }
      // now we have a context, create the FBO
      recreateFBOAndPaintDevice();
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
  // check if we need to initialize stuff
  initialize_internal_();
  // check if we need to call the user initialization
//    makeCurrent(); // TODO: may be makeCurrent() must be here, as noted for QOpenGLWidget.initializeGL()
  if (!m_initializedGL) {
    m_initializedGL = true;
    initializeGL();
  }
  // make context current and bind framebuffer
  makeCurrent();
  bindFramebufferObject();
  // call user paint function
  paintGL();
  doneCurrent();
  // mark that we're done with updating
  m_updatePending = false;
}


void OglOffscreenSurface::exposeEvent(QExposeEvent* e)
{
  // render window content if window is exposed
  render();
}


void OglOffscreenSurface::resizeEvent(QResizeEvent* e)
{
  // call base implementation
  resize(e->size());
  emit resized();
}


void OglOffscreenSurface::resize(const QSize& newSize)
{
  m_mutex.lock();

  makeCurrent();
  m_size = QSize(newSize);
  // update FBO and paint device
  recreateFBOAndPaintDevice();
  m_mutex.unlock();
  // call user-defined resize method
  resizeGL(bufferSize().width(), bufferSize().height());
}


void OglOffscreenSurface::resize(
    int w,
    int h)
{
  resize(QSize(w, h));
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

UInt32 OglOffscreenSurface::createVAO()
{
  makeCurrent();
  auto* vao = new OglVAO;
  vao->create();
  m_vaos.push_back(vao);
  doneCurrent();
  return m_vaos.size() - 1;
}

UInt32 OglOffscreenSurface::createVBO(UInt32 vao)
{
  makeCurrent();
  m_vaos[vao]->bind();
  auto* vbo = new OglBuffer(OglBuffer::VertexBuffer);
  vbo->create();
  m_vbos.push_back(vbo);
  doneCurrent();
  return m_vbos.size() - 1;
}

UInt32 OglOffscreenSurface::createEBO(UInt32 vao)
{
  makeCurrent();
  m_vaos[vao]->bind();
  auto* ebo = new OglBuffer(OglBuffer::IndexBuffer);
  ebo->create();
  m_ebos.push_back(ebo);
  doneCurrent();
  return m_ebos.size() - 1;
}

UInt32 OglOffscreenSurface::createProgram(const String& vert, const String& frag)
{
  makeCurrent();
  auto* program = new OglProgram;
  program->addShaderFromSourceFile(OglShader::Vertex, vert);
  program->addShaderFromSourceFile(OglShader::Fragment, frag);
  program->bindAttributeLocation("inPosition", 0);
  program->bindAttributeLocation("inNormal", 1);
  program->bindAttributeLocation("inTexCoord", 2);
  program->link();
  program->bind();
  program->setUniformValue("texture0", 0);
  program->setUniformValue("texture1", 1);
  program->setUniformValue("texture2", 2);
  program->setUniformValue("texture3", 3);
  m_programs.push_back(program);
  doneCurrent();
  return m_programs.size() - 1;
}
