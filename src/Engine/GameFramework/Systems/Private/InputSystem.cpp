#include <GameFramework/Systems/InputSystem.hpp>
#include <QCursor>

InputSystem::InputSystem(const String& name, Engine* engine, Object* parent)
  : System(name, engine, parent)
{
  qInfo() << "Input System creation  =>" << objectName();
}

InputSystem::~InputSystem()
{
  qDebug() << "Shut down InputSystem...";
  qDebug() << "Shut down InputSystem... [Done]";
  // TODO
}

void InputSystem::init()
{
  qInfo() << "- Input system initialization...";
  // TODO
}

/*
template <typename T>
struct InputInstance : std::pair<T, InputState> {
  typedef std::pair<T, InputState> base_class;
  inline explicit InputInstance(T value) : base_class(value, InputState::Invalid) { }
  inline InputInstance(T value, InputState state) : base_class(value, state) { }
  inline bool operator==(const InputInstance& rhs) const {
    return this->first == rhs.first;
  }
};

typedef InputInstance<Qt::Key> KeyInstance;
typedef InputInstance<Qt::MouseButton> MouseButtonInstance;

typedef std::vector<KeyInstance> KeyContainer;
typedef std::vector<MouseButtonInstance> MouseButtonContainer;

// Globals
static KeyContainer gKeyInstances;
static MouseButtonContainer gMouseButtonInstances;
static QPoint gMouseCurrPosition;
static QPoint gMousePrevPosition;
static QPoint gMouseDelta;

static inline KeyContainer::iterator _find_key(Qt::Key value)
{
  return std::find(gKeyInstances.begin(), gKeyInstances.end(), value);
}

static inline MouseButtonContainer::iterator _find_button(Qt::MouseButton value)
{
  return std::find(gMouseButtonInstances.begin(), gMouseButtonInstances.end(), value);
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
  auto it = _find_key(key);
  return (it != gKeyInstances.end()) ? it->second : InputState::Invalid;
}

inline bool InputSystem::isKeyTriggered(Qt::Key key)
{
  return keyState(key) == InputState::Triggered;
}

inline bool InputSystem::isKeyPressed(Qt::Key key)
{
  return keyState(key) == InputState::Pressed;
}

InputState InputSystem::mouseButtonState(Qt::MouseButton button)
{
  auto it = _find_button(button);
  return (it != gMouseButtonInstances.end()) ? it->second : InputState::Invalid;
}

inline bool InputSystem::isMouseButtonTriggered(Qt::MouseButton button)
{
  return mouseButtonState(button) == InputState::Triggered;
}

inline bool InputSystem::isMouseButtonPressed(Qt::MouseButton button)
{
  return mouseButtonState(button) == InputState::Pressed;
}

inline bool InputSystem::isMouseButtonReleased(Qt::MouseButton button)
{
  return mouseButtonState(button) == InputState::Released;
}

QPoint InputSystem::mousePosition()
{
  return QCursor::pos();
}

QPoint InputSystem::mouseDelta()
{
  return gMouseDelta;
}

void InputSystem::update()
{
  gMousePrevPosition = gMouseCurrPosition;
  gMouseCurrPosition = QCursor::pos();
  gMouseDelta = gMouseCurrPosition - gMousePrevPosition;
  _update(gMouseButtonInstances);
  _update(gKeyInstances);
}

void InputSystem::register_key_press(Qt::Key key)
{
  auto it = _find_key(key);
  if (it == gKeyInstances.end())
    gKeyInstances.push_back(KeyInstance(key, InputState::Registered));
}

void InputSystem::register_key_release(Qt::Key key)
{
  auto it = _find_key(key);
  if (it != gKeyInstances.end())
    it->second = InputState::Unregistered;
}

void InputSystem::register_mouse_button_press(Qt::MouseButton button)
{
  auto it = _find_button(button);
  if (it == gMouseButtonInstances.end())
    gMouseButtonInstances.push_back(MouseButtonInstance(button, InputState::Registered));
}

*/