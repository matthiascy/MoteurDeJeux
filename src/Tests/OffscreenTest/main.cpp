#include <QApplication>
#include "TestWindow.hpp"

int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  TestWindow w;
  w.show();

  return QApplication::exec();
}

