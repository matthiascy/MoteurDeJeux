#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QLabel>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QTimer>
#include <memory>
#include <QtCore/QTime>
#include "Types.hpp"
#include "Terrain.hpp"
#include "Camera.hpp"
#include "SceneNode.hpp"
#include "SceneManager.hpp"

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions {

    Q_OBJECT

public:
    explicit GLWidget(QWidget *parent = nullptr);
    ~GLWidget() override;

    friend class Window;

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    void rotateView();
    void cleanup();

signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void wheelEvent(QWheelEvent* event) override;

    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

private:
    //void setupVertexAttribs();
    void setupTextures();

    UniquePtr<SceneManager> m_scn_mgr;

    int m_xRot;
    int m_yRot;
    int m_zRot;
    QPoint m_lastPos;

    // Pour le rendu
    OpenGLVAO m_vao;
    // chaque vbo est lie a un object
    Array<OpenGLBuffer> m_vbos;
    Array<OpenGLBuffer*> m_ebos;

    OpenGLProgram* m_program;
    // Pointer sur le camera de scene manager
    Camera* m_camera;

    int m_projMatrixLoc;
    int m_mvMatrixLoc;
    int m_normalMatrixLoc;
    int m_lightPosLoc;
    QMatrix4x4 m_proj;
    QMatrix4x4 m_model;

  UniquePtr<QOpenGLTexture> m_grass_texture;
  UniquePtr<QOpenGLTexture> m_rock_texture;
  UniquePtr<QOpenGLTexture> m_snow_rock_texture;
  UniquePtr<QOpenGLTexture> m_ocean_texture;
  UniquePtr<QOpenGLTexture> m_earth_texture;
  UniquePtr<QOpenGLTexture> m_sun_texture;
  UniquePtr<QOpenGLTexture> m_moon_texture;
  UniquePtr<QOpenGLTexture> m_jupiter_texture;
  UniquePtr<QOpenGLTexture> m_milky_texture;

    std::unique_ptr<QTimer> m_timer;
    bool m_is_ctrl_pressed;

    QTime m_last_time;
    UInt32 m_fps;

    QLabel* m_fps_label;
    QTimer* m_fps_timer;

signals:
  void fpsChanged(UInt32 fps);
};

#endif
