#include "VectorMath.h"
#include <cmath>

celestia::Vec3 celestia::math::normalize(const Vec3& vec)
{
    return vec * inverseSqrt(dotf(vec, vec));
}

float celestia::math::dotf(const Vec3& vec1, const Vec3& vec2)
{
    celestia::Vec3 temp(vec1 * vec2);
    return temp.x + temp.y + temp.z;
}

float celestia::math::inverseSqrt(const float x)
{
    return 1.f / sqrtf(x);
}
