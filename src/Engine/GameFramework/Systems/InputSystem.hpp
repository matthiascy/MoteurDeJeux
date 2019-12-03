#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_SYSTEMS_INPUT_SYSTEM_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_SYSTEMS_INPUT_SYSTEM_HPP

#include <Core/Core.hpp>
#include <GameFramework/System.hpp>

class InputSystem : public System {
public:
  InputSystem(const String& name, Engine* engine, Object* parent = nullptr);
  ~InputSystem() override;

  void init() override;

  void fixedUpdate(Real dt) override { };

  void preUpdate(Real dt) override { };

  void update(Real dt) override { };

  void postUpdate(Real dt) override { };
  /*
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
   */
};

#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_SYSTEMS_INPUT_SYSTEM_HPP */
