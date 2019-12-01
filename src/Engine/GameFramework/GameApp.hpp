#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_GAME_APP_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_GAME_APP_HPP

#include <QtGui/QGuiApplication>
#include <Core/Core.hpp>

class Engine;
class SplashScreen;

class GameApp {
protected:
  UniquePtr<QCoreApplication> m_app;
  UniquePtr<Engine>           m_engine;
  UniquePtr<SplashScreen>     m_splash;

  bool m_is_with_editor;

public:
  GameApp(const String& name, const String& description, int argc, char** argv);
  virtual ~GameApp();
  [[nodiscard]] bool isEditorEnabled() const;
  [[nodiscard]] StringView name() const;

  virtual void run() { };
};


#endif /* !MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_GAME_APP_HPP */
