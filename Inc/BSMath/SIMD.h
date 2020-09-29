#pragma once

#include <emmintrin.h>
#include "Basic.h"

#if defined(_MSC_VER) && !defined(_M_ARM) && !defined(_M_ARM64) && !defined(_M_HYBRID_X86_ARM64) && (!_MANAGED) && (!_M_CEE) && (!defined(_M_IX86_FP) || (_M_IX86_FP > 1)) && !defined(_XM_NO_INTRINSICS_) && !defined(_XM_VECTORCALL_)
#define HAS_VECTOR_CALL
#endif

#if defined(HAS_VECTOR_CALL)
#define XM_CALLCONV __vectorcall
#elif defined(__GNUC__)
#define XM_CALLCONV
#else
#define XM_CALLCONV __fastcall
#endif

namespace BSMath::SIMD
{
	using VectorRegister = __m128;
	using IntVectorRegister = __m128i;

    [[nodiscard]] IGNORE_ODR float InvSqrt(float n, size_t iterationNum) noexcept
    {
        const VectorRegister oneHalf = _mm_set_ss(0.5f);

        VectorRegister num = _mm_set_ss(n);
        VectorRegister y = _mm_rsqrt_ss(num);
        VectorRegister halfN = _mm_mul_ss(num, oneHalf);
        VectorRegister beforeY;

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