#pragma once

#include <numbers>

namespace Math {
constexpr inline float deg2rad(float deg)
{
    return deg * (std::numbers::pi / 180);
}

constexpr inline float rad2deg(float rad)
{
    return rad * (180 / std::numbers::pi);
}
}