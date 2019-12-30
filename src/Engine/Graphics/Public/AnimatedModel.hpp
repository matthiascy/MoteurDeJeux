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

#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GRAPHICS_ANIMATED_MODEL_HPP */
