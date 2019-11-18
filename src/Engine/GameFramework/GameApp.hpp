#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_GAME_APP_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_GAME_APP_HPP


#include <QtGui/QGuiApplication>
#include <Core/Core.hpp>

// QApplication : widgets
// QGuiApplication : opengl, quick

class Engine;

class GameApp {
private:
  QCoreApplication* m_app;
  Engine* m_engine;
  bool m_is_with_editor;

public:
  GameApp(const String& name, const String& description, int argc, char** argv);
  bool isEditorEnabled() const;
  virtual void run() { };
};


#endif /* !MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_GAME_APP_HPP */
