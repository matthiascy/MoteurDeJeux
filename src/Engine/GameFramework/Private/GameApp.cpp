#include <GameFramework/GameApp.hpp>
#include <GameFramework/Engine.hpp>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>
#include <QtGui/QOpenGLWindow>
#include <QtGui/QGuiApplication>
#include <QApplication>
#include <Editor/EditorMainWindow.hpp>
#include <QtWidgets/QSplashScreen>
#include <QtCore/QTimer>

GameApp::GameApp(const String& name, const String& description, int argc, char** argv)
{
  qDebug() << "GameApp creation :";
  Q_INIT_RESOURCE(resources);

  QCoreApplication::setApplicationName(name);

  m_app = makeUnique<QApplication>(argc, argv);

  {
    QCommandLineParser parser;
    parser.setApplicationDescription(description);
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption withEditorOpt = {{"e", "with-editor"}, "Run with in-game editor."};
    parser.addOption(withEditorOpt);

    parser.process(*m_app);
    m_is_with_editor = parser.isSet(withEditorOpt);
  }

  if (m_is_with_editor) {
    qDebug() << "\twith editor (Widget Application)";
  } else {
    qDebug() << "\twithout editor (OpenGL Application)";
  }

  m_engine = makeUnique<Engine>(this);
  m_engine->init();

  m_splash = makeUnique<QSplashScreen>(QPixmap{":/App/engine-logo"}, Qt::WindowStaysOnTopHint);
  m_splash->show();
  QTimer::singleShot(2000, m_splash.get(), &QSplashScreen::close);
  QTimer::singleShot(1000, dynamic_cast<QWidget*>(m_engine->window()), &QWidget::show);

  QCoreApplication::exec();
}

GameApp::~GameApp()
{
  m_splash.reset(nullptr);
  m_engine.reset(nullptr);
  m_app.reset(nullptr);
}

bool GameApp::isEditorEnabled() const
{
  return m_is_with_editor;
}

StringView GameApp::name() const
{
  return QCoreApplication::applicationName();
}


