#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_PERSPECTIVE_CAMERA_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_PERSPECTIVE_CAMERA_HPP

#include "Camera.hpp"

class PerspectiveCamera : Camera<PerspectiveCamera> {
private:
  Real m_fov {};

public:
  PerspectiveCamera(const String& name, GameObject* gameObject,
                    float fov, float aspect, float zNear, float zFar);
  ~PerspectiveCamera() override = default;

  [[nodiscard]]
  Mat4 projectionMatrix() const override;

public:

  [[nodiscard]]
  Real fieldOfView() const;
  void setFieldOfView(Real fov);
};


#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_PERSPECTIVE_CAMERA_HPP */
