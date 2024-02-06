#pragma once
#include "System/Vector.h"


namespace celestia
{
	namespace math
	{
		Vec3 normalize(const Vec3& vec);

		float dotf(const Vec3& vec1, const Vec3& vec2);

		float inverseSqrt(const float x);
	}
}
