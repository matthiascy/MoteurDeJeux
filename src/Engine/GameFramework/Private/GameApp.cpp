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
  QCoreApplication::setApplicationName(name);
  {
    QCommandLineParser parser;
    parser.setApplicationDescription(description);
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption withEditorOpt = {{"e", "with-editor"}, "Run with in-game editor."};
    parser.addOption(withEditorOpt);

    QCoreApplication appTmp(argc, argv);
    QCoreApplication::setApplicationVersion("0.1.0");
    parser.process(appTmp);
    m_is_with_editor = parser.isSet(withEditorOpt);
  }

  if (m_is_with_editor) {
    qDebug() << "Launching with editor : Qt Widget Application";
    m_app = new QApplication(argc, argv);
  } else {
    qDebug() << "Launching without editor : Qt OpenGL Application";
    m_app = new QGuiApplication(argc, argv);
  }

  {
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(4, 0);
    format.setDepthBufferSize(24);
    format.setSamples(16);
    QSurfaceFormat::setDefaultFormat(format);
  }

  //QPixmap engineLogo(":/App/engine-logo");
  //QSplashScreen splashScreen(engineLogo);
  //splashScreen.show();

  //QTimer::singleShot(1000, &splashScreen, &QSplashScreen::close);
  m_engine = new Engine(this);

  if (m_is_with_editor) {
    //QTimer::singleShot(1000, dynamic_cast<QMainWindow*>(m_engine->window()), &QMainWindow::show);
    dynamic_cast<QMainWindow*>(m_engine->window())->show();
  } else {
    //QTimer::singleShot(1000, dynamic_cast<QOpenGLWindow*>(m_engine->window()), &QOpenGLWindow::show);
    dynamic_cast<QOpenGLWindow*>(m_engine->window())->show();
  }

  QCoreApplication::exec();
}

bool GameApp::isEditorEnabled() const
{
  return m_is_with_editor;
}


