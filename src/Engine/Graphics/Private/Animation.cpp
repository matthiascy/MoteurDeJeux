#include <Graphics/Public/Animation.hpp>

Skeleton::Skeleton()
{
  m_bones = makeUnique<Array<Bone*>>();
}

const Array<Bone*>& Skeleton::bones() const
{
  return *m_bones;
}

Array<Bone*>& Skeleton::bones()
{
  return *m_bones;
}

Bone* Skeleton::boneAt(UInt32 idx) const
{
  if (idx < m_bones->size())
    return m_bones->at(idx);
}

Skeleton::~Skeleton()
{
  for (auto* bone : *m_bones) {
    delete bone;
  }
  m_bones.reset(nullptr);
}

Bone* Skeleton::bone(const String& name) const
{
  for (auto* bone : *m_bones) {
    if (bone->name == name) {
      return bone;
    }
  }
  return nullptr;
}
