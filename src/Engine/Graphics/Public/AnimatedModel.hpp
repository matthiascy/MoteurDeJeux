#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_ANIMATED_MODEL_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_ANIMATED_MODEL_HPP

#include "Animation.hpp"
#include "Forward.hpp"

class AnimatedModel {
private:
  SkeletonHandle          m_skeleton;
  ModelHandle             m_skin;
  Array<AnimationHandle>  m_animations;
  Mat4                    m_world_inverse_transform;

public:
  AnimatedModel(SkeletonHandle skeleton, ModelHandle skin, Array<AnimationHandle> animations);
  AnimatedModel();

  ~AnimatedModel() = default;

  [[nodiscard]]
  SkeletonHandle skeleton() const;

  void setSkeleton(SkeletonHandle skeleton);

  [[nodiscard]]
  ModelHandle skin() const;

  void setSkin(ModelHandle skin);

  void addAnimation(AnimationHandle animation);

  [[nodiscard]]
  const Array<AnimationHandle>& animations() const;

  [[nodiscard]]
  const Mat4& worldInverseTransform() const;

  void setWorldInverseTransform(const Mat4& transform);
};

/*
static Vec3 NodeAnimation_FindInterpolatedPosition(const NodeAnimation& nodeAnim, float animTime);
static Quat NodeAnimation_FindInterpolatedRotation(const NodeAnimation& nodeAnim, float animTime);

template <typename T>
static T NodeAnimation_FindInterpolatedValue(const Array<AnimKeyFrame<T>>& keys, float animTime);

template <typename T>
static UInt32 NodeAnimation_FindKeyIndex(const T& keys, float animTime);

extern const NodeAnimation* FindNodeAnim(const Animation& anim, const String& nodeName);
extern Vec3 InterpolateValue(const Vec3& start, const Vec3& end, float factor);
extern Quat InterpolateValue(const Quat& start, const Quat& end, float factor);
extern UInt32 Skeleton_FindBoneIndex(const Skeleton& skeleton, const String& name);
extern Bone* Skeleton_FindBone(Skeleton& skeleton, const String& name);

template <typename T>
T NodeAnimation_FindInterpolatedValue(Array<AnimKeyFrame<T>>& keys, float animTime)
{
  if (keys.size() == 1)
    return keys[0].value;

  UInt32 currKeyIndex = NodeAnimation_FindKeyIndex(keys, animTime);
  UInt32 nextKeyIndex = currKeyIndex + 1;
  float deltaTime = keys[nextKeyIndex].time - keys[currKeyIndex].time;
  float factor = (animTime - keys[currKeyIndex].time) / deltaTime;
  const T& start = keys[currKeyIndex].value;
  const T& end   = keys[nextKeyIndex].value;

  return InterpolateValue(start, end, factor);
}

template <typename T>
UInt32 NodeAnimation_FindKeyIndex(const T& keys, float animTime)
{
  for (auto i = 0; i < keys.size() - 1; ++i) {
    if (animTime < keys[i+1].time)
      return i;
  }
  return -1;
}
 */

#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_ANIMATED_MODEL_HPP */
