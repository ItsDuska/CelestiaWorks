#include "MatrixMath.h"
#include "VectorMath.h"
#include <cmath>

#define DEG_TO_RAD 0.01745329251994329577f

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

celestia::Mat4 celestia::math::rotate(const Mat4& m, const float angle, const Vec3& vec)
{
	const float a = angle;
	const float c = cos(a);
	const float s = sin(a);
	
	Vec3 axis(normalize(vec));
	Vec3 temp(axis * (1.f - c));

	Mat4 Rotate;
	Rotate[0].x = c + temp.x * axis.x;
	Rotate[0].y = temp.x * axis.y + s * axis.z;
	Rotate[0].z = temp.x * axis.z - s * axis.y;

	Rotate[1].x = temp.y * axis.x - s * axis.z;
	Rotate[1].y = c + temp.y * axis.y;
	Rotate[1].z = temp.y * axis.z + s * axis.x;

	Rotate[2].x = temp.z * axis.x + s * axis.y;
	Rotate[2].y = temp.z * axis.y - s * axis.x;
	Rotate[2].z = c + temp.z * axis.z;

	Mat4 Result;
	Result[0] = m[0] * Rotate[0].x + m[1] * Rotate[0].y + m[2] * Rotate[0].z;
	Result[1] = m[0] * Rotate[1].x + m[1] * Rotate[1].y + m[2] * Rotate[1].z;
	Result[2] = m[0] * Rotate[2].x + m[1] * Rotate[2].y + m[2] * Rotate[2].z;
	Result[3] = m[3];
	return Result;
}

float celestia::math::radians(float degrees)
{
	return degrees * DEG_TO_RAD;
}
