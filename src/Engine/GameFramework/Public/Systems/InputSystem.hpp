#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_SYSTEMS_INPUT_SYSTEM_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_SYSTEMS_INPUT_SYSTEM_HPP

#include <Core/Public/Core.hpp>
#include <GameFramework/Public/ECS/System.hpp>
#include <QtGui/QWheelEvent>

class InputSystem : public System {

  friend class GameApp;

private:
  HashMap<Qt::Key, InputState> m_key_map;
  HashMap<Qt::MouseButton, InputState> m_btn_map;
  QPoint m_mouse_delta;
  QPoint m_mouse_prev_position;
  QPoint m_mouse_curr_position;
  QPoint m_mouse_wheel_angle_delta;
  bool m_is_mouse_wheel_scrolled;

public:
  InputSystem(const String& name, Engine* engine, Object* parent = nullptr);
  ~InputSystem() override;

  void init() override;

  void fixedUpdate(Real dt) override {
    _update();
  };

  void preUpdate(Real dt) override { };

  void update(Real dt) override {
  };

  void postUpdate(Real dt) override { };

public:
  InputState keyState(Qt::Key key);

  bool isKeyTriggered(Qt::Key key);

  bool isKeyPressed(Qt::Key key);

  bool isKeyReleased(Qt::Key key);

  InputState mouseButtonState(Qt::MouseButton button);

  bool isMouseButtonTriggered(Qt::MouseButton button);

  bool isMouseButtonPressed(Qt::MouseButton button);

  bool isMouseButtonReleased(Qt::MouseButton button);

  [[nodiscard]]
  QPoint mousePosition() const;

  [[nodiscard]]
  QPoint mouseDelta() const;

  [[nodiscard]]
  QPoint angleDelta() const;

  [[nodiscard]]
  Real verticalScroll() const;

  [[nodiscard]]
  Real horizontalScroll() const;

private:
  void _register_key_press(Qt::Key key);
  void _register_key_release(Qt::Key key);
  void _register_mouse_button_press(Qt::MouseButton button);
  void _register_mouse_button_release(Qt::MouseButton release);
  void _register_mouse_wheel(QWheelEvent* event);
  void _reset();
  void _update();
};

#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_SYSTEMS_INPUT_SYSTEM_HPP */
