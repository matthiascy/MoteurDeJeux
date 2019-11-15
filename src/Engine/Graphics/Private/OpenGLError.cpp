#include "Engine/Graphics/OpenGLError.hpp"
#include <QCoreApplication>

QStack<QObject*> OpenGLError::m_error_handler;

inline OpenGLError::OpenGLError(const char* caller,
                                const char* fn,
                                OpenGLError::ObjectType objectType,
                                OpenGLError::FunctionType fnType)
  : QEvent(type()), m_function_name{fn}, m_caller_name{caller},
    m_object_type{objectType}, m_function_type{fnType}
{ }

inline const char* OpenGLError::functionName() const
{
  return m_function_name;
}

const char* OpenGLError::callerName() const
{
  return m_caller_name;
}

OpenGLError::ObjectType OpenGLError::objectType() const
{
  return m_object_type;
}

OpenGLError::FunctionType OpenGLError::functionType() const
{
  return m_function_type;
}

QEvent::Type OpenGLError::type()
{
  static auto custom_event_type = static_cast<QEvent::Type>(QEvent::registerEventType());
  return custom_event_type;
}

bool OpenGLError::sendEvent(OpenGLError* event)
{
  if (m_error_handler.empty()) {
    qWarning("Set OpenGLError::setErrorHandler() before calling any GL_DEBUG OpenGL functions!");
    return false;
  }
  return QCoreApplication::sendEvent(m_error_handler.top(), event);
}

void OpenGLError::pushErrorHandler(QObject* obj)
{
#ifdef DEBUG_GL
  qDebug() << "DEBUG_GL Error Handler (" << obj << ")";
#endif
  m_error_handler.push_back(obj);
}

void OpenGLError::popErrorHandler()
{
  m_error_handler.pop();
}
