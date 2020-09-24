#pragma once

#include "SSE.h"

namespace BSMath
{
    constexpr float Pi = 3.141592654f;
    constexpr float Epsilon = 1.192092896e-07F;

    template <class T1, class T2>
    [[nodiscard]] constexpr auto Min(T1 lhs, T2 rhs) noexcept
    {
        return lhs < rhs ? lhs : rhs;
    }

    template <class T1, class T2, class... Ts>
    [[nodiscard]] constexpr auto Min(T1 x1, T2 x2, Ts... xs) noexcept
    {
        return Min(Min(x1, x2), xs...);
    }

    template <class T1, class T2>
    [[nodiscard]] constexpr auto Max(T1 lhs, T2 rhs) noexcept
    {
        return lhs > rhs ? lhs : rhs;
    }

    template <class T1, class T2, class... Ts>
    [[nodiscard]] constexpr auto Max(T1 x1, T2 x2, Ts... xs) noexcept
    {
        return Max(Max(x1, x2), xs...);
    }

    template <class T, class U, class V>
    [[nodiscard]] constexpr auto Clamp(T v, U min, V max) noexcept
    {
        return Max(Min(v, max), min);
    }

    template <class T>
    [[nodiscard]] constexpr T Abs(T n) noexcept
    {
        return n >= static_cast<T>(0) ? n : -n;
    }

    template <class T>
    [[nodiscard]] constexpr T Sign(T n) noexcept
    {
        return n >= static_cast<T>(0) ? static_cast<T>(1) : static_cast<T>(-1);
    }

    [[nodiscard]] inline float Cos(float n) noexcept { return std::cos(n); }
    [[nodiscard]] inline float Sin(float n) noexcept { return std::sin(n); }
    [[nodiscard]] inline float Tan(float n) noexcept { return std::tan(n); }

    [[nodiscard]] inline float ACos(float n) noexcept { return std::acos(n); }
    [[nodiscard]] inline float ASin(float n) noexcept { return std::asin(n); }
    [[nodiscard]] inline float ATan(float n) noexcept { return std::atan(n); }
    [[nodiscard]] inline float ATan2(float y, float n) noexcept { return std::atan2(y, n); }

    [[nodiscard]] constexpr bool IsNearlyEqual(float lhs, float rhs, float tolerance = Epsilon) noexcept
    {
        return Abs(lhs - rhs) <= tolerance;
    }

    [[nodiscard]] constexpr bool IsNearlyZero(float n, float tolerance = Epsilon) noexcept
    {
        return Abs(n) <= tolerance;
    }

    [[nodiscard]] constexpr float Normalized(float n, float min, float max) noexcept
    {
        return (n - min) / (max - min);
    }

    [[nodiscard]] constexpr float Lerp(float a, float b, float t) noexcept
    {
        return a + t * (b - a);
    }

    [[nodiscard]] constexpr float Rad2Deg(float rad) noexcept
    {
        return rad * (180.0f / Pi);
    }

    [[nodiscard]] constexpr float Deg2Rad(float deg) noexcept
    {
        return deg * (Pi / 180.0f);
    }

    template <class T>
    [[nodiscard]] constexpr float Square(T n) { return n * n; }

    [[nodiscard]] inline float Sqrt(float n, size_t iterationNum = 2) noexcept
    {
        return IsNearlyZero(n) ? 0.0f : n * SSE::InvSqrt(n, iterationNum);
    }

    [[nodiscard]] inline float InvSqrt(float n, size_t iterationNum = 2) noexcept
    {
        return SSE::InvSqrt(n, iterationNum);
    }

    [[nodiscard]] inline int Trunc(float n) noexcept
    {
        return SSE::Trunc(n);
    }

    [[nodiscard]] inline int Ceil(float n) noexcept
    {
        return SSE::Ceil(n);
    }

    [[nodiscard]] inline int Round(float n) noexcept
    {
        return SSE::Round(n);
    }

    [[nodiscard]] inline int Floor(float n) noexcept
    {
        return SSE::Floor(n);
    }
}