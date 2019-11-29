#include <QApplication>
#include <Engine/GameFramework/Engine.hpp>
#include "TestWindow.hpp"

int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  //TestMainWindow w;
  Engine engine;
  engine.window().show();

  return QApplication::exec();
}

