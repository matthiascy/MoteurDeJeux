#ifndef MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_CAMERA_HPP
#define MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_CAMERA_HPP

#include <Core/Core.hpp>
#include <GameFramework/Component.hpp>

class GameObject;

class Camera : public Component {
protected:
  Real m_aspect {};
  Real m_z_near {};
  Real m_z_far  {};

public:
  Camera(String name, GameObject* gameObject, Real aspect, Real zNear, Real zFar);
  ~Camera() override = default;

  [[nodiscard]]
  virtual Mat4 viewMatrix() const;

  [[nodiscard]]
  virtual Mat4 projectionMatrix() const;

  [[nodiscard]]
  virtual Real aspect() const;
  virtual void setAspect(Real aspect);

  [[nodiscard]]
  virtual Real zNear() const;
  virtual void setZNear(Real z);

  [[nodiscard]]
  virtual Real zFar() const;
  virtual void setZFar(Real z);
};

#endif  /* !MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_CAMERA_HPP */
