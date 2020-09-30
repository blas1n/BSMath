#pragma once

#include <emmintrin.h>
#include "Basic.h"

#if defined(_MSC_VER) && !defined(_M_ARM) && !defined(_M_ARM64) && !defined(_M_HYBRID_X86_ARM64) && (!_MANAGED) && (!_M_CEE) && (!defined(_M_IX86_FP) || (_M_IX86_FP > 1)) && !defined(_XM_NO_INTRINSICS_) && !defined(_XM_VECTORCALL_)
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
    namespace Integer
    {
        using VectorRegister = __m128i;

        const static VectorRegister Zero = _mm_setzero_si128();

        [[nodiscard]] NO_ODR VectorRegister VectorLoad(int x = 0.0f, int y = 0.0f, int z = 0.0f, int w = 0.0f) noexcept
        {
            return _mm_setr_epi32(x, y, z, w);
        }

        [[nodiscard]] NO_ODR VectorRegister VectorLoad1(int n) noexcept
        {
            return _mm_set1_epi32(n);
        }

        [[nodiscard]] NO_ODR void VectorStore(VectorRegister vec, int* ptr) noexcept
        {
            _mm_store_si128(reinterpret_cast<VectorRegister*>(ptr), vec);
        }

        [[nodiscard]] NO_ODR int VectorStore1(VectorRegister vec) noexcept
        {
            return _mm_cvtsi128_si32(vec);
        }

        [[nodiscard]] NO_ODR VectorRegister VECTOR_CALL VectorAdd(VectorRegister lhs, VectorRegister rhs) noexcept
        {
            return _mm_add_epi32(lhs, rhs);
        }

        [[nodiscard]] NO_ODR VectorRegister VECTOR_CALL VectorSubtract(VectorRegister lhs, VectorRegister rhs) noexcept
        {
            return _mm_sub_epi32(lhs, rhs);
        }

        [[nodiscard]] NO_ODR VectorRegister VECTOR_CALL VectorMultiply(VectorRegister lhs, VectorRegister rhs) noexcept
        {
            const VectorRegister tmp1 = _mm_mul_epu32(lhs, rhs);
            const VectorRegister tmp2 = _mm_mul_epu32(_mm_srli_si128(lhs, 4), _mm_srli_si128(rhs, 4));
            return _mm_unpacklo_epi32(_mm_shuffle_epi32(tmp1, _MM_SHUFFLE(3, 1, 2, 0)), tmp2);
        }

        [[nodiscard]] NO_ODR VectorRegister VECTOR_CALL VectorDivide(VectorRegister lhs, VectorRegister rhs) noexcept
        {
            // ToDo: Use integer specific instruction
            const __m128 lhsReal = _mm_cvtepi32_ps(lhs);
            const __m128 rhsReal = _mm_cvtepi32_ps(rhs);
            return _mm_cvtps_epi32(_mm_div_ps(lhsReal, rhsReal));
        }
    }

    namespace Real
    {
        using VectorRegister = __m128;

        [[nodiscard]] NO_ODR VectorRegister VectorLoad(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f) noexcept
        {
            return _mm_setr_ps(x, y, z, w);
        }

        [[nodiscard]] NO_ODR VectorRegister VectorLoad1(float n) noexcept
        {
            return _mm_set1_ps(n);
        }

        [[nodiscard]] NO_ODR void VectorStore(VectorRegister vec, float* ptr) noexcept
        {
            _mm_store_ps(ptr, vec);
        }

        [[nodiscard]] NO_ODR float VectorStore1(VectorRegister vec) noexcept
        {
            float ret;
            _mm_store_ps1(&ret, vec);
            return ret;
        }

        [[nodiscard]] NO_ODR VectorRegister VECTOR_CALL VectorAdd(VectorRegister lhs, VectorRegister rhs) noexcept
        {
            return _mm_add_ps(lhs, rhs);
        }

        [[nodiscard]] NO_ODR VectorRegister VECTOR_CALL VectorSubtract(VectorRegister lhs, VectorRegister rhs) noexcept
        {
            return _mm_sub_ps(lhs, rhs);
        }

        [[nodiscard]] NO_ODR VectorRegister VECTOR_CALL VectorMultiply(VectorRegister lhs, VectorRegister rhs) noexcept
        {
            return _mm_mul_ps(lhs, rhs);
        }

        [[nodiscard]] NO_ODR VectorRegister VECTOR_CALL VectorDivide(VectorRegister lhs, VectorRegister rhs) noexcept
        {
            return _mm_div_ps(lhs, rhs);
        }
    }

    [[nodiscard]] NO_ODR float InvSqrt(float n, size_t iterationNum) noexcept
    {
        const Real::VectorRegister oneHalf = _mm_set_ss(0.5f);

        Real::VectorRegister num = _mm_set_ss(n);
        Real::VectorRegister y = _mm_rsqrt_ss(num);
        Real::VectorRegister halfN = _mm_mul_ss(num, oneHalf);
        Real::VectorRegister beforeY;

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