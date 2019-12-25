#include <GameFramework/Public/Systems/InputSystem.hpp>
#include <QCursor>

template <typename T>
static void _update_states(HashMap<T, InputState>& map)
{
  for (auto iter = map.begin(); iter != map.end(); ++iter) {
    switch (iter.value()) {
      case InputState::Registered: {
        iter.value() = InputState::Triggered;
      } break;

      case InputState::Triggered: {
        iter.value() = InputState::Pressed;
      } break;

      case InputState::Unregistered: {
        iter.value() = InputState::Released;
      } break;

      default:
        break;
    }
  }
}

template <typename T>
static void _remove_if_released(HashMap<T, InputState>& map)
{
  auto iter = map.begin();
  while (iter != map.end()) {
    if (iter.value() == InputState::Released)
      iter = map.erase(iter);
    else
      ++iter;
  }
}

InputSystem::InputSystem(const String& name, Engine* engine, Object* parent)
  : System(name, engine, parent), m_is_mouse_wheel_scrolled{false}
{
  qInfo() << "Creation =>" << objectName();
}

InputSystem::~InputSystem()
{
  qDebug() << "Shutting down...";
  qDebug() << "Shutting down...[Finished]";
}

void InputSystem::init()
{
  qInfo() << "Initialization...";
  // TODO
}

bool InputSystem::isKeyPressed(Qt::Key key)
{
  return keyState(key) == InputState::Pressed;
}

bool InputSystem::isKeyReleased(Qt::Key key)
{
  return keyState(key) == InputState::Released;
}

bool InputSystem::isKeyTriggered(Qt::Key key)
{
  return keyState(key) == InputState::Triggered;
}

bool InputSystem::isMouseButtonTriggered(Qt::MouseButton button)
{
  return mouseButtonState(button) == InputState::Triggered;
}

bool InputSystem::isMouseButtonPressed(Qt::MouseButton button)
{
  return mouseButtonState(button) == InputState::Pressed;
}

bool InputSystem::isMouseButtonReleased(Qt::MouseButton button)
{
  return mouseButtonState(button) == InputState::Released;
}

InputState InputSystem::keyState(Qt::Key key)
{
  auto it = m_key_map.find(key);
  if (it != m_key_map.end())
    return it.value();
  else
    return InputState::Invalid;
}

InputState InputSystem::mouseButtonState(Qt::MouseButton button)
{
  auto it = m_btn_map.find(button);
  if (it != m_btn_map.end())
    return it.value();
  else
    return InputState::Invalid;
}

QPoint InputSystem::mousePosition() const
{
  return QCursor::pos();
}

QPoint InputSystem::mouseDelta() const
{
  return m_mouse_delta;
}

void InputSystem::_update()
{
  m_mouse_prev_position = m_mouse_curr_position;
  m_mouse_curr_position = QCursor::pos();
  m_mouse_delta = m_mouse_curr_position - m_mouse_prev_position;
  if (m_is_mouse_wheel_scrolled) m_is_mouse_wheel_scrolled = !m_is_mouse_wheel_scrolled;
  m_mouse_wheel_angle_delta = {0, 0};

  _remove_if_released(m_key_map);
  _remove_if_released(m_btn_map);
  _update_states(m_key_map);
  _update_states(m_btn_map);

}

void InputSystem::_register_key_press(Qt::Key key)
{
  auto it = m_key_map.find(key);
  if (it == m_key_map.end())
    m_key_map.insert(key, InputState::Registered);
}

void InputSystem::_register_key_release(Qt::Key key)
{
  auto it = m_key_map.find(key);
  if (it != m_key_map.end())
    m_key_map[key] = InputState::Unregistered;
}

void InputSystem::_register_mouse_button_press(Qt::MouseButton button)
{
  auto it = m_btn_map.find(button);
  if (it == m_btn_map.end())
    m_btn_map.insert(button, InputState::Registered);
}

void InputSystem::_register_mouse_button_release(Qt::MouseButton button)
{
  auto it = m_btn_map.find(button);
  if (it != m_btn_map.end())
    m_btn_map[button] = InputState::Unregistered;
}

void InputSystem::_reset()
{
  m_key_map.clear();
  m_btn_map.clear();
}

QPoint InputSystem::angleDelta() const
{
  return m_mouse_wheel_angle_delta;
}

void InputSystem::_register_mouse_wheel(QWheelEvent* event)
{
  m_is_mouse_wheel_scrolled = true;
  m_mouse_wheel_angle_delta = event->angleDelta();
}

Real InputSystem::verticalScroll() const
{
   return m_mouse_wheel_angle_delta.y();
}

Real InputSystem::horizontalScroll() const
{
  return m_mouse_wheel_angle_delta.x();
}
