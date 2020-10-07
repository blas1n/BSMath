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
        const static VectorRegister One = _mm_set1_epi32(1);

        [[nodiscard]] NO_ODR VectorRegister VectorLoad(int x = 0, int y = 0, int z = 0, int w = 0) noexcept
        {
            return _mm_setr_epi32(x, y, z, w);
        }

        template <size_t L>
        [[nodiscard]] NO_ODR VectorRegister VectorLoad(const int(&vec)[L]) noexcept
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

        [[nodiscard]] NO_ODR VectorRegister VectorLoad1(int n) noexcept
        {
            return _mm_set1_epi32(n);
        }

        NO_ODR void VectorStore(VectorRegister vec, int* ptr) noexcept
        {
            _mm_store_si128(reinterpret_cast<VectorRegister*>(ptr), vec);
        }

        [[nodiscard]] NO_ODR int VectorStore1(VectorRegister vec) noexcept
        {
            return _mm_cvtsi128_si32(vec);
        }

        [[nodiscard]] NO_ODR VectorRegister VECTOR_CALL VectorAnd(VectorRegister lhs, VectorRegister rhs) noexcept
        {
            return _mm_and_si128(lhs, rhs);
        }

        [[nodiscard]] NO_ODR VectorRegister VECTOR_CALL VectorOr(VectorRegister lhs, VectorRegister rhs) noexcept
        {
            return _mm_or_si128(lhs, rhs);
        }

        [[nodiscard]] NO_ODR VectorRegister VECTOR_CALL VectorXor(VectorRegister lhs, VectorRegister rhs) noexcept
        {
            return _mm_xor_si128(lhs, rhs);
        }

        [[nodiscard]] NO_ODR VectorRegister VECTOR_CALL VectorNot(VectorRegister vec) noexcept
        {
            return VectorXor(vec, One);
        }

        [[nodiscard]] NO_ODR VectorRegister VECTOR_CALL VectorAndNot(VectorRegister lhs, VectorRegister rhs) noexcept
        {
            return _mm_andnot_si128(lhs, rhs);
        }

        [[nodiscard]] NO_ODR VectorRegister VECTOR_CALL VectorSelect(VectorRegister lhs, VectorRegister rhs, VectorRegister mask) noexcept
        {
            return VectorXor(rhs, VectorAnd(mask, VectorXor(lhs, rhs)));
        }

        [[nodiscard]] NO_ODR int VECTOR_CALL VectorMoveMask(VectorRegister vec) noexcept
        {
            // ToDo: Use integer specific instruction
            return _mm_movemask_ps(_mm_castsi128_ps(vec));
        }

        [[nodiscard]] NO_ODR VectorRegister VECTOR_CALL VectorEqual(VectorRegister lhs, VectorRegister rhs) noexcept
        {
            return _mm_cmpeq_epi32(lhs, rhs);
        }

        [[nodiscard]] NO_ODR VectorRegister VECTOR_CALL VectorNotEqual(VectorRegister lhs, VectorRegister rhs) noexcept
        {
            return VectorNot(VectorEqual(lhs, rhs));
        }

        [[nodiscard]] NO_ODR VectorRegister VECTOR_CALL VectorGreaterThan(VectorRegister lhs, VectorRegister rhs) noexcept
        {
            return _mm_cmpgt_epi32(lhs, rhs);
        }

        [[nodiscard]] NO_ODR VectorRegister VECTOR_CALL VectorGreaterEqual(VectorRegister lhs, VectorRegister rhs) noexcept
        {
            return VectorNot(_mm_cmplt_epi32(lhs, rhs));
        }

        [[nodiscard]] NO_ODR VectorRegister VECTOR_CALL VectorLessThan(VectorRegister lhs, VectorRegister rhs) noexcept
        {
            return _mm_cmplt_epi32(lhs, rhs);
        }

        [[nodiscard]] NO_ODR VectorRegister VECTOR_CALL VectorLessEqual(VectorRegister lhs, VectorRegister rhs) noexcept
        {
            return VectorNot(VectorGreaterThan(lhs, rhs));
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

        [[nodiscard]] NO_ODR VectorRegister VECTOR_CALL VectorMin(VectorRegister lhs, VectorRegister rhs) noexcept
        {
            return VectorSelect(lhs, rhs, _mm_cmplt_epi32(lhs, rhs));
        }

        [[nodiscard]] NO_ODR VectorRegister VECTOR_CALL VectorMax(VectorRegister lhs, VectorRegister rhs) noexcept
        {
            return VectorSelect(lhs, rhs, _mm_cmpgt_epi32(lhs, rhs));
        }
    }

    namespace Real
    {
        using VectorRegister = __m128;

        const static VectorRegister Zero = _mm_setzero_ps();
        const static VectorRegister One = _mm_set1_ps(1.0f);

        [[nodiscard]] NO_ODR VectorRegister VectorLoad(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f) noexcept
        {
            return _mm_setr_ps(x, y, z, w);
        }

        template <size_t L>
        [[nodiscard]] NO_ODR VectorRegister VectorLoad(const float(&vec)[L]) noexcept
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

        [[nodiscard]] NO_ODR VectorRegister VectorLoad1(float n) noexcept
        {
            return _mm_set1_ps(n);
        }

        NO_ODR void VectorStore(VectorRegister vec, float* ptr) noexcept
        {
            _mm_store_ps(ptr, vec);
        }

        [[nodiscard]] NO_ODR float VectorStore1(VectorRegister vec) noexcept
        {
            float ret;
            _mm_store_ps1(&ret, vec);
            return ret;
        }

        [[nodiscard]] NO_ODR int VECTOR_CALL VectorMoveMask(VectorRegister vec) noexcept
        {
            return _mm_movemask_ps(vec);
        }

        [[nodiscard]] NO_ODR VectorRegister VECTOR_CALL VectorAnd(VectorRegister lhs, VectorRegister rhs) noexcept
        {
            return _mm_and_ps(lhs, rhs);
        }

        [[nodiscard]] NO_ODR VectorRegister VECTOR_CALL VectorOr(VectorRegister lhs, VectorRegister rhs) noexcept
        {
            return _mm_or_ps(lhs, rhs);
        }

        [[nodiscard]] NO_ODR VectorRegister VECTOR_CALL VectorXor(VectorRegister lhs, VectorRegister rhs) noexcept
        {
            return _mm_xor_ps(lhs, rhs);
        }

        [[nodiscard]] NO_ODR VectorRegister VECTOR_CALL VectorNot(VectorRegister vec) noexcept
        {
            return VectorXor(vec, One);
        }

        [[nodiscard]] NO_ODR VectorRegister VECTOR_CALL VectorAndNot(VectorRegister lhs, VectorRegister rhs) noexcept
        {
            return _mm_andnot_ps(lhs, rhs);
        }

        [[nodiscard]] NO_ODR VectorRegister VECTOR_CALL VectorEqual(VectorRegister lhs, VectorRegister rhs) noexcept
        {
            return _mm_cmpeq_ps(lhs, rhs);
        }

        [[nodiscard]] NO_ODR VectorRegister VECTOR_CALL VectorNotEqual(VectorRegister lhs, VectorRegister rhs) noexcept
        {
            return VectorNot(VectorEqual(lhs, rhs));
        }

        [[nodiscard]] NO_ODR VectorRegister VECTOR_CALL VectorGreaterThan(VectorRegister lhs, VectorRegister rhs) noexcept
        {
            return _mm_cmpgt_ps(lhs, rhs);
        }

        [[nodiscard]] NO_ODR VectorRegister VECTOR_CALL VectorGreaterEqual(VectorRegister lhs, VectorRegister rhs) noexcept
        {
            return _mm_cmpge_ps(lhs, rhs);
        }

        [[nodiscard]] NO_ODR VectorRegister VECTOR_CALL VectorLessThan(VectorRegister lhs, VectorRegister rhs) noexcept
        {
            return _mm_cmplt_ps(lhs, rhs);
        }

        [[nodiscard]] NO_ODR VectorRegister VECTOR_CALL VectorLessEqual(VectorRegister lhs, VectorRegister rhs) noexcept
        {
            return _mm_cmple_ps(lhs, rhs);
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

        [[nodiscard]] NO_ODR VectorRegister VECTOR_CALL VectorMin(VectorRegister lhs, VectorRegister rhs) noexcept
        {
            return _mm_min_ps(lhs, rhs);
        }

        [[nodiscard]] NO_ODR VectorRegister VECTOR_CALL VectorMax(VectorRegister lhs, VectorRegister rhs) noexcept
        {
            return _mm_max_ps(lhs, rhs);
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

#define INVOKE_SIMD(T, ...) \
if constexpr (std::is_floating_point_v<T>) \
{ \
	using namespace SIMD::Real; \
	__VA_ARGS__ \
} \
else \
{ \
	using namespace SIMD::Integer; \
	__VA_ARGS__ \
}
