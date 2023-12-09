#include "MatrixMath.h"

celestia::Mat4 celestia::math::translate(const Mat4& m, const Vec3& v)
{
	Mat4 Result(m);
	Result[3] = Vec4(m[0] * v.x) + (m[1] * v.y) + (m[2] * v.z + m[3]);
	return Result;
}

celestia::Mat4 celestia::math::scale(const Mat4& m, const Vec3& v)
{
	Mat4 Result(1.f);
	Result[0] = m[0] * v.x;
	Result[1] = m[1] * v.y;
	Result[2] = m[2] * v.z;
	Result[3] = m[3];
	return Result;
}

celestia::Mat4 celestia::math::ortho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	Mat4 result(1.f);
	result[0].x = 2.f / (right - left);
	result[1].y = 2.f / (top - bottom);
	result[2].z = -1.f;
	result[3].x = -(right + left) / (right - left);
	result[3].y = -(top + bottom) / (top - bottom);
	return result;
	
}
