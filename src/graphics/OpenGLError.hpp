#ifndef MOTEUR_DE_JEUX_SRC_GRAPHICS_OPENGL_ERROR_HPP
#define MOTEUR_DE_JEUX_SRC_GRAPHICS_OPENGL_ERROR_HPP

#include <QtCore/QEvent>
#include <QtCore/QStack>

class OpenGLError : public QEvent {
public:
  enum class FunctionType {
    Bind,
    Read,
    Create,
    Unmap,
    Link,
    AddShader,
    AddShaderFromSourceCode,
    AddShaderFromSourceFile
  };

  enum class ObjectType {
    OpenGLBuffer,
    OpenGLShaderProgram,
    OpenGLVAO
  };

  OpenGLError(const char* caller, const char* fn, ObjectType objectType, FunctionType fnType);
  ~OpenGLError() override = default;

  const char* functionName() const;
  const char* callerName() const;
  ObjectType objectType() const;
  FunctionType functionType() const;

  static QEvent::Type type();
  static bool sendEvent(OpenGLError* event);
  static void pushErrorHandler(QObject* obj);
  static void popErrorHandler();

private:
  const char*  m_function_name;
  const char*  m_caller_name;
  ObjectType   m_object_type;
  FunctionType m_function_type;
  static QStack<QObject*> m_error_handler;
};

#endif  /* !MOTEUR_DE_JEUX_SRC_GRAPHICS_OPENGL_ERROR_HPP */
