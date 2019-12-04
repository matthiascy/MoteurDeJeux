#include <Core/Math/Math.hpp>
#include <QtGui/QTransform>

Mat4 Math::ortho(Real left, Real right, Real bottom, Real top, Real znear, Real zfar)
{
  Mat4 out;
  out.ortho(left, right, bottom, top, znear, zfar);
  return out;
}

Mat4 Math::perspective(Real fov, Real aspect, Real znear, Real zfar)
{
  Mat4 out;
  out.perspective(fov, aspect, znear, zfar);
  return out;
}

Mat4 Math::lookAtMatrix(const Vec3& eye, const Vec3& center, const Vec3& up)
{
  Mat4 out;
  out.lookAt(eye, center, up);
  return out;
}

Quat Math::lookAtQuaternion(const Vec3& eye, const Vec3& center, const Vec3& up)
{
  return Quat::fromRotationMatrix(lookAtMatrix(eye, center, up).normalMatrix());
}

const Mat4 Math::Mat4Identity = Mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

const Quat Math::QuatIdentity = Quat(1.0f, 0.0f, 0.0f, 0.0f);

