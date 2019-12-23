#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <Core/Public/Core.hpp>

#define DEFAULT_NEAR 0.25f
#define DEFAULT_FAR  1024.0f
#define DEFAULT_FOV  (75.0f * M_PI / 180.0f)

const float kYaw         = -90.0f;
const float kPitch       = -45.0f;
const float kSpeed       = 2.5f;
const float kSensitivity = 0.1f;
const float kZoom        = 45.0f;

class EditorCamera {
public:
  explicit EditorCamera(const String& name);
  EditorCamera(Vec3 pos = {0.0f, 0.0f, 0.0f}, Vec3 up = {0.0f, 1.0f, 0.0f}, float yaw = kYaw, float pitch = kPitch);
  ~EditorCamera() = default;

public:
  QMatrix4x4 ViewMatrix();
  void SetYaw(float yaw);
  void SetPitch(float pitch);

public:
  Vec3 pos;
  Vec3 up;
  Vec3 front;
  Vec3 right;
  Vec3 worldUp;

  float yaw;
  float pitch;
  float zoom;

private:
  void update_camera_vectors();

};

#endif  // !CAMERA_HPP

