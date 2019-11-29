#ifndef HOMELAND_TESTWINDOW_HPP
#define HOMELAND_TESTWINDOW_HPP

#include <QMainWindow>
#include <QPaintEvent>
#include <QPainter>
#include <Graphics/OglOffscreenSurface.hpp>

class TestWindow : public QMainWindow {
  Q_OBJECT

private:
  OglOffscreenSurface* surface;

public:
  explicit TestWindow(QWidget* parent = nullptr) : QMainWindow(parent) {

    qDebug() << "\t\t2. OffScreenSurface creation.";
    surface = new OglOffscreenSurface();
    //surface->setFormat(surface->context()->format());
    surface->create();
    surface->init();

    std::cout << surface->isValid() << std::endl;

    qDebug() << "THERE";
    if (!surface->isValid())
      qDebug() << "\t\t\tCreation failed";

    surface->resize({1024, 768});
  }

  ~TestWindow() override {
    delete surface;
  }

protected:
  void paintEvent(QPaintEvent*) override {
    surface->render();
    QImage image = surface->grabFramebuffer();
    QPainter p;
    p.begin(this);
    p.drawImage(rect(), image, image.rect());
    p.end();
  }
};

#endif  /* !HOMELAND_TESTWINDOW_HPP */
