#include <Core/Public/Core.hpp>
#include "EditorCamera.hpp"


EditorCamera::EditorCamera(const String& name)
  : pos{Math::Zero}, up{0.0f, 1.0f, 0.0f}, worldUp{up}, yaw{kYaw}, pitch{kPitch},
    zoom{kZoom}
{
  update_camera_vectors();
}

EditorCamera::EditorCamera(Vec3 pos, Vec3 up, float yaw, float pitch)
  : pos{pos}, up{up}, worldUp{up}, yaw{yaw}, pitch{pitch},
    zoom{kZoom}
{
  update_camera_vectors();
}

Mat4 EditorCamera::ViewMatrix()
{
  update_camera_vectors();

  Mat4 out;
  out.lookAt(pos, pos + front, up);

  return out;
}

void EditorCamera::update_camera_vectors()
{
  Vec3 new_front;
  new_front.setX(std::cos(qDegreesToRadians(yaw)) * std::cos(qDegreesToRadians(pitch)));
  new_front.setY(std::sin(qDegreesToRadians(pitch)));
  new_front.setZ(std::sin(qDegreesToRadians(yaw)) * std::cos(qDegreesToRadians(pitch)));
  front = new_front.normalized();
  right = Vec3::crossProduct(front, worldUp).normalized();
  up    = Vec3::crossProduct(right, front).normalized();
}

void EditorCamera::SetYaw(float yaw)
{
  this->yaw = yaw;
  update_camera_vectors();
}

void EditorCamera::SetPitch(float pitch)
{
  this->pitch = pitch;
  update_camera_vectors();
}
