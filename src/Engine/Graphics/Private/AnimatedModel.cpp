#include "Graphics/Public/AnimatedModel.hpp"

AnimatedModel::AnimatedModel(SkeletonHandle skeleton, ModelHandle skin, Array<AnimationHandle> animations)
  : m_skeleton{skeleton}, m_skin{skin}, m_animations{std::move(animations)}
{ }

AnimatedModel::AnimatedModel()
  : m_skeleton{-1}, m_skin{-1}, m_animations{}
{ }

SkeletonHandle AnimatedModel::skeleton() const
{
  return m_skeleton;
}

void AnimatedModel::setSkeleton(SkeletonHandle skeleton)
{
  m_skeleton = skeleton;
}

ModelHandle AnimatedModel::skin() const
{
  return m_skin;
}

void AnimatedModel::setSkin(ModelHandle skin)
{
  m_skin = skin;
}

void AnimatedModel::addAnimation(AnimationHandle animation)
{
  if (!m_animations.contains(animation))
    m_animations.push_back(animation);
}

const Array<AnimationHandle>& AnimatedModel::animations() const
{
  return m_animations;
}

const Mat4& AnimatedModel::worldInverseTransform() const
{
  return m_world_inverse_transform;
}

void AnimatedModel::setWorldInverseTransform(const Mat4& transform)
{
  m_world_inverse_transform = transform;
}
