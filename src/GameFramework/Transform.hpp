#ifndef MOTEUR_DE_JEUX_SRC_TRANSFORM_HPP
#define MOTEUR_DE_JEUX_SRC_TRANSFORM_HPP

#include <Core/Core.hpp>

class Transform {
private:
  Vec3 m_position;
  Quat m_rotation;
  Vec3 m_scale;

public:
  explicit Transform(const Vec3& position = vec3::Zero, const Quat& rotation = quat::Identity, const Vec3& scale = vec3::mkVec3(1));
  ~Transform() = default;

  Transform& rotate(const Vec3& axis, Real angle);
  Transform& rotate(const Quat& q);
  Transform& scale(Real s);
  Transform& scale(const Vec3& s);
  Transform& translate(const Vec3& t);

  Transform& setPosition(const Vec3& position);
  Transform& setScale(const Vec3& scale);
  Transform& setRotation(const Quat& rotation);

  const Vec3& position() const;
  const Vec3& scale() const;
  const Quat& rotation() const;

  UInt32 childCount() const;

  Mat4 matrix() const;
};


#endif  /* !MOTEUR_DE_JEUX_SRC_TRANSFORM_HPP */
