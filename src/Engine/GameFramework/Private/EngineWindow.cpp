#include <GameFramework/Engine.hpp>
#include <GameFramework/EngineWindow.hpp>
#include <QPainter>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLayout>
#include <QtWidgets/QPushButton>
#include <QtUiTools/QUiLoader>
#include <QtWidgets/QLCDNumber>
#include <GameFramework/GameApp.hpp>

EngineWindow::EngineWindow(GameApp* app, QWidget* parent)
  : QWidget(parent), m_app{app}, m_image{}//, m_ui{nullptr}
{
  /*
  QUiLoader loader;
  QFile file(":/UI/MainMenu");
  file.open(QFile::ReadOnly);
  m_ui = loader.load(&file, this);
  m_ui->setStyleSheet("background-color: rgba(0,0,0,0)");
  file.close();
   */
  // TODO:: set render system size
  setMinimumSize(1024, 768);

  QFont font("Arial", 16, QFont::Bold);
  auto* fpsLabel = new QLabel("FPS : ", this);
  fpsLabel->setMaximumSize(64, 32);
  fpsLabel->setGeometry(16, 16, 48, 16);
  fpsLabel->setFont(font);

  m_fps_widget = makeUnique<QLCDNumber>(this);
  m_fps_widget->setGeometry(56, 16, 60, 16);
  m_fps_widget->setStyleSheet("QLCDNumber{ background-color: rgba(0,0,0,0);}");
  m_fps_widget->setFont(font);

  connect(m_app, SIGNAL(fpsChanged(double)), m_fps_widget.get(), SLOT(display(double)));
}

void EngineWindow::paintEvent(QPaintEvent *)
{
  m_app->engine()->renderSystem()->renderScene(nullptr);
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
  m_fps_widget.reset(nullptr);
}

QLCDNumber* EngineWindow::fpsWidget() const
{
  return m_fps_widget.get();
}

void EngineWindow::closeEvent(QCloseEvent*)
{
  m_app->quit();
}
