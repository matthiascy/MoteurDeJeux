#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_ORTHOGONAL_CAMERA_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_ORTHOGONAL_CAMERA_HPP

#include "Camera.hpp"

class OrthogonalCamera : Camera<OrthogonalCamera> {
private:
  Real m_scale  {};

public:
  OrthogonalCamera(const String& name, GameObject* gameObject, Real aspect, Real scale, Real zNear, Real zFar);
  ~OrthogonalCamera() override = default;

  [[nodiscard]]
  Mat4 projectionMatrix() const override;
};


#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_ORTHOGONAL_CAMERA_HPP */
