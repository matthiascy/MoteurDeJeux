#include <GameFramework/Engine.hpp>
#include <GameFramework/EngineWindow.hpp>
#include <QPainter>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLayout>
#include <QtWidgets/QPushButton>

EngineWindow::EngineWindow(Engine* engine, QWidget* parent)
  : QWidget(parent), m_engine{engine}, m_image{}
{ }

void EngineWindow::paintEvent(QPaintEvent *)
{
  m_engine->renderSystem()->renderScene(nullptr);

  m_image = m_engine->renderSystem()->grabFramebuffer();
  {
    QPainter painter;
    painter.begin(this);
    painter.drawImage(rect(), m_image, m_image.rect());
    painter.end();
  }
}
