#include <GameFramework/Engine.hpp>
#include <GameFramework/EngineWindow.hpp>
#include <QPainter>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLayout>
#include <QtWidgets/QPushButton>
//#include <QtUiTools/QUiLoader>
#include <QtWidgets/QLCDNumber>
#include <GameFramework/GameApp.hpp>

EngineWindow::EngineWindow(GameApp* app, QWidget* parent)
  : QWidget(parent), m_app{app}, m_image{},
    ui_exit_btn{new QPushButton("EXIT", this)}//, m_ui{nullptr}
{
  // TODO:: set render system size
  //setWindowFlags(Qt::FramelessWindowHint);

  QFont font("Arial", 16, QFont::Bold);
  auto* fpsLabel = new QLabel("FPS : ", this);
  fpsLabel->setMaximumSize(64, 32);
  fpsLabel->setGeometry(16, 16, 48, 16);
  fpsLabel->setFont(font);

  ui_fps = makeUnique<QLCDNumber>(this);
  ui_fps->setGeometry(56, 16, 60, 16);
  ui_fps->setStyleSheet("QLCDNumber{ background-color: rgba(0,0,0,0);}");
  ui_fps->setFont(font);

  ui_exit_btn->setGeometry(16, 48, 64, 20);
  ui_exit_btn->setFont(font);

  connect(ui_exit_btn.get(), &QPushButton::pressed, m_app, &GameApp::quit);
  connect(m_app, SIGNAL(fpsChanged(double)), ui_fps.get(), SLOT(display(double)));
  connect(this, &EngineWindow::windowResized, m_app->engine()->renderSystem(), &RenderSystem::resize);
}

void EngineWindow::paintEvent(QPaintEvent*)
{
  // TODO::Better way to deal with update mismatch while resizing
  m_image = m_app->engine()->renderSystem()->grabFramebuffer();

  {
    QPainter painter;
    painter.begin(this);
    painter.drawImage(rect(), m_image, m_image.rect());
    painter.end();
  }
}

EngineWindow::~EngineWindow()
{
  ui_fps.reset(nullptr);
  ui_exit_btn.reset(nullptr);
}

QLCDNumber* EngineWindow::fpsWidget() const
{
  return ui_fps.get();
}

void EngineWindow::closeEvent(QCloseEvent*)
{
  m_app->quit();
}

void EngineWindow::resizeEvent(QResizeEvent* e)
{
  emit windowResized(e->size());
}
