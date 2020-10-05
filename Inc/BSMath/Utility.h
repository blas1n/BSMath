#pragma once

#include <cmath>
#include "SIMD.h"

namespace BSMath
{
    constexpr float Pi = 3.141592654f;
    constexpr float Epsilon = 1.192092896e-07F;

    namespace Detail
    {
        template <class T>
        [[nodiscard]] constexpr T MinImpl(const void*, const T& lhs, const T& rhs) noexcept
        {
            return lhs < rhs ? lhs : rhs;
        }

        template <class T>
        [[nodiscard]] constexpr T MaxImpl(const void*, const T& lhs, const T& rhs) noexcept
        {
            return lhs > rhs ? lhs : rhs;
        }

        template <class T>
        [[nodiscard]] constexpr T ClampImpl(const void*, const T& n, const T& min, const T& max) noexcept
        {
            return Max(Min(n, max), min);
        }

        template <class T>
        [[nodiscard]] constexpr T AbsImpl(const void*, const T& n) noexcept
        {
            return n >= static_cast<T>(0) ? n : -n;
        }

        template <class T>
        [[nodiscard]] constexpr T SignImpl(const void*, const T& n) noexcept
        {
            return n >= static_cast<T>(0) ? static_cast<T>(1) : static_cast<T>(-1);
        }
    }

    template <class T>
    [[nodiscard]] constexpr T Min(const T& lhs, const T& rhs) noexcept
    {
        return Detail::MinImpl(static_cast<T*>(nullptr), lhs, rhs);
    }

    template <class T1, class T2>
    [[nodiscard]] constexpr auto Min(const T1& lhs, const T2& rhs) noexcept
    {
        return lhs < rhs ? lhs : rhs;
    }

    template <class T1, class T2, class... Ts>
    [[nodiscard]] constexpr auto Min(const T1& x1, const T2& x2, const Ts&... xs) noexcept
    {
        return Min(Min(x1, x2), xs...);
    }

    template <class ForwardIt>
    [[nodiscard]] constexpr typename std::iterator_traits<ForwardIt>::value_type Min(ForwardIt first, ForwardIt last) noexcept
    {
        if (first == last) return *first;

        auto ret = *first;
        for (++first; first != last; ++first)
            ret = Min(ret, *first);

        return ret;
    }

    template <class T>
    [[nodiscard]] constexpr T Max(const T& lhs, const T& rhs) noexcept
    {
        return Detail::MaxImpl(static_cast<T*>(nullptr), lhs, rhs);
    }

    template <class T1, class T2>
    [[nodiscard]] constexpr auto Max(const T1& lhs, const T2& rhs) noexcept
    {
        return lhs > rhs ? lhs : rhs;
    }

    template <class T1, class T2, class... Ts>
    [[nodiscard]] constexpr auto Max(const T1& x1, const T2& x2, const Ts&... xs) noexcept
    {
        return Max(Max(x1, x2), xs...);
    }

    template <class ForwardIt>
    [[nodiscard]] constexpr typename std::iterator_traits<ForwardIt>::value_type Max(ForwardIt first, ForwardIt last) noexcept
    {
        if (first == last) return *first;

        auto ret = *first;
        for (++first; first != last; ++first)
            ret = Max(ret, *first);

        return ret;
    }

    template <class T>
    [[nodiscard]] constexpr T Clamp(const T& n, const T& min, const T& max) noexcept
    {
        return Detail::ClampImpl(static_cast<T*>(nullptr), n, min, max);
    }

    template <class T, class U, class V>
    [[nodiscard]] constexpr auto Clamp(const T& n, const U& min, const V& max) noexcept
    {
        return Max(Min(n, max), min);
    }

    template <class T>
    [[nodiscard]] constexpr T Abs(const T& n) noexcept
    {
        return Detail::AbsImpl(static_cast<T*>(nullptr), n);
    }

    template <class T>
    [[nodiscard]] constexpr T Sign(const T& n) noexcept
    {
        return Detail::SignImpl(static_cast<T*>(nullptr), n);
    }

    [[nodiscard]] NO_ODR float Cos(float n) noexcept { return std::cos(n); }
    [[nodiscard]] NO_ODR float Sin(float n) noexcept { return std::sin(n); }
    [[nodiscard]] NO_ODR float Tan(float n) noexcept { return std::tan(n); }

    [[nodiscard]] NO_ODR float ACos(float n) noexcept { return std::acos(n); }
    [[nodiscard]] NO_ODR float ASin(float n) noexcept { return std::asin(n); }
    [[nodiscard]] NO_ODR float ATan(float n) noexcept { return std::atan(n); }
    [[nodiscard]] NO_ODR float ATan2(float y, float n) noexcept { return std::atan2(y, n); }

    [[nodiscard]] constexpr bool IsNearlyEqual(float lhs, float rhs, float tolerance = Epsilon) noexcept
    {
        return Abs(lhs - rhs) <= tolerance;
    }

    [[nodiscard]] constexpr bool IsNearlyZero(float n, float tolerance = Epsilon) noexcept
    {
        return Abs(n) <= tolerance;
    }

    [[nodiscard]] constexpr float GetRangePct(float n, float min, float max) noexcept
    {
        return (n - min) / (max - min);
    }

    template <class T>
    [[nodiscard]] T Lerp(const T& a, const T& b, float t) noexcept
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
    [[nodiscard]] constexpr float Square(T n) noexcept { return n * n; }

    [[nodiscard]] NO_ODR float InvSqrt(float n, size_t iterationNum = 2) noexcept
    {
        return SIMD::InvSqrt(n, iterationNum);
    }

    [[nodiscard]] NO_ODR float Sqrt(float n, size_t iterationNum = 2) noexcept
    {
        return IsNearlyZero(n) ? 0.0f : n * InvSqrt(n, iterationNum);
    }

    [[nodiscard]] NO_ODR float Fmod(float x, float y) noexcept
    {
        return std::fmod(x, y);
    }

    [[nodiscard]] NO_ODR int Trunc(float n) noexcept
    {
        return static_cast<int>(std::trunc(n));
    }

    [[nodiscard]] NO_ODR int Ceil(float n) noexcept
    {
        return static_cast<int>(std::ceil(n));
    }

    [[nodiscard]] NO_ODR int Round(float n) noexcept
    {
        return static_cast<int>(std::round(n));
    }

    [[nodiscard]] NO_ODR int Floor(float n) noexcept
    {
        return static_cast<int>(std::floor(n));
    }
}