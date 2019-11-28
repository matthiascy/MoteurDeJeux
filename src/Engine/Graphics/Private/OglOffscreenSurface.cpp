#include <Graphics/OglOffscreenSurface.hpp>

OglOffscreenSurface::OglOffscreenSurface(QScreen* targetScreen, const QSize& size)
  : QOffscreenSurface(targetScreen),
    m_is_initialized{false}, m_is_update_pending{false}, m_is_gl_initialized{false},
    m_mutex{}, m_ctx{nullptr}, m_fns{nullptr}, m_fns40{nullptr}, m_dev{nullptr},
    m_fbo{nullptr}, m_resolved_fbo{nullptr}, m_blit_shader{nullptr}, m_size(size)
{ }

QOpenGLContext *OglOffscreenSurface::context() const
{
  return m_ctx;
}

QOpenGLFunctions *OglOffscreenSurface::fns() const
{
  return m_fns;
}

UInt32 OglOffscreenSurface::framebufferObjectId() const
{
  return (m_fbo ? m_fbo->handle() : 0);
}
