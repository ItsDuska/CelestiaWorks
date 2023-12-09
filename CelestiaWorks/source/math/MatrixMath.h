#pragma once
#include "celestiaTypes/Matrix.h"

namespace celestia
{
	namespace math
	{
		Mat4 translate(const Mat4& m, const Vec3& v);

		Mat4 scale(const Mat4& m, const Vec3& v);

		Mat4 ortho(float left, float right, float bottom, float top, float zNear, float zFar);
	}
}