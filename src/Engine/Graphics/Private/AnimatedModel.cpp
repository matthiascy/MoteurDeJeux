#include "Graphics/Public/AnimatedModel.hpp"

const BoneAnimation* FindNodeAnim(const Animation& anim, const String& nodeName)
{
  for (const auto & nodeAnimation : anim.boneAnimations) {
    if (nodeAnimation.name == nodeName) {
      return &nodeAnimation;
    }
  }
}

Vec3 InterpolateValue(const Vec3& start, const Vec3& end, float t)
{
  return Math::lerp(start, end, t);
}

Quat InterpolateValue(const Quat& start, const Quat& end, float t)
{
  return Quat::slerp(start, end, t);
}

UInt32 Skeleton_FindBoneIndex(const Skeleton& skeleton, const String& name)
{
  for (auto i = 0; i < skeleton.bones.size(); ++i) {
    if (skeleton.bones[i].name == name)
      return i;
  }
  return -1;
}

Bone* Skeleton_FindBone(Skeleton& skeleton, const String& name)
{
  return &skeleton.bones[Skeleton_FindBoneIndex(skeleton, name)];
}

/*
AnimatedModel::AnimatedModel()
  : m_animation_time(0.0f)
{
  clear();
}

AnimatedModel::~AnimatedModel()
{
  clear();
}

void AnimatedModel::update(float dt)
{
  m_animation_time = fmodf(m_animation_time + dt * m_animation.ticksPerSecond, m_animation.duration);

  _read_node_hierarchy(m_animation_time, m_animation, m_skeleton, m_skeleton.bones[0], Math::Mat4Identity);
  _transform_vertices(m_skeleton);
}

void AnimatedModel::clear()
{
  m_meshes.clear();
  m_skeleton.bones.clear();
  m_animation.boneAnimations.clear();
  m_animation.duration = 0.0f;
  m_animation.ticksPerSecond = 0.0f;
  m_global_inverse_transform = Math::Mat4Identity;
  m_animation_time = 0.0f;
}

void AnimatedModel::_read_node_hierarchy(Real animationTime, Animation& animation, Skeleton& skeleton, Bone& bone,
                                         const Mat4& parentTransform)
{
  String nodeName(bone.name);
  Mat4 nodeTransform = bone.nodeTransform;
  const BoneAnimation* newNodeAnim = FindNodeAnim(animation, nodeName);

  if (newNodeAnim) {
    Vec3 translation = NodeAnimation_FindInterpolatedPosition(*newNodeAnim, animationTime);
    Quat rotation = NodeAnimation_FindInterpolatedRotation(*newNodeAnim, animationTime);
    // TODO: scaling
    nodeTransform.setToIdentity();
    nodeTransform.rotate(rotation);
    nodeTransform.translate(translation);
  }

  Mat4 globalTransform = parentTransform * nodeTransform;

  UInt32 boneIndex = Skeleton_FindBoneIndex(skeleton, nodeName);
  if (boneIndex != -1) {
    Joint& joint = skeleton.bones[boneIndex];
    joint.finalTransform = m_global_inverse_transform * globalTransform * joint.offsetMatrix;
  }

  for (UInt32 i : bone.children) {
    _read_node_hierarchy(animationTime, animation, skeleton, skeleton.bones[i], globalTransform);
  }
}

void AnimatedModel::_transform_vertices(const Skeleton& skeleton)
{
  for (UInt32 i = 0; i < m_meshes.size(); ++i) {
    AnimatedMesh& animMesh = m_meshes[i];

    animMesh.transformedVertices.clear();
    animMesh.transformedNormals.clear();

    for (UInt32 j = 0; j < skeleton.bones.size(); ++i) {
      const Bone& bone = skeleton.bones[i];

      Mat4 transform = bone.finalTransform;
      Quat rotation = Math::extractRotationQuat(transform);

      for (UInt32 k = 0; k < bone.weights.size(); ++i) {
        VertexWeight weight = bone.weights[i];
        Vec3 inVertex = animMesh.vertices[weight.vertexIdx];
        Vec3& outVertex = animMesh.transformedVertices[weight.vertexIdx];
        outVertex += Vec3((transform * Vec4(inVertex, 1)) * weight.weight);

        Vec3 inNormal = animMesh.normals[weight.vertexIdx];
        Vec3& outNormal = animMesh.transformedNormals[weight.vertexIdx];
        outNormal += (rotation.rotatedVector(inNormal)) * weight.weight;
      }
    }

    // normalize normals
    for (auto& mesh : m_meshes) {
      for (auto j = 0; j < mesh.vertices.size(); ++j) {
        mesh.transformedNormals[j] = mesh.transformedNormals[j].normalized();
      }
    }
  }
}

Vec3 NodeAnimation_FindInterpolatedPosition(const BoneAnimation& nodeAnim, float animTime)
{
  return NodeAnimation_FindInterpolatedValue(nodeAnim.positionKeys, animTime);
}

Quat NodeAnimation_FindInterpolatedRotation(const BoneAnimation& nodeAnim, float animTime)
{
  return NodeAnimation_FindInterpolatedValue(nodeAnim.rotationKeys, animTime);
}

 */