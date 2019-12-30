#include <GameFramework/Public/ECS/GameObject.hpp>
#include <GameFramework/Public/Managers/AssetManager.hpp>
#include <Graphics/Public/AnimatedMeshRenderer.hpp>
#include <Graphics/Public/AnimatedModel.hpp>
#include <Graphics/Public/Animator.hpp>
#include <Graphics/Public/Animation.hpp>

/**
 * Add AnimatedMeshRenderer before Animator
 */

Animator::Animator(const String& name, GameObject* gameObject)
  : Component(name, gameObject),
    m_current_anim{0},
    m_animation_time{0.0},
    m_animated_model{nullptr},
    m_animations{},
    m_skeleton{nullptr},
    m_is_initialized{false},
    m_animated_mesh_renderer{nullptr}
{
  auto* animatedMeshRenderer = m_game_object->getComponent<AnimatedMeshRenderer>();
  if (animatedMeshRenderer) {
    m_animated_mesh_renderer = animatedMeshRenderer;
    animatedMeshRenderer->setAnimator(this);
  }
}

void Animator::init(AssetManager* assetManager)
{
  if (!m_is_initialized) {
    auto* animatedMeshRenderer = m_game_object->getComponent<AnimatedMeshRenderer>();

    if (animatedMeshRenderer) {
      m_animated_model = assetManager->getAnimatedModel(animatedMeshRenderer->modelHandle());
      for (auto anim : m_animated_model->animations()) {
        m_animations.push_back(assetManager->getAnimation(anim));
      }

      m_skeleton = assetManager->getSkeleton(m_animated_model->skeleton());
    }

    m_bone_transforms.resize(m_skeleton->bones().size());

    m_is_initialized = true;
  }
}

void Animator::update(Real dt)
{
  if (m_current_anim != -1) {
    Animation* currentAnim = m_animations[m_current_anim];
    m_animation_time = fmodf(m_animation_time + dt * currentAnim->ticksPerSecond, currentAnim->duration);
    _update_skeleton_recursively(m_animation_time, currentAnim, m_skeleton, m_skeleton->boneAt(0), Math::Mat4Identity);

    for (auto i = 0; i < m_bone_transforms.size(); ++i) {
      m_bone_transforms[i] = m_skeleton->boneAt(i)->animatedTransform;
    }
  }
}

void Animator::runNextAnimation()
{
  m_current_anim = (m_current_anim + 1) % m_animations.size();
}

void Animator::runAnimation(const String& name)
{
  for (auto i = 0; i < m_animations.size(); ++i) {
    if (m_animations[i]->name == name) {
      m_current_anim = i;
    }
  }
}

void Animator::_update_skeleton_recursively(Real animTime, const Animation* animation, const Skeleton* skeleton, Bone* bone, const Mat4& parentTransform)
{
  // Obtain node transformation from current Bone.
  Mat4 currentTransform = bone->nodeTransform;

  // Animation of current bone/node
  const NodeAnimation* nodeAnimation = nullptr;
  for (auto& anim : animation->nodeAnimations) {
    if (anim.name == bone->name) {
      nodeAnimation = &anim;
    }
  }

  if (nodeAnimation) {
    // Interpolation
    Vec3 t = _interpolate_translation(nodeAnimation->positionKeys, animTime);
    Quat r = _interpolate_rotation(nodeAnimation->rotationKeys, animTime);
    Vec3 s = _interpolate_scaling(nodeAnimation->scalingKeys, animTime);

    // Combine the transformation
    currentTransform = Math::mat4FromTRS(t, r, s);
  }

  Mat4 toWorldTransformation = parentTransform * currentTransform;

  bone->animatedTransform = m_animated_model->worldInverseTransform() * toWorldTransformation * bone->offSetMatrix;
  //bone->animatedTransform = m_animated_model->worldInverseTransform() * toWorldTransformation * bone->offSetMatrix;

  for (auto child : bone->children) {
    _update_skeleton_recursively(animTime, animation, skeleton, skeleton->boneAt(child), toWorldTransformation);
  }
}

Vec3 Animator::_interpolate_translation(const Array<AnimKeyFrame<Vec3>>& keyFrames, Real animTime)
{
  if (keyFrames.size() == 1) {
    return keyFrames.at(0).value;
  }

  Int32 key = _locate_animation_key_frame(keyFrames, animTime);
  Int32 nextKey = key + 1;
  Real delta = keyFrames[nextKey].time - keyFrames[key].time;
  Real t = (animTime - keyFrames[key].time) / delta;

  return Math::lerp(keyFrames[key].value, keyFrames[nextKey].value, t);
}

Quat Animator::_interpolate_rotation(const Array<AnimKeyFrame<Quat>>& keyFrames, Real animTime)
{
  if (keyFrames.size() == 1) {
    return keyFrames.at(0).value;
  }

  Int32 key = _locate_animation_key_frame(keyFrames, animTime);
  Int32 nextKey = key + 1;
  Real delta = keyFrames[nextKey].time - keyFrames[key].time;
  Real t = (animTime - keyFrames[key].time) / delta;

  return Quat::slerp(keyFrames[key].value, keyFrames[nextKey].value, t);
}

Vec3 Animator::_interpolate_scaling(const Array<AnimKeyFrame<Vec3>>& keyFrames, Real animTime)
{
  if (keyFrames.size() == 1) {
    return keyFrames.at(0).value;
  }

  Int32 key = _locate_animation_key_frame(keyFrames, animTime);
  Int32 nextKey = key + 1;
  Real delta = keyFrames[nextKey].time - keyFrames[key].time;
  Real t = (animTime - keyFrames[key].time) / delta;

  return Math::lerp(keyFrames[key].value, keyFrames[nextKey].value, t);
}

const Array<Mat4>& Animator::boneTransforms() const
{
  return m_bone_transforms;
}

bool Animator::isInitialized() const
{
  return m_is_initialized;
}

UInt64 Animator::typeID() const {
  return Component::family::type<Animator>;
}
