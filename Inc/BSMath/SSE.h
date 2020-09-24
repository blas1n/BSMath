#pragma once

#include <smmintrin.h>

namespace BSMath::SSE
{
	[[nodiscard]] inline int Trunc(float n) noexcept
	{
		return _mm_cvtt_ss2si(_mm_set_ss(n));
	}

	[[nodiscard]] inline int Floor(float n) noexcept
	{
		return _mm_cvt_ss2si(_mm_set_ss(n + n - 0.5f)) >> 1;
	}

	[[nodiscard]] inline int Round(float n) noexcept
	{
		return _mm_cvt_ss2si(_mm_set_ss(n + n + 0.5f)) >> 1;
	}

	[[nodiscard]] inline int Ceil(float n) noexcept
	{
		return -(_mm_cvt_ss2si(_mm_set_ss(-0.5f - (n + n))) >> 1);
	}

	[[nodiscard]] static float InvSqrt(float n, size_t iterationNum) noexcept
	{
		const __m128 oneHalf = _mm_set_ss(0.5f);

		__m128 num = _mm_set_ss(n);
		__m128 y = _mm_rsqrt_ss(num);
		__m128 halfN = _mm_mul_ss(num, oneHalf);
		__m128 beforeY;
		
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