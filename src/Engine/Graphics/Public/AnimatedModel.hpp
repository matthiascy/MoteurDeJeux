#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_ANIMATED_MODEL_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_ANIMATED_MODEL_HPP

#include "Animation.hpp"
#include "AnimatedMesh.hpp"

class AnimatedModel {
private:
  Skeleton            m_skeleton;
  Animation           m_animation;
  Array<AnimatedMesh> m_meshes;
  /*
  Mat4                m_global_inverse_transform;
  Real                m_animation_time;
   */

public:
  AnimatedModel();
  ~AnimatedModel();

  //void update(float dt);

  /*
  [[nodiscard]]
  UInt32 meshesNum() const { return m_meshes.size(); }

  [[nodiscard]]
  const AnimatedMesh& animatedMesh(UInt32 i) const { return m_meshes[i]; }
  void addAnimatedMesh(const AnimatedMesh& mesh) { m_meshes.push_back(mesh); }

  void setGlobalInverseTransform(const Mat4& transform) { m_global_inverse_transform = transform; }
  [[nodiscard]]
  const Mat4& globalInverseTransform() const { return m_global_inverse_transform; }

  [[nodiscard]]
  const Skeleton& skeleton() const { return m_skeleton; }
  Skeleton& skeleton() { return m_skeleton; }

  [[nodiscard]]
  const Animation& animation() const { return m_animation; }

  Animation& animation() { return m_animation; }

  void clear();
   */

private:
  void _read_node_hierarchy(Real animationTime, Animation& animation, Skeleton& skeleton, Bone& bone, const Mat4& parentTransform);
  void _transform_vertices(const Skeleton& skeleton);
};

static Vec3 NodeAnimation_FindInterpolatedPosition(const BoneAnimation& nodeAnim, float animTime);
static Quat NodeAnimation_FindInterpolatedRotation(const BoneAnimation& nodeAnim, float animTime);

template <typename T>
static T NodeAnimation_FindInterpolatedValue(const Array<AnimKeyFrame<T>>& keys, float animTime);

template <typename T>
static UInt32 NodeAnimation_FindKeyIndex(const T& keys, float animTime);

extern const BoneAnimation* FindNodeAnim(const Animation& anim, const String& nodeName);
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

#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_ANIMATED_MODEL_HPP */
