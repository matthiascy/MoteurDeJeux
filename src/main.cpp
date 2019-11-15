#include <QApplication>
#include <QDesktopWidget>
#include <QSurfaceFormat>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtGui/QScreen>
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlComponent>
#include <QtQuick/QQuickWindow>
#include <QtCore/QUrl>
#include <QSplashScreen>
#include <QThread>
#include "GLWidget.hpp"
#include "EngineViewport.hpp"
#include "mainwindow.h"
#include "Editor/LevelEditorMainWindow.hpp"

#include <Engine/GameFramework/GameApp.hpp>

int main(int argc, char *argv[])
{
  /*
  qmlRegisterType<EngineViewport>("Engine", 1, 0, "Viewport");
  QGuiApplication app(argc, argv);
  QGuiApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
  QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  QSurfaceFormat format;
  format.setDepthBufferSize(24);
  format.setSamples(16);
  format.setProfile(QSurfaceFormat::CoreProfile);
  QSurfaceFormat::setDefaultFormat(format);

  QQmlApplicationEngine engine;
  QQuickWindow::setDefaultAlphaBuffer(true);
  engine.load(QUrl("qrc:/Qml/main"));
   */

  /*
  QApplication app(argc, argv);
  QApplication::setApplicationName("MoteurDeJeux");

  QSurfaceFormat format;
  format.setRenderableType(QSurfaceFormat::OpenGL);
  format.setProfile(QSurfaceFormat::CoreProfile);
  format.setVersion(4, 0);
  format.setDepthBufferSize(24);
  format.setSamples(16);
  QSurfaceFormat::setDefaultFormat(format);

#ifdef DEBUG_GL
  format.setOption(QSurfaceFormat::DebugContext);
#endif

  QPixmap engineLogo(":/App/engine-logo");
  QSplashScreen splash(engineLogo);
  splash.show();

  //LevelEditorMainWindow mainWindow;
  MainWindow mainWindow;
  mainWindow.resize(mainWindow.sizeHint());

  QTimer::singleShot(1000, &splash, &QSplashScreen::close);
  QTimer::singleShot(1000, &mainWindow, &MainWindow::show);

  return QApplication::exec();
   */
}
