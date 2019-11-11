#ifndef MOTEUR_DE_JEUX_SRC_INPUT_HPP
#define MOTEUR_DE_JEUX_SRC_INPUT_HPP

#include "Types.hpp"

class Input {
public:
  static InputState keyState(Qt::Key key);
  static bool isKeyTriggered(Qt::Key key);
  static bool isKeyPressed(Qt::Key key);
  static bool isKeyReleased(Qt::Key key);
  static InputState mouseButtonState(Qt::MouseButton button);
  static bool isMouseButtonTriggered(Qt::MouseButton button);
  static bool isMouseButtonPressed(Qt::MouseButton button);
  static bool isMouseButtonReleased(Qt::MouseButton button);
  static QPoint mousePosition();
  static QPoint mouseDelta();

private:
  static void update();
  static void register_key_press(Qt::Key key);
  static void register_key_release(Qt::Key key);
  static void register_mouse_button_press(Qt::MouseButton button);
  static void register_mouse_button_release(Qt::MouseButton button);
  static void reset();

  friend class Window;
};

#endif  /* !MOTEUR_DE_JEUX_SRC_INPUT_HPP */
