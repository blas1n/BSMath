#pragma once

#include <cmath>
#include  <limits>

namespace BSMath
{
    constexpr float Pi = 3.141592654f;
    constexpr float Epsilon = std::numeric_limits<float>::epsilon();

    template <class T1, class T2>
    [[nodiscard]] constexpr decltype(auto) Min(T1 lhs, T2 rhs) noexcept
    {
        return lhs < rhs ? lhs : rhs;
    }

    template <class T1, class T2, class... Ts>
    [[nodiscard]] constexpr decltype(auto) Min(T1 x1, T2 x2, Ts... xs) noexcept
    {
        return Min(Min(x1, x2), xs...);
    }

    template <class T1, class T2>
    [[nodiscard]] constexpr decltype(auto) Max(T1 lhs, T2 rhs) noexcept
    {
        return lhs > rhs ? lhs : rhs;
    }

    template <class T1, class T2, class... Ts>
    [[nodiscard]] constexpr decltype(auto) Max(T1 x1, T2 x2, Ts... xs) noexcept
    {
        return Max(Max(x1, x2), xs...);
    }

    template <class T, class U, class V>
    [[nodiscard]] constexpr decltype(auto) Clamp(T v, U min, V max) noexcept
    {
        return Max(Min(v, max), min);
    }

    template <class T>
    [[nodiscard]] constexpr T Abs(T x) noexcept
    {
        return x >= static_cast<T>(0) ? x : -x;
    }

    template <class T>
    [[nodiscard]] constexpr T Sign(T x) noexcept
    {
        return x >= static_cast<T>(0) ? static_cast<T>(1) : static_cast<T>(-1);
    }

    [[nodiscard]] inline float Cos(float x) noexcept { return std::cos(x); }
    [[nodiscard]] inline float Sin(float x) noexcept { return std::sin(x); }
    [[nodiscard]] inline float Tan(float x) noexcept { return std::tan(x); }

    [[nodiscard]] inline float ACos(float x) noexcept { return std::acos(x); }
    [[nodiscard]] inline float ASin(float x) noexcept { return std::asin(x); }
    [[nodiscard]] inline float ATan(float x) noexcept { return std::atan(x); }
    [[nodiscard]] inline float ATan2(float y, float x) noexcept { return std::atan2(y, x); }

    [[nodiscard]] constexpr bool IsNearlyEqual(float lhs, float rhs, float tolerance = Epsilon) noexcept
    {
        return Abs(lhs - rhs) <= tolerance;
    }

    [[nodiscard]] constexpr bool IsNearlyZero(float x, float tolerance = Epsilon) noexcept
    {
        return Abs(x) <= tolerance;
    }

    [[nodiscard]] constexpr float Normalized(float x, float min, float max) noexcept
    {
        return (x - min) / (max - min);
    }

    [[nodiscard]] constexpr float Lerp(float a, float b, float t) noexcept
    {
        return a + t * (b - a);
    }

    [[nodiscard]] constexpr float Rad2Deg(float rad) noexcept
    {
        return rad * (360.0f / (Pi * 2.0f));
    }

    [[nodiscard]] constexpr float Deg2Rad(float deg) noexcept
    {
        return deg * ((Pi * 2.0f) / 360.0f);
    }
}