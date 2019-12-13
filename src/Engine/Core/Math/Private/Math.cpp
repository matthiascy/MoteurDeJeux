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
  Mat4 mat = lookAtMatrix(eye, center, up);
  float data[] = {mat.row(0)[0], mat.row(0)[1], mat.row(0)[2],
                  mat.row(1)[0], mat.row(1)[1], mat.row(1)[2],
                  mat.row(2)[0], mat.row(2)[1], mat.row(2)[2]};
  return Quat::fromRotationMatrix(Mat3(data));
}

const Mat4 Math::Mat4Identity = Mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

const Quat Math::QuatIdentity = Quat(1.0f, 0.0f, 0.0f, 0.0f);

Mat4 Math::mat4FromTRS(const Vec3& translation, const Quat& rotation, const Vec3& scale)
{
  Mat4 out;
  out.scale(scale);
  out.rotate(rotation);
  out.translate(translation);
  return out;
}

Vec3 Math::extractTranslation(const Mat4& mat)
{
  return {mat.column(3)[0], mat.column(3)[1], mat.column(3)[2]};
}

Vec3 Math::extractScale(const Mat4& mat)
{
  return {axisX(mat).length(), axisY(mat).length(), axisZ(mat).length()};
}

Quat Math::extractRotation(const Mat4& mat)
{
  float data[] = {mat.row(0)[0], mat.row(0)[1], mat.row(0)[2],
                  mat.row(1)[0], mat.row(1)[1], mat.row(1)[2],
                  mat.row(2)[0], mat.row(2)[1], mat.row(2)[2]};
  return Quat::fromRotationMatrix(Mat3(data));
}

Vec3 Math::axisX(const Mat4& mat)
{
  Vec4 v = mat.column(0);
  return {v.x(), v.y(), v.z()};
}

Vec3 Math::axisY(const Mat4& mat)
{
  Vec4 v = mat.column(1);
  return {v.x(), v.y(), v.z()};
}

Vec3 Math::axisZ(const Mat4& mat)
{
  Vec4 v = mat.column(2);
  return {v.x(), v.y(), v.z()};
}