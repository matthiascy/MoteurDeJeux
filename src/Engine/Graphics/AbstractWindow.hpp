#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_ABSTRACT_WINDOW_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_ABSTRACT_WINDOW_HPP

class Engine;
class QLCDNumber;

class AbstractWindow {

public:
  virtual void startShowing() = 0;
  virtual ~AbstractWindow() = default;
};

#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_ABSTRACT_WINDOW_HPP */
