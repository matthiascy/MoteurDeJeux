#include <Graphics/SplashScreen.hpp>
#include <QEvent>

SplashScreen::SplashScreen(const QPixmap& pixmap, Qt::WindowFlags f)
  : QSplashScreen(pixmap, f)
{
  installEventFilter(this);
}

bool SplashScreen::eventFilter(QObject *target, QEvent *event)
{
  Q_UNUSED(target)

  return (event->type() == QEvent::MouseButtonPress) ||
         (event->type() == QEvent::MouseButtonDblClick) ||
         (event->type() == QEvent::MouseButtonRelease) ||
         (event->type() == QEvent::KeyPress) ||
         (event->type() == QEvent::KeyRelease);
}
