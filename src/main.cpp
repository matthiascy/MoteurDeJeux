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
#include "GLWidget.hpp"
#include "EngineViewport.hpp"
#include "mainwindow.h"
#include "editor/LevelEditorMainWindow.hpp"

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
  QApplication app(argc, argv);

  QCoreApplication::setApplicationName("MoteurDeJeux");

  QSurfaceFormat format;
  format.setDepthBufferSize(24);
  format.setSamples(16);
  format.setProfile(QSurfaceFormat::CoreProfile);
  QSurfaceFormat::setDefaultFormat(format);

  LevelEditorMainWindow mainWindow;
  //MainWindow mainWindow;
  mainWindow.resize(mainWindow.sizeHint());

  mainWindow.show();

  return QCoreApplication::exec();
}
