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

int main(int argc, char *argv[])
{
  /*
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("MoteurDeJeux");
    QCoreApplication::setOrganizationName("");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::applicationName());
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption multipleSampleOption("multisample", "Multisampling");
    parser.addOption(multipleSampleOption);
    QCommandLineOption coreProfileOption("coreprofile", "Use core profile");
    parser.addOption(coreProfileOption);

    parser.process(app);

    QSurfaceFormat fmt;
    fmt.setDepthBufferSize(24);

    if (parser.isSet(multipleSampleOption))
        fmt.setSamples(4);

    if (parser.isSet(coreProfileOption)) {
        fmt.setVersion(3, 2);
        fmt.setProfile(QSurfaceFormat::CoreProfile);
    }

    // set core forcely
    fmt.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(fmt);

    MainWindow mainWindow;
    mainWindow.resize(mainWindow.sizeHint());
    int desktopArea = QApplication::desktop()->width() *
                     QApplication::desktop()->height();
    int widgetArea = mainWindow.width() * mainWindow.height();
    if (((float)widgetArea / (float)desktopArea) < 0.75f)
        mainWindow.show();
    else
        mainWindow.showMaximized();
  return app.exec();
  */
  {
    QSurfaceFormat fmt;
    fmt.setDepthBufferSize(24);
    fmt.setSamples(16);
    fmt.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(fmt);
  }

  QGuiApplication app(argc, argv);
  QGuiApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
  QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  const auto screens = QGuiApplication::screens();
  for (QScreen* screen : screens) {
    screen->setOrientationUpdateMask(Qt::LandscapeOrientation
                                     | Qt::PortraitOrientation
                                     | Qt::InvertedLandscapeOrientation
                                     | Qt::InvertedPortraitOrientation);
  }

  QQmlApplicationEngine engine;
  qmlRegisterType<EngineViewport>("Engine", 1, 0, "Viewport");
  QQuickWindow::setDefaultAlphaBuffer(true);
  engine.load(QUrl("qrc:/Qml/main"));
  if (engine.rootObjects().isEmpty())
    return -1;

  return QGuiApplication::exec();
}
