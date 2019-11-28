#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include <QSurfaceFormat>
#include <QDebug>
#include <QPainter>
#include <QOpenGLPaintDevice>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  m_size = {200, 200};
  QSurfaceFormat format;
  format.setRenderableType(QSurfaceFormat::OpenGL);
  format.setMajorVersion(4);
  format.setMinorVersion(0);
  format.setProfile(QSurfaceFormat::CompatibilityProfile);
  m_context = new QOpenGLContext();
  m_surface = new QOffscreenSurface();
  m_context->setFormat(format);
  if (!m_context->create())
    qDebug() << "Context creation error";
  m_surface->setFormat(m_context->format());

  m_surface->create();
  if (!m_surface->isValid())
    qDebug() << "Surface creation error";

  m_context->makeCurrent(m_surface);
  m_fbo = new QOpenGLFramebufferObject(m_size);

  QOpenGLFunctions* f = m_context->functions();
  m_fbo->bind();
  QOpenGLPaintDevice device(m_size);
  QPainter painter(&device);
  painter.setRenderHint(QPainter::RenderHint::Antialiasing, true);
  painter.beginNativePainting();
  f->glClearColor(1.0f, 0.0f, 0.0f, 1.0);
  f->glClear(GL_COLOR_BUFFER_BIT);



  painter.endNativePainting();
  painter.end();
  QImage img = m_fbo->toImage();
  img.save("fbo.png");
  ui->label->setPixmap(QPixmap::fromImage(img));
  m_fbo->release();
  m_context->doneCurrent();
}

MainWindow::~MainWindow()
{
  delete ui;
  delete m_fbo;
  delete m_surface;
}

