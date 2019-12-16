#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_PERSPECTIVE_CAMERA_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_PERSPECTIVE_CAMERA_HPP

#include "Camera.hpp"

class PerspectiveCamera : public Camera {
private:
  Real m_fov {};

public:
  PerspectiveCamera(const String& name, GameObject* gameObject,
                    Real fov, Real aspect, Real zNear, Real zFar);
  ~PerspectiveCamera() override = default;

  [[nodiscard]]
  UInt64 typeID() const override {
    return family::type<PerspectiveCamera>;
  }

  [[nodiscard]]
  Mat4 projectionMatrix() const override;

public:

  [[nodiscard]]
  Real fieldOfView() const;
  void setFieldOfView(Real fov);
};


#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_PERSPECTIVE_CAMERA_HPP */
