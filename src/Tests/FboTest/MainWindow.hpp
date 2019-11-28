#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

#include <QOpenGLContext>
#include <QOpenGLFramebufferObject>
#include <QOffscreenSurface>
#include <QOpenGLFunctions>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow() override;

private:
  Ui::MainWindow *ui;
  QOpenGLFramebufferObject* m_fbo;
  QOpenGLContext* m_context;
  QOffscreenSurface* m_surface;
  QSize m_size;
};
#endif // MAINWINDOW_HPP
