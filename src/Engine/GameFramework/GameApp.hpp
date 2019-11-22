#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_GAME_APP_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_GAME_APP_HPP


#include <QtGui/QGuiApplication>
#include <Core/Core.hpp>

/*
 * QApplication : widgets
 * QGuiApplication : opengl, quick
 */

class Engine;
class QSplashScreen;

class GameApp {
private:
  UniquePtr<QCoreApplication> m_app;
  UniquePtr<Engine>           m_engine;
  UniquePtr<QSplashScreen>    m_splash;

  bool m_is_with_editor;

public:
  GameApp(const String& name, const String& description, int argc, char** argv);
  virtual ~GameApp();
  [[nodiscard]] bool isEditorEnabled() const;
  [[nodiscard]] StringView name() const;

  virtual void run() { };
};


#endif /* !MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_GAME_APP_HPP */
