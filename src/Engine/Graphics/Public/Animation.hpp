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

  Mat4 offSetMatrix;
  //Mat4 inverseLocalTransform;

  Mat4 animatedTransform;

  Array<VertexWeight> weights;

  Array<UInt32> children;
};

using Joint = Bone;

class Skeleton {
private:
  UniquePtr<Array<Bone*>> m_bones;

public:
  Skeleton();
  ~Skeleton();

  [[nodiscard]]
  const Array<Bone*>& bones() const;

  Array<Bone*>& bones();

  [[nodiscard]]
  Bone* boneAt(UInt32 idx) const;

  [[nodiscard]]
  Bone* bone(const String& name) const;
};

template <typename T>
struct AnimKeyFrame {
  T value;
  float time;
};

struct NodeAnimation {
  String name;
  Array<AnimKeyFrame<Vec3>> positionKeys;
  Array<AnimKeyFrame<Quat>> rotationKeys;
  Array<AnimKeyFrame<Vec3>> scalingKeys;
};

struct Animation {
  String name;
  Array<NodeAnimation> nodeAnimations;
  Real ticksPerSecond;
  Real duration;
};


#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_ANIMATION_HPP */
