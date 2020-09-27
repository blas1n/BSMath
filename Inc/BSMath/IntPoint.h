#pragma once

#include <emmintrin.h>
#include "Utility.h"

namespace BSMath
{
	struct IntPoint final
	{
	public: // Templates
		[[nodiscard]] constexpr static IntPoint Zero() noexcept
		{
			return IntPoint{ 0.0f };
		}

	public:
		constexpr IntPoint() : x(0), y(0) {}

		explicit constexpr IntPoint(int n)
			: x(n), y(n) {}

		explicit constexpr IntPoint(const int* ptr)
			: x(ptr[0]), y(ptr[1]) {}

		explicit constexpr IntPoint(int inX, int inY)
			: x(inX), y(inY) {}

		explicit IntPoint(const __m128i& simd)
		{
			alignas(16) int ret[4];
			_mm_store_si128(reinterpret_cast<__m128i*>(ret), simd);
			x = ret[0]; y = ret[1];
		}

		constexpr void Set(int inX, int inY) noexcept
		{
			x = inX; y = inY;
		}

		[[nodiscard]] constexpr int GetMin() const noexcept { return Min(x, y); }
		[[nodiscard]] constexpr int GetMax() const noexcept { return Max(x, y); }

		[[nodiscard]] bool operator==(const IntPoint& other) const noexcept;
		[[nodiscard]] inline bool operator!=(const IntPoint& other) const noexcept { return !(*this == other); }

		IntPoint operator-() const noexcept;

		IntPoint& operator+=(const IntPoint& other) noexcept;
		IntPoint& operator-=(const IntPoint& other) noexcept;

		IntPoint& operator*=(const IntPoint& other) noexcept;
		IntPoint& operator*=(int scaler) noexcept;

		IntPoint& operator/=(const IntPoint& other) noexcept;
		IntPoint& operator/=(int divisor) noexcept;

		[[nodiscard]] constexpr int& operator[](size_t idx) noexcept { return (&x)[idx]; }
		[[nodiscard]] constexpr int operator[](size_t idx) const noexcept { return (&x)[idx]; }

	public:
		int x;
		int y;
	};

	bool IntPoint::operator==(const IntPoint& other) const noexcept
	{
		const __m128 lhs = _mm_set_epi32(0, 0, y, x);
		const __m128 rhs = _mm_set_epi32(0, 0, other.y, other.x);
		const auto val = _mm_movemask_ps(_mm_cmpeq_ps(lhs, rhs));
		return val == 15;
	}

	[[nodiscard]] IntPoint IntPoint::operator-() const noexcept
	{
		const __m128i zero = _mm_setzero_si128();
		const __m128i vec = _mm_set_epi32(0, 0, y, x);
		return IntPoint{ _mm_sub_epi32(zero, vec) };
	}

	IntPoint& IntPoint::operator+=(const IntPoint& other) noexcept
	{
		const __m128i lhs = _mm_set_epi32(0, 0, y, x);
		const __m128i rhs = _mm_set_epi32(0, 0, other.y, other.x);
		return *this = IntPoint{ _mm_add_epi32(lhs, rhs) };
	}

	IntPoint& IntPoint::operator-=(const IntPoint& other) noexcept
	{
		const __m128i lhs = _mm_set_epi32(0, 0, y, x);
		const __m128i rhs = _mm_set_epi32(0, 0, other.y, other.x);
		return *this = IntPoint{ _mm_sub_epi32(lhs, rhs) };
	}

	IntPoint& IntPoint::operator*=(const IntPoint& other) noexcept
	{
		const __m128i operand = _mm_set_epi32(0, other.y, 0, other.x);
		__m128i point = _mm_set_epi32(0, y, 0, x);
		point = _mm_mul_epu32(point, operand);
		return *this = IntPoint{ _mm_shuffle_epi32(point, _MM_SHUFFLE(3, 1, 2, 0)) };
	}

	inline IntPoint& IntPoint::operator*=(int scaler) noexcept
	{
		return *this *= IntPoint{ scaler };
	}

	// ToDo: Use integer specific instruction
	IntPoint& IntPoint::operator/=(const IntPoint& other) noexcept
	{
		const __m128 lhs = _mm_set_ps(0.0f, 0.0f, y, x);
		const __m128 rhs = _mm_set_ps(0.0f, 0.0f, other.y, other.x);
		return *this = IntPoint{ _mm_castps_si128(_mm_div_ps(lhs, rhs)) };
	}

	IntPoint& IntPoint::operator/=(int divisor) noexcept
	{
		return *this /= {divisor};
	}

	[[nodiscard]] inline IntPoint operator+(const IntPoint& lhs, const IntPoint& rhs) noexcept
	{
		return IntPoint{ lhs } += rhs;
	}

	[[nodiscard]] inline IntPoint operator-(const IntPoint& lhs, const IntPoint& rhs) noexcept
	{
		return IntPoint{ lhs } -= rhs;
	}

	[[nodiscard]] inline IntPoint operator*(const IntPoint& lhs, const IntPoint& rhs) noexcept
	{
		return IntPoint{ lhs } *= rhs;
	}

	[[nodiscard]] inline IntPoint operator*(const IntPoint& vec, int scaler) noexcept
	{
		return IntPoint{ vec } *= scaler;
	}

	[[nodiscard]] inline IntPoint operator*(int scaler, const IntPoint& vec) noexcept
	{
		return IntPoint{ vec } *= scaler;
	}

	[[nodiscard]] inline IntPoint operator/(const IntPoint& lhs, const IntPoint& rhs) noexcept
	{
		return IntPoint{ lhs } /= rhs;
	}

	[[nodiscard]] inline IntPoint operator/(const IntPoint& vec, int divisor) noexcept
	{
		return IntPoint{ vec } /= divisor;
	}

	template <>
	[[nodiscard]] auto Min<IntPoint>(const IntPoint& lhs, const IntPoint& rhs) noexcept
	{
		const __m128i lhsSimd = _mm_set_epi32(0, 0, lhs.y, lhs.x);
		const __m128i rhsSimd = _mm_set_epi32(0, 0, rhs.y, rhs.x);
		const __m128i mask = _mm_cmplt_epi32(lhsSimd, rhsSimd);
		return IntPoint{ _mm_xor_si128(rhsSimd, _mm_and_si128(mask, _mm_xor_si128(lhsSimd, rhsSimd))) };
	}

	template <>
	[[nodiscard]] auto Max<IntPoint>(const IntPoint& lhs, const IntPoint& rhs) noexcept
	{
		const __m128i lhsSimd = _mm_set_epi32(0, 0, lhs.y, lhs.x);
		const __m128i rhsSimd = _mm_set_epi32(0, 0, rhs.y, rhs.x);
		const __m128i mask = _mm_cmpgt_epi32(lhsSimd, rhsSimd);
		return IntPoint{ _mm_xor_si128(rhsSimd, _mm_and_si128(mask, _mm_xor_si128(lhsSimd, rhsSimd))) };
	}

	template <>
	[[nodiscard]] auto Clamp<IntPoint>(const IntPoint& n, const IntPoint& min, const IntPoint& max) noexcept
	{
		IntPoint realMin = Min(min, max);
		IntPoint realMax = Max(min, max);
		return Max(min, Min(max, n));
	}

	template <>
	[[nodiscard]] IntPoint Abs<IntPoint>(const IntPoint& n) noexcept
	{
		__m128i point = _mm_set_epi32(0, 0, n.y, n.x);
		__m128i mask = _mm_cmplt_epi32(point, _mm_setzero_si128());
		point = _mm_xor_si128(point, mask);
		mask = _mm_and_si128(mask, _mm_set1_epi32(1));
		return IntPoint{ _mm_add_epi32(point, mask) };
	}

	template <>
	[[nodiscard]] IntPoint Sign<IntPoint>(const IntPoint& n) noexcept
	{
		const __m128i zero = _mm_setzero_si128();
		const __m128i simd = _mm_set_epi32(0, 0, n.y, n.x);
		const __m128i positive = _mm_and_si128(_mm_cmpgt_epi32(simd, zero), _mm_set1_epi32(1.0f));
		const __m128i negative = _mm_and_si128(_mm_cmplt_epi32(simd, zero), _mm_set1_epi32(-1.0f));
		return IntPoint{ _mm_or_si128(positive, negative) };
	}
}