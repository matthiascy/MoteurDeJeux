#include <GameFramework/Systems/InputSystem.hpp>
#include <QCursor>

InputSystem::InputSystem(const String& name, Engine* engine, Object* parent)
  : System(name, engine, parent)
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

inline bool InputSystem::isKeyPressed(Qt::Key key)
{
  return keyState(key) == InputState::Pressed;
}

inline bool InputSystem::isKeyReleased(Qt::Key key)
{
  return keyState(key) == InputState::Released;
}

inline bool InputSystem::isKeyTriggered(Qt::Key key)
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

template <typename TPair>
static inline void _update_states(TPair& instance)
{
  switch (instance.second) {
    case InputState::Registered: {
      instance.second = InputState::Triggered;
    } break;

    case InputState::Triggered: {
      instance.second = InputState::Pressed;
    } break;

    case InputState::Unregistered: {
      instance.second = InputState::Released;
    } break;

    default:
      break;
  }
}

template <typename TPair>
static inline bool _check_released(const TPair& instance)
{
  return instance.second == InputState::Released;
}

template <typename Container>
static inline void _update(Container& container)
{
  typedef typename Container::iterator Iter;
  typedef typename Container::value_type TPair;
  // remove old data
  Iter remove = std::remove_if(container.begin(), container.end(), &_check_released<TPair>);
  container.erase(remove, container.end());
  // update existing data
  std::for_each(container.begin(), container.end(), &_update_states<TPair>);
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
  auto it = m_mouse_button_map.find(button);
  if (it != m_mouse_button_map.end())
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
  auto it = m_mouse_button_map.find(button);
  if (it == m_mouse_button_map.end())
    m_mouse_button_map.insert(button, InputState::Registered);
}

void InputSystem::_register_mouse_button_release(Qt::MouseButton button)
{
  auto it = m_mouse_button_map.find(button);
  if (it != m_mouse_button_map.end())
    m_mouse_button_map[button] = InputState::Unregistered;
}

void InputSystem::_reset()
{
  m_key_map.clear();
  m_mouse_button_map.clear();
}