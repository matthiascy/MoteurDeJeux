#include <Core/Math/Math.hpp>

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

const Mat4 Math::Mat4Identity = Mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

const Quat Math::QuatIdentity = Quat(1.0f, 0.0f, 0.0f, 0.0f);

