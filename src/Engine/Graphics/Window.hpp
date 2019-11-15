#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_WINDOW_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_WINDOW_HPP

class Window {
public:
  virtual ~Window() = default;
  virtual void startShowing() = 0;
};

#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_WINDOW_HPP */
