#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_SYSTEMS_INPUT_SYSTEM_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_SYSTEMS_INPUT_SYSTEM_HPP

#include <Core/Core.hpp>
#include <GameFramework/System.hpp>

class InputSystem : public System {

  friend class EngineWindow;

private:
  HashMap<Qt::Key, InputState> m_key_map;
  HashMap<Qt::MouseButton, InputState> m_mouse_button_map;
  QPoint m_mouse_delta;
  QPoint m_mouse_prev_position;
  QPoint m_mouse_curr_position;

public:
  InputSystem(const String& name, Engine* engine, Object* parent = nullptr);
  ~InputSystem() override;

  void init() override;

  void fixedUpdate(Real dt) override { };

  void preUpdate(Real dt) override { };

  void update(Real dt) override { };

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

private:
  void _register_key_press(Qt::Key key);
  void _register_key_release(Qt::Key key);
  void _register_mouse_button_press(Qt::MouseButton button);
  void _register_mouse_button_release(Qt::MouseButton release);
  void _reset();
  void _update();
};

#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_SYSTEMS_INPUT_SYSTEM_HPP */
