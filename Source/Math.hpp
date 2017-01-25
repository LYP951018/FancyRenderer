#pragma once

#include <cmath>

namespace Fancy::Math
{
    constexpr float kPi = 3.141592654f;
    constexpr float k2Pi = 6.283185307f;

    constexpr float kqNan = NAN;
    constexpr float kInfinity = INFINITY;

    inline bool FloatEqual(float lhs, float rhs, float epsilon = 1e-6f) noexcept
    {
        return std::abs(lhs - rhs) <= epsilon;
    }
}


