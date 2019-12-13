#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_GAME_APP_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_GAME_APP_HPP

#include <QtGui/QGuiApplication>
#include <Core/Core.hpp>

class Engine;
class SplashScreen;

class GameApp : public QObject {

  Q_OBJECT

protected:
  UniquePtr<QCoreApplication> m_app;
  UniquePtr<QWidget>          m_window;
  UniquePtr<Engine>           m_engine;
  UniquePtr<SplashScreen>     m_splash;

  bool m_is_with_editor;
  bool m_is_initialized;

  bool   m_is_quit;
  double m_fps;
  UInt64 m_frames;
  UInt64 m_start_time;
  QElapsedTimer m_elapsed_timer;
  QSize  m_win_min_size;

public:
  GameApp(const String& name, const String& description, QSize&& minSize, int argc, char** argv);
  ~GameApp() override;

  [[nodiscard]]
  bool isEditorEnabled() const;

  [[nodiscard]]
  StringView name() const;

  [[nodiscard]]
  inline QWidget* window() const { return m_window.get(); }

  [[nodiscard]]
  inline Engine* engine() const { return m_engine.get(); }

  [[nodiscard]]
  inline double fps() const { return m_fps; }

  void setMinimumSize(QSize&& size);

  void run();

  void quit();

  virtual void onUpdate() { };

public:
  bool eventFilter(QObject* object, QEvent* event) override;

signals:
  void fpsChanged(double fps);
};


#endif /* !MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_GAME_APP_HPP */
