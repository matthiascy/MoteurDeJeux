#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_ANIMATION_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_ANIMATION_HPP

#include <Core/Public/Core.hpp>

struct VertexWeight {
  UInt32 vertexIdx;
  Real weight;
};

struct Bone {
  String name;
  Mat4 nodeTransform;
  Mat4 offsetMatrix;  // T-Pose to local bone space
  Mat4 finalTransform;
  Array<VertexWeight> weights;
  Array<UInt32> children;
};

using Joint = Bone;

struct Skeleton {
  Array<Bone> bones;
};

template <typename T>
struct AnimKeyFrame {
  T value;
  float time;
};

struct BoneAnimation {
  String name;
  Array<AnimKeyFrame<Vec3>> positionKeys;
  Array<AnimKeyFrame<Quat>> rotationKeys;
  Array<AnimKeyFrame<Vec3>> scalingKeys;
};

struct Animation {
  Array<BoneAnimation> boneAnimations;
  Real ticksPerSecond;
  Real duration;
};


#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_ANIMATION_HPP */
