#pragma once

#include <emmintrin.h>
#include <type_traits>
#include "Basic.h"

#if defined(_MSC_VER) && !defined(_M_ARM) && !defined(_M_ARM64) \
    && !defined(_M_HYBRID_X86_ARM64) && (!_MANAGED) && (!_M_CEE) \
    && (!defined(_M_IX86_FP) || (_M_IX86_FP > 1)) && !defined(_XM_NO_INTRINSICS_) && !defined(_XM_VECTORCALL_)
#define HAS_VECTOR_CALL
#endif

#if defined(HAS_VECTOR_CALL)
#define VECTOR_CALL __vectorcall
#elif defined(__GNUC__)
#define VECTOR_CALL
#else
#define VECTOR_CALL __fastcall
#endif

namespace BSMath::SIMD
{
    enum class Swizzle : uint32
    {
        X, Y, Z, W
    };

    template <class T>
    using VectorRegister = std::conditional_t<std::is_integral_v<T>, __m128i, __m128>;

    template <class T>
    inline const VectorRegister<T> Zero;

    template <>
    inline const VectorRegister<float> Zero<float> = _mm_setzero_ps();

    template <>
    inline const VectorRegister<int> Zero<int> = _mm_setzero_si128();

    template <class T>
    inline const VectorRegister<T> One;

    template <>
    inline const VectorRegister<float> One<float> = _mm_set1_ps(1.0f);

    template <>
    inline const VectorRegister<int> One<int> = _mm_set1_epi32(1);

    [[nodiscard]] NO_ODR VectorRegister<float> VectorLoad(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f) noexcept
    {
        return _mm_setr_ps(x, y, z, w);
    }

    [[nodiscard]] NO_ODR VectorRegister<int> VectorLoad(int x = 0, int y = 0, int z = 0, int w = 0) noexcept
    {
        return _mm_setr_epi32(x, y, z, w);
    }

    template <size_t L>
    [[nodiscard]] NO_ODR VectorRegister<float> VectorLoad(const float(&vec)[L]) noexcept
    {
        if constexpr (L == 1)
            return VectorLoad(vec[0]);
        else if (L == 2)
            return VectorLoad(vec[0], vec[1]);
        else if (L == 3)
            return VectorLoad(vec[0], vec[1], vec[2]);
        else
            return VectorLoad(vec[0], vec[1], vec[2], vec[3]);
    }

    template <size_t L>
    [[nodiscard]] NO_ODR VectorRegister<int> VectorLoad(const int(&vec)[L]) noexcept
    {
        if constexpr (L == 1)
            return VectorLoad(vec[0]);
        else if (L == 2)
            return VectorLoad(vec[0], vec[1]);
        else if (L == 3)
            return VectorLoad(vec[0], vec[1], vec[2]);
        else
            return VectorLoad(vec[0], vec[1], vec[2], vec[3]);
    }

    [[nodiscard]] NO_ODR VectorRegister<float> VectorLoad1(float n) noexcept
    {
        return _mm_set1_ps(n);
    }

    [[nodiscard]] NO_ODR VectorRegister<int> VectorLoad1(int n) noexcept
    {
        return _mm_set1_epi32(n);
    }

    NO_ODR void VECTOR_CALL VectorStore(VectorRegister<float> vec, float* ptr) noexcept
    {
        _mm_store_ps(ptr, vec);
    }

    NO_ODR void VECTOR_CALL VectorStore(VectorRegister<int> vec, int* ptr) noexcept
    {
        _mm_store_si128(reinterpret_cast<VectorRegister<int>*>(ptr), vec);
    }

    [[nodiscard]] NO_ODR float VECTOR_CALL VectorStore1(VectorRegister<float> vec) noexcept
    {
        float ret;
        _mm_store_ps1(&ret, vec);
        return ret;
    }

    [[nodiscard]] NO_ODR int VECTOR_CALL VectorStore1(VectorRegister<int> vec) noexcept
    {
        return _mm_cvtsi128_si32(vec);
    }

    template <Swizzle X, Swizzle Y, Swizzle Z, Swizzle W>
    [[nodiscard]] NO_ODR VectorRegister<float> VECTOR_CALL VectorSwizzle(VectorRegister<float> vec) noexcept
    {
        constexpr static auto mask = _MM_SHUFFLE(static_cast<uint32>(W),
            static_cast<uint32>(Z), static_cast<uint32>(Y), static_cast<uint32>(X));
        return _mm_shuffle_ps(vec, vec, mask);
    }

    template <Swizzle X, Swizzle Y, Swizzle Z, Swizzle W>
    [[nodiscard]] NO_ODR VectorRegister<int> VECTOR_CALL VectorSwizzle(VectorRegister<int> vec) noexcept
    {
        constexpr static auto mask = _MM_SHUFFLE(static_cast<uint32>(W),
            static_cast<uint32>(Z), static_cast<uint32>(Y), static_cast<uint32>(X));
        return _mm_shuffle_epi32(vec, mask);
    }

    template <Swizzle X, Swizzle Y, Swizzle Z, Swizzle W>
    [[nodiscard]] NO_ODR VectorRegister<float> VECTOR_CALL VectorShuffle(VectorRegister<float> lhs, VectorRegister<float> rhs) noexcept
    {
        constexpr static auto mask = _MM_SHUFFLE(static_cast<uint32>(W),
            static_cast<uint32>(Z), static_cast<uint32>(Y), static_cast<uint32>(X));
        return _mm_shuffle_ps(lhs, rhs, mask);
    }

    template <Swizzle X, Swizzle Y, Swizzle Z, Swizzle W>
    [[nodiscard]] NO_ODR VectorRegister<int> VECTOR_CALL VectorShuffle(VectorRegister<int> lhs, VectorRegister<int> rhs) noexcept
    {
        constexpr static auto mask = _MM_SHUFFLE(static_cast<uint32>(W),
            static_cast<uint32>(Z), static_cast<uint32>(Y), static_cast<uint32>(X));
        return _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(lhs), _mm_castsi128_ps(rhs), mask));
    }

    [[nodiscard]] NO_ODR VectorRegister<float> VECTOR_CALL VectorShuffle0101(VectorRegister<float> lhs, VectorRegister<float> rhs) noexcept
    {
        return _mm_movelh_ps(lhs, rhs);
    }

    [[nodiscard]] NO_ODR VectorRegister<int> VECTOR_CALL VectorShuffle0101(VectorRegister<int> lhs, VectorRegister<int> rhs) noexcept
    {
        return _mm_castps_si128(_mm_movelh_ps(_mm_castsi128_ps(lhs), _mm_castsi128_ps(rhs)));
    }

    [[nodiscard]] NO_ODR VectorRegister<float> VECTOR_CALL VectorShuffle2323(VectorRegister<float> lhs, VectorRegister<float> rhs) noexcept
    {
        return _mm_movehl_ps(rhs, lhs);
    }

    [[nodiscard]] NO_ODR VectorRegister<int> VECTOR_CALL VectorShuffle2323(VectorRegister<int> lhs, VectorRegister<int> rhs) noexcept
    {
        return _mm_castps_si128(_mm_movehl_ps(_mm_castsi128_ps(rhs), _mm_castsi128_ps(lhs)));
    }

    [[nodiscard]] NO_ODR VectorRegister<float> VECTOR_CALL VectorAnd(VectorRegister<float> lhs, VectorRegister<float> rhs) noexcept
    {
        return _mm_and_ps(lhs, rhs);
    }

    [[nodiscard]] NO_ODR VectorRegister<int> VECTOR_CALL VectorAnd(VectorRegister<int> lhs, VectorRegister<int> rhs) noexcept
    {
        return _mm_and_si128(lhs, rhs);
    }

    [[nodiscard]] NO_ODR VectorRegister<float> VECTOR_CALL VectorOr(VectorRegister<float> lhs, VectorRegister<float> rhs) noexcept
    {
        return _mm_or_ps(lhs, rhs);
    }

    [[nodiscard]] NO_ODR VectorRegister<int> VECTOR_CALL VectorOr(VectorRegister<int> lhs, VectorRegister<int> rhs) noexcept
    {
        return _mm_or_si128(lhs, rhs);
    }

    [[nodiscard]] NO_ODR VectorRegister<float> VECTOR_CALL VectorXor(VectorRegister<float> lhs, VectorRegister<float> rhs) noexcept
    {
        return _mm_xor_ps(lhs, rhs);
    }

    [[nodiscard]] NO_ODR VectorRegister<int> VECTOR_CALL VectorXor(VectorRegister<int> lhs, VectorRegister<int> rhs) noexcept
    {
        return _mm_xor_si128(lhs, rhs);
    }

    [[nodiscard]] NO_ODR VectorRegister<float> VECTOR_CALL VectorNot(VectorRegister<float> vec) noexcept
    {
        return VectorXor(vec, One<float>);
    }

    [[nodiscard]] NO_ODR VectorRegister<int> VECTOR_CALL VectorNot(VectorRegister<int> vec) noexcept
    {
        return VectorXor(vec, One<int>);
    }

    [[nodiscard]] NO_ODR VectorRegister<float> VECTOR_CALL VectorAndNot(VectorRegister<float> lhs, VectorRegister<float> rhs) noexcept
    {
        return _mm_andnot_ps(lhs, rhs);
    }

    [[nodiscard]] NO_ODR VectorRegister<int> VECTOR_CALL VectorAndNot(VectorRegister<int> lhs, VectorRegister<int> rhs) noexcept
    {
        return _mm_andnot_si128(lhs, rhs);
    }

    [[nodiscard]] NO_ODR VectorRegister<int> VECTOR_CALL VectorSelect(VectorRegister<int> lhs, VectorRegister<int> rhs, VectorRegister<int> mask) noexcept
    {
        return VectorXor(rhs, VectorAnd(mask, VectorXor(lhs, rhs)));
    }

    [[nodiscard]] NO_ODR int VECTOR_CALL VectorMoveMask(VectorRegister<float> vec) noexcept
    {
        return _mm_movemask_ps(vec);
    }

    [[nodiscard]] NO_ODR int VECTOR_CALL VectorMoveMask(VectorRegister<int> vec) noexcept
    {
        // ToDo: Use integer specific instruction
        return _mm_movemask_ps(_mm_castsi128_ps(vec));
    }

    [[nodiscard]] NO_ODR VectorRegister<float> VECTOR_CALL VectorEqual(VectorRegister<float> lhs, VectorRegister<float> rhs) noexcept
    {
        return _mm_cmpeq_ps(lhs, rhs);
    }

    [[nodiscard]] NO_ODR VectorRegister<int> VECTOR_CALL VectorEqual(VectorRegister<int> lhs, VectorRegister<int> rhs) noexcept
    {
        return _mm_cmpeq_epi32(lhs, rhs);
    }

    [[nodiscard]] NO_ODR VectorRegister<float> VECTOR_CALL VectorNotEqual(VectorRegister<float> lhs, VectorRegister<float> rhs) noexcept
    {
        return VectorNot(VectorEqual(lhs, rhs));
    }

    [[nodiscard]] NO_ODR VectorRegister<int> VECTOR_CALL VectorNotEqual(VectorRegister<int> lhs, VectorRegister<int> rhs) noexcept
    {
        return VectorNot(VectorEqual(lhs, rhs));
    }

    [[nodiscard]] NO_ODR VectorRegister<float> VECTOR_CALL VectorGreaterThan(VectorRegister<float> lhs, VectorRegister<float> rhs) noexcept
    {
        return _mm_cmpgt_ps(lhs, rhs);
    }

    [[nodiscard]] NO_ODR VectorRegister<int> VECTOR_CALL VectorGreaterThan(VectorRegister<int> lhs, VectorRegister<int> rhs) noexcept
    {
        return _mm_cmpgt_epi32(lhs, rhs);
    }

    [[nodiscard]] NO_ODR VectorRegister<float> VECTOR_CALL VectorGreaterEqual(VectorRegister<float> lhs, VectorRegister<float> rhs) noexcept
    {
        return _mm_cmpge_ps(lhs, rhs);
    }

    [[nodiscard]] NO_ODR VectorRegister<int> VECTOR_CALL VectorGreaterEqual(VectorRegister<int> lhs, VectorRegister<int> rhs) noexcept
    {
        return VectorNot(_mm_cmplt_epi32(lhs, rhs));
    }

    [[nodiscard]] NO_ODR VectorRegister<float> VECTOR_CALL VectorLessThan(VectorRegister<float> lhs, VectorRegister<float> rhs) noexcept
    {
        return _mm_cmplt_ps(lhs, rhs);
    }

    [[nodiscard]] NO_ODR VectorRegister<int> VECTOR_CALL VectorLessThan(VectorRegister<int> lhs, VectorRegister<int> rhs) noexcept
    {
        return _mm_cmplt_epi32(lhs, rhs);
    }

    [[nodiscard]] NO_ODR VectorRegister<float> VECTOR_CALL VectorLessEqual(VectorRegister<float> lhs, VectorRegister<float> rhs) noexcept
    {
        return _mm_cmple_ps(lhs, rhs);
    }

    [[nodiscard]] NO_ODR VectorRegister<int> VECTOR_CALL VectorLessEqual(VectorRegister<int> lhs, VectorRegister<int> rhs) noexcept
    {
        return VectorNot(VectorGreaterThan(lhs, rhs));
    }

    [[nodiscard]] NO_ODR VectorRegister<float> VECTOR_CALL VectorAdd(VectorRegister<float> lhs, VectorRegister<float> rhs) noexcept
    {
        return _mm_add_ps(lhs, rhs);
    }

    [[nodiscard]] NO_ODR VectorRegister<int> VECTOR_CALL VectorAdd(VectorRegister<int> lhs, VectorRegister<int> rhs) noexcept
    {
        return _mm_add_epi32(lhs, rhs);
    }

    [[nodiscard]] NO_ODR VectorRegister<float> VECTOR_CALL VectorSubtract(VectorRegister<float> lhs, VectorRegister<float> rhs) noexcept
    {
        return _mm_sub_ps(lhs, rhs);
    }

    [[nodiscard]] NO_ODR VectorRegister<int> VECTOR_CALL VectorSubtract(VectorRegister<int> lhs, VectorRegister<int> rhs) noexcept
    {
        return _mm_sub_epi32(lhs, rhs);
    }

    [[nodiscard]] NO_ODR VectorRegister<float> VECTOR_CALL VectorMultiply(VectorRegister<float> lhs, VectorRegister<float> rhs) noexcept
    {
        return _mm_mul_ps(lhs, rhs);
    }

    [[nodiscard]] NO_ODR VectorRegister<int> VECTOR_CALL VectorMultiply(VectorRegister<int> lhs, VectorRegister<int> rhs) noexcept
    {
        const VectorRegister<int> tmp1 = _mm_mul_epu32(lhs, rhs);
        const VectorRegister<int> tmp2 = _mm_mul_epu32(_mm_srli_si128(lhs, 4), _mm_srli_si128(rhs, 4));
        return _mm_unpacklo_epi32(VectorSwizzle<Swizzle::Z, Swizzle::Y, Swizzle::Z, Swizzle::X>(tmp1), tmp2);
    }

    [[nodiscard]] NO_ODR VectorRegister<float> VECTOR_CALL VectorDivide(VectorRegister<float> lhs, VectorRegister<float> rhs) noexcept
    {
        return _mm_div_ps(lhs, rhs);
    }

    [[nodiscard]] NO_ODR VectorRegister<int> VECTOR_CALL VectorDivide(VectorRegister<int> lhs, VectorRegister<int> rhs) noexcept
    {
        // ToDo: Use integer specific instruction
        const __m128 lhsReal = _mm_cvtepi32_ps(lhs);
        const __m128 rhsReal = _mm_cvtepi32_ps(rhs);
        return _mm_cvtps_epi32(_mm_div_ps(lhsReal, rhsReal));
    }

    [[nodiscard]] NO_ODR VectorRegister<float> VECTOR_CALL VectorHadd(VectorRegister<float> lhs, VectorRegister<float> rhs) noexcept
    {
        const auto swi0 = VectorSwizzle<Swizzle::Y, Swizzle::W, Swizzle::Z, Swizzle::W>(lhs);
        const auto swi1 = VectorSwizzle<Swizzle::Y, Swizzle::W, Swizzle::Z, Swizzle::W>(rhs);
        const auto swi2 = VectorSwizzle<Swizzle::X, Swizzle::Z, Swizzle::Z, Swizzle::W>(lhs);
        const auto swi3 = VectorSwizzle<Swizzle::X, Swizzle::Z, Swizzle::Z, Swizzle::W>(rhs);
        lhs = VectorShuffle<Swizzle::X, Swizzle::Y, Swizzle::X, Swizzle::Y>(swi0, swi1);
        rhs = VectorShuffle<Swizzle::X, Swizzle::Y, Swizzle::X, Swizzle::Y>(swi2, swi3);
        return VectorAdd(lhs, rhs);
    }

    [[nodiscard]] NO_ODR VectorRegister<int> VECTOR_CALL VectorHadd(VectorRegister<int> lhs, VectorRegister<int> rhs) noexcept
    {
        const auto swi0 = VectorSwizzle<Swizzle::Y, Swizzle::W, Swizzle::Z, Swizzle::W>(lhs);
        const auto swi1 = VectorSwizzle<Swizzle::Y, Swizzle::W, Swizzle::Z, Swizzle::W>(rhs);
        const auto swi2 = VectorSwizzle<Swizzle::X, Swizzle::Z, Swizzle::Z, Swizzle::W>(lhs);
        const auto swi3 = VectorSwizzle<Swizzle::X, Swizzle::Z, Swizzle::Z, Swizzle::W>(rhs);
        lhs = VectorShuffle<Swizzle::X, Swizzle::Y, Swizzle::X, Swizzle::Y>(swi0, swi1);
        rhs = VectorShuffle<Swizzle::X, Swizzle::Y, Swizzle::X, Swizzle::Y>(swi2, swi3);
        return VectorAdd(lhs, rhs);
    }

    [[nodiscard]] NO_ODR VectorRegister<float> VECTOR_CALL VectorMin(VectorRegister<float> lhs, VectorRegister<float> rhs) noexcept
    {
        return _mm_min_ps(lhs, rhs);
    }

    [[nodiscard]] NO_ODR VectorRegister<int> VECTOR_CALL VectorMin(VectorRegister<int> lhs, VectorRegister<int> rhs) noexcept
    {
        return VectorSelect(lhs, rhs, _mm_cmplt_epi32(lhs, rhs));
    }

    [[nodiscard]] NO_ODR VectorRegister<float> VECTOR_CALL VectorMax(VectorRegister<float> lhs, VectorRegister<float> rhs) noexcept
    {
        return _mm_max_ps(lhs, rhs);
    }

    [[nodiscard]] NO_ODR VectorRegister<int> VECTOR_CALL VectorMax(VectorRegister<int> lhs, VectorRegister<int> rhs) noexcept
    {
        return VectorSelect(lhs, rhs, _mm_cmpgt_epi32(lhs, rhs));
    }

    [[nodiscard]] NO_ODR float InvSqrt(float n, size_t iterationNum) noexcept
    {
        const auto oneHalf = _mm_set_ss(0.5f);

        const auto num = _mm_set_ss(n);
        const auto halfN = _mm_mul_ss(num, oneHalf);

        auto y = _mm_rsqrt_ss(num);
        auto beforeY = y;

        for (size_t i = 0; i < iterationNum; ++i)
        {
            beforeY = y;
            y = _mm_mul_ss(y, y);
            y = _mm_sub_ss(oneHalf, _mm_mul_ss(halfN, y));
            y = _mm_add_ss(beforeY, _mm_mul_ss(beforeY, y));
        }

        float ret;
        _mm_store_ss(&ret, y);
        return ret;
    }
}