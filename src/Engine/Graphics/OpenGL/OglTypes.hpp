#ifndef MOTEUR_DE_JEUX_SRC_GRAPHICS_OGL_TYPES_HPP
#define MOTEUR_DE_JEUX_SRC_GRAPHICS_OGL_TYPES_HPP

#include <QOpenGLBuffer>
#include <QOpenGLShader>
#include <QOpenGLTexture>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLPaintDevice>
#include <QOpenGLDebugLogger>
#include <QOpenGLDebugMessage>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFramebufferObject>
#include <QOpenGLFunctions_4_0_Core>
#include <QOpenGLFramebufferObjectFormat>

typedef QOpenGLBuffer                  OglBuffer;
typedef QOpenGLShader                  OglShader;
typedef QOpenGLContext                 OglContext;
typedef QOpenGLTexture                 OglTexture;
typedef QOpenGLFunctions               OglFns;
typedef QOpenGLPaintDevice             OglPaintDevice;
typedef QOpenGLShaderProgram           OglProgram;
typedef QOpenGLFramebufferObject       OglFBO;
typedef QOpenGLVertexArrayObject       OglVAO;
typedef QOpenGLFunctions_4_0_Core      OglFnsCore4_0;
typedef QOpenGLFramebufferObjectFormat OglFBOFormat;
typedef QOpenGLDebugMessage            OglDebugMessage;
typedef QOpenGLDebugLogger             OglDebugLogger;

#endif  /* !MOTEUR_DE_JEUX_SRC_GRAPHICS_OGL_TYPES_HPP */
