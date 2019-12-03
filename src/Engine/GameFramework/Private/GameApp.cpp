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
#include <Graphics/SplashScreen.hpp>

GameApp::GameApp(const String& name, const String& description, int argc, char** argv)
{
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
    qDebug() << "GameApp creation : running with editor.";
  } else {
    qDebug() << "GameApp creation : running without editor.";
  }

  m_engine = makeUnique<Engine>(this);
  m_engine->init();

  m_engine->window()->showMaximized();
  //m_engine->window()->showFullscreen();
  m_engine->window()->hide();

  m_splash = makeUnique<SplashScreen>(QPixmap{":/App/loading-engine"}, Qt::WindowStaysOnTopHint);
  m_splash->showFullScreen();
  QTimer::singleShot(3000, m_splash.get(), [this](){
    m_splash->setPixmap(QPixmap{":/App/loading-game"});
    QTimer::singleShot(3000, m_splash.get(), &QSplashScreen::close);
  });
  QTimer::singleShot(5000, dynamic_cast<QWidget*>(m_engine->window()), &QWidget::showMaximized);


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


