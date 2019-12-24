#include "Core/Public/Math/BoundingBox.hpp"

BoundingBox::BoundingBox(Real min, Real max)
{
  m_bbox.min = {min, min, min};
  m_bbox.max = {max, max, max};
}

BoundingBox::BoundingBox(const Vec3& min, const Vec3& max)
{
  m_bbox.min = min;
  m_bbox.max = max;
}

Vec3 BoundingBox::center() const
{
  return (m_bbox.max + m_bbox.min) * 0.5f;
}

Vec3 BoundingBox::size() const
{
  return m_bbox.max - m_bbox.min;
}

Vec3 BoundingBox::halfSize() const
{
  return (m_bbox.max - m_bbox.min) * 0.5f;
}

bool BoundingBox::operator==(const BoundingBox& other) const {
  return other.m_bbox.min == m_bbox.min && other.m_bbox.max == m_bbox.max;
}

bool BoundingBox::operator!=(const BoundingBox& other) const {
  return !(other == *this);
}
