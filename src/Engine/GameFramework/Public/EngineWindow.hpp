#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_ENGINE_WINDOW_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_ENGINE_WINDOW_HPP

#include <QtWidgets/QWidget>
#include <Core/Public/Core.hpp>

class GameApp;
class QLCDNumber;
class QPushButton;

class EngineWindow : public QWidget {

  Q_OBJECT

private:
  GameApp* m_app;
  QImage   m_image;
  //QWidget* m_ui;
  UniquePtr<QLCDNumber>  ui_fps;
  UniquePtr<QPushButton> ui_exit_btn;

protected:
  void paintEvent(QPaintEvent*) override;

  void closeEvent(QCloseEvent*) override;

  void resizeEvent(QResizeEvent*) override;

public:
  explicit EngineWindow(GameApp* gameApp, QWidget* parent = nullptr);
  ~EngineWindow() override;

  [[nodiscard]]
  QLCDNumber* fpsWidget() const;

signals:
  void windowResized(QSize size);
};


#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_ENGINE_WINDOW_HPP */
