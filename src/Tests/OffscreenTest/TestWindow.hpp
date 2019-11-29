
#ifndef TEST_MAINWINDOW_H
#define TEST_MAINWINDOW_H

#include <QMainWindow>
#include <QPaintEvent>
#include <QPaintEngine>
#include <QtGui/QPainter>
#include "ExamplePaintSurface.hpp"
#include "Offscreen.hpp"
#include <Graphics/OglOffscreenSurface.hpp>

class TestMainWindow : public QMainWindow
{
Q_OBJECT

public:
  explicit TestMainWindow(QWidget *parent = 0) : QMainWindow{parent} {
    surface = new OglOffscreenSurface;
  }

  ~TestMainWindow() override { delete surface; };

  OglOffscreenSurface* surface;

protected:

  void paintEvent(QPaintEvent *) override {
    surface->resize(300, 200);
    surface->render();
    QImage image = surface->grabFramebuffer();

    QPainter* p = new QPainter();
    p->begin(this);
    p->drawImage(rect(), image, image.rect());
    p->end();
  }
};

#endif // MAINWINDOW_H
