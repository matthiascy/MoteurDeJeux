#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_SPLASH_SCREEN_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_SPLASH_SCREEN_HPP

#include <QtWidgets/QSplashScreen>

class SplashScreen : public QSplashScreen {
  Q_OBJECT
public:
  explicit SplashScreen(const QPixmap& pixmap = {}, Qt::WindowFlags f = {});
  bool eventFilter(QObject* target, QEvent* event) override;
};

#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_SPLASH_SCREEN_HPP */
