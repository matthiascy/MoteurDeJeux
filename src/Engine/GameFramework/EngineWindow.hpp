#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_ENGINE_WINDOW_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_ENGINE_WINDOW_HPP

#include <QtWidgets/QWidget>

class Engine;

class EngineWindow : public QWidget {
  Q_OBJECT

private:
  Engine* m_engine;
  QImage  m_image;


protected:
  void paintEvent(QPaintEvent*) override;

public:
  explicit EngineWindow(Engine* engine, QWidget* parent = nullptr);
};


#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_ENGINE_WINDOW_HPP */
