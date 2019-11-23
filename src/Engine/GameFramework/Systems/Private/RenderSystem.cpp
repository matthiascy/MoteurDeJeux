#include <GameFramework/Systems/RenderSystem.hpp>
#include <QOpenGLFunctions>

RenderSystem::RenderSystem(String name, Engine* engine)
  : System(std::move(name), engine)
{
  qDebug() << "Render System creation =>" << m_name;
  m_context = makeUnique<QOpenGLContext>();
  m_surface = makeUnique<QOffscreenSurface>();
}

RenderSystem::~RenderSystem()
{
  m_context.reset(nullptr);
}

void RenderSystem::init()
{
  qDebug() << "\tRender system initialization";
  QSurfaceFormat format;
  format.setRenderableType(QSurfaceFormat::OpenGL);
  format.setProfile(QSurfaceFormat::CoreProfile);
  format.setVersion(4, 0);
  format.setDepthBufferSize(24);
  format.setSamples(16);
  QSurfaceFormat::setDefaultFormat(format);
  qDebug() << "\t\t1. Context creation : OpenGL Core Profile 4.6.0.";
  if (!m_context->create()) {
    qDebug() << "\t\t\t!!!Creation failed";
  }

  qDebug() << "\t\t2. OffScreenSurface creation.";
  m_surface->setFormat(m_context->format());
  m_surface->create();
  if (!m_surface->isValid())
    qDebug() << "\t\t\tCreation failed";

  m_context->makeCurrent(m_surface.get());

}
