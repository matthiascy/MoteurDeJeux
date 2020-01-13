#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_ANIMATOR_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_ANIMATOR_HPP

#include <GameFramework/Public/ECS/Component.hpp>

class Animation;
class Skeleton;
class AssetManager;

/**
 * Controls which animation to be read.
 * Updates the animation time and determines the current pose,
 * before setting the joint/bone transforms in the `AnimatedModel'
 * Interpolate animation.
 */
class Animator : public Component {
private:
  Int32     m_current_anim;
  Skeleton*  m_skeleton;
  Real       m_animation_time;
  Array<Animation*> m_animations;
  AnimatedModel* m_animated_model;
  bool m_is_initialized;
  Array<Mat4> m_bone_transforms;
  AnimatedMeshRenderer* m_animated_mesh_renderer;
  bool m_is_registered_in_system;
  bool m_is_stopped;

public:
  [[nodiscard]]
  UInt64 typeID() const override;

public:
  Animator(const String& name, GameObject* gameObject);
  ~Animator() override = default;

  void init(AssetManager* assetManager);

  void runNextAnimation();

  void runAnimation(const String& name);

  void update(Real dt);

  [[nodiscard]]
  const Array<Mat4>& boneTransforms() const;

  [[nodiscard]]
  bool isInitialized() const;

  [[nodiscard]]
  bool isRegisteredInAnimationSystem() const;
  void registerInAnimationSystem();

  void stopAnimation();
  void startAnimation();
  bool isAnimationStopped();

private:
  void _update_skeleton_recursively(Real animTime, const Animation* animation, const Skeleton* skeleton, Bone* bone, const Mat4& parentTransform);

  Vec3 _interpolate_translation(const Array<AnimKeyFrame<Vec3>>& keyFrames, Real animTime);
  Quat _interpolate_rotation(const Array<AnimKeyFrame<Quat>>& keyFrames, Real animTime);
  Vec3 _interpolate_scaling(const Array<AnimKeyFrame<Vec3>>& keyFrames, Real animTime);

  template <typename T>
  Int32 _locate_animation_key_frame(const T& keyFrames, float animTime);
};

template <typename T>
Int32 Animator::_locate_animation_key_frame(const T& keyFrames, float animTime)
{

  for (auto i = 0; i < keyFrames.size() - 1; ++i) {
    if (animTime < keyFrames[i + 1].time)
      return i;
  }

  // TODO:
  return 0;
}


#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_ANIMATOR_HPP */
