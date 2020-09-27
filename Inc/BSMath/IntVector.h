#pragma once

#include "IntPoint.h"

namespace BSMath
{
	struct IntVector final
	{
	public: // Templates
		[[nodiscard]] constexpr static IntVector Zero() noexcept
		{
			return IntVector{ 0 };
		}

		[[nodiscard]] constexpr static IntVector One() noexcept
		{
			return IntVector{ 1 };
		}

		[[nodiscard]] constexpr static IntVector Right() noexcept
		{
			return IntVector{ 1, 0, 0 };
		}

		[[nodiscard]] constexpr static IntVector Left() noexcept
		{
			return IntVector{ -1, 0, 0 };
		}

		[[nodiscard]] constexpr static IntVector Up() noexcept
		{
			return IntVector{ 0, 1, 0 };
		}

		[[nodiscard]] constexpr static IntVector Down() noexcept
		{
			return IntVector{ 0, -1, 0 };
		}

		[[nodiscard]] constexpr static IntVector Forward() noexcept
		{
			return IntVector{ 0, 0, 1 };
		}

		[[nodiscard]] constexpr static IntVector Backward() noexcept
		{
			return IntVector{ 0, 0, -1 };
		}

	public:
		constexpr IntVector() : x(0), y(0), z(0) {}

		explicit constexpr IntVector(int n)
			: x(n), y(n), z(n) {}

		explicit constexpr IntVector(const int* ptr)
			: x(ptr[0]), y(ptr[1]), z(ptr[2]) {}

		explicit constexpr IntVector(int inX, int inY, int inZ)
			: x(inX), y(inY), z(inZ) {}

		explicit constexpr IntVector(const IntVector& point, int inZ = 0)
			: x(point.x), y(point,y), z(inZ) {}

		explicit IntVector(const __m128i& simd)
		{
			alignas(16) int ret[4];
			_mm_store_si128(reinterpret_cast<__m128i*>(ret), simd);
			x = ret[0]; y = ret[1]; y = ret[2];
		}

		constexpr void Set(int inX, int inY, int inZ) noexcept
		{
			x = inX; y = inY; z = inZ;
		}

		[[nodiscard]] constexpr int GetMin() const noexcept { return Min(x, Min(y, z)); }
		[[nodiscard]] constexpr int GetMax() const noexcept { return Max(x, Max(y, z)); }

		[[nodiscard]] bool operator==(const IntVector& other) const noexcept;
		[[nodiscard]] inline bool operator!=(const IntVector& other) const noexcept { return !(*this == other); }

		IntVector operator-() const noexcept;

		IntVector& operator+=(const IntVector& other) noexcept;
		IntVector& operator-=(const IntVector& other) noexcept;

		IntVector& operator*=(const IntVector& other) noexcept;
		IntVector& operator*=(int scaler) noexcept;

		IntVector& operator/=(const IntVector& other) noexcept;
		IntVector& operator/=(int divisor) noexcept;

		IntVector& operator^=(const IntVector& other) noexcept;

		[[nodiscard]] constexpr int& operator[](size_t idx) noexcept { return (&x)[idx]; }
		[[nodiscard]] constexpr int operator[](size_t idx) const noexcept { return (&x)[idx]; }

		[[nodiscard]] static int Dot(const IntVector& lhs, const IntVector& rhs) noexcept;
		[[nodiscard]] static IntVector Cross(const IntVector& lhs, const IntVector& rhs) noexcept;

	public:
		int x;
		int y;
		int z;
	};

	bool IntVector::operator==(const IntVector& other) const noexcept
	{
		const __m128i lhs = _mm_set_epi32(0, z, y, x);
		const __m128i rhs = _mm_set_epi32(0, other.z, other.y, other.x);
		const auto val = _mm_movemask_epi8((_mm_cmpeq_epi32(lhs, rhs)));
		return val == 0xFFFF;
	}

	[[nodiscard]] IntVector IntVector::operator-() const noexcept
	{
		const __m128i zero = _mm_setzero_si128();
		const __m128i vec = _mm_set_epi32(0, z, y, x);
		return IntVector{ _mm_sub_epi32(zero, vec) };
	}

	IntVector& IntVector::operator+=(const IntVector& other) noexcept
	{
		const __m128i lhs = _mm_set_epi32(0, z, y, x);
		const __m128i rhs = _mm_set_epi32(0, other.z, other.y, other.x);
		return *this = IntVector{ _mm_add_epi32(lhs, rhs) };
	}

	IntVector& IntVector::operator-=(const IntVector& other) noexcept
	{
		const __m128i lhs = _mm_set_epi32(0, z, y, x);
		const __m128i rhs = _mm_set_epi32(0, other.z, other.y, other.x);
		return *this = IntVector{ _mm_sub_epi32(lhs, rhs) };
	}

	IntVector& IntVector::operator*=(const IntVector& other) noexcept
	{
		const __m128i lhsSimd = _mm_set_epi32(0, y, z, x);
		const __m128i rhsSimd = _mm_set_epi32(0, other.y, other.z, other.x);
		const __m128i tmp1 = _mm_mul_epu32(lhsSimd, rhsSimd);
		const __m128i tmp2 = _mm_mul_epu32(_mm_srli_si128(lhsSimd, 8), _mm_srli_si128(rhsSimd, 8));
		return _mm_unpacklo_epi32(tmp1, tmp2);
	}

	inline IntVector& IntVector::operator*=(int scaler) noexcept
	{
		return *this *= IntVector{ scaler };
	}

	// ToDo: Use integer specific instruction
	IntVector& IntVector::operator/=(const IntVector& other) noexcept
	{
		const __m128 lhs = _mm_set_ps(0.0f, static_cast<float>(z),
			static_cast<float>(y), static_cast<float>(x));

		const __m128 rhs = _mm_set_ps(0.0f, static_cast<float>(other.z),
			static_cast<float>(other.y), static_cast<float>(other.x));

		return *this = IntVector{ _mm_cvtps_epi32(_mm_div_ps(lhs, rhs)) };
	}

	IntVector& IntVector::operator/=(int divisor) noexcept
	{
		return *this /= IntVector{ divisor };
	}

	[[nodiscard]] inline IntVector operator+(const IntVector& lhs, const IntVector& rhs) noexcept
	{
		return IntVector{ lhs } += rhs;
	}

	[[nodiscard]] inline IntVector operator-(const IntVector& lhs, const IntVector& rhs) noexcept
	{
		return IntVector{ lhs } -= rhs;
	}

	[[nodiscard]] inline IntVector operator*(const IntVector& lhs, const IntVector& rhs) noexcept
	{
		return IntVector{ lhs } *= rhs;
	}

	[[nodiscard]] inline IntVector operator*(const IntVector& vec, int scaler) noexcept
	{
		return IntVector{ vec } *= scaler;
	}

	[[nodiscard]] inline IntVector operator*(int scaler, const IntVector& vec) noexcept
	{
		return IntVector{ vec } *= scaler;
	}

	[[nodiscard]] inline IntVector operator/(const IntVector& lhs, const IntVector& rhs) noexcept
	{
		return IntVector{ lhs } /= rhs;
	}

	[[nodiscard]] inline IntVector operator/(const IntVector& vec, int divisor) noexcept
	{
		return IntVector{ vec } /= divisor;
	}

	[[nodiscard]] inline int operator|(const IntVector& lhs, const IntVector& rhs) noexcept
	{
		auto mul = lhs * rhs;
		return mul.x + mul.y;
	}

	[[nodiscard]] inline IntVector operator^(const IntVector& lhs, const IntVector& rhs) noexcept
	{
		return IntVector{ lhs } ^= rhs;
	}

	IntVector& IntVector::operator^=(const IntVector& other) noexcept
	{
		auto ret1 = IntVector{ y, z, x } *IntVector{ other.z, other.x, other.y };
		auto ret2 = IntVector{ z, x, y } *IntVector{ other.y, other.z, other.x };
		return *this = ret1 - ret2;
	}

	inline int IntVector::Dot(const IntVector& lhs, const IntVector& rhs) noexcept { return lhs | rhs; }
	inline IntVector IntVector::Cross(const IntVector& lhs, const IntVector& rhs) noexcept { return lhs ^ rhs; }

	template <>
	[[nodiscard]] auto Min<IntVector>(const IntVector& lhs, const IntVector& rhs) noexcept
	{
		const __m128i lhsSimd = _mm_set_epi32(0, lhs.z, lhs.y, lhs.x);
		const __m128i rhsSimd = _mm_set_epi32(0, rhs.z, rhs.y, rhs.x);
		const __m128i mask = _mm_cmplt_epi32(lhsSimd, rhsSimd);
		return IntVector{ _mm_xor_si128(rhsSimd, _mm_and_si128(mask, _mm_xor_si128(lhsSimd, rhsSimd))) };
	}

	template <>
	[[nodiscard]] auto Max<IntVector>(const IntVector& lhs, const IntVector& rhs) noexcept
	{
		const __m128i lhsSimd = _mm_set_epi32(0, lhs.z, lhs.y, lhs.x);
		const __m128i rhsSimd = _mm_set_epi32(0, rhs.z , rhs.y, rhs.x);
		const __m128i mask = _mm_cmpgt_epi32(lhsSimd, rhsSimd);
		return IntVector{ _mm_xor_si128(rhsSimd, _mm_and_si128(mask, _mm_xor_si128(lhsSimd, rhsSimd))) };
	}

	template <>
	[[nodiscard]] auto Clamp<IntVector>(const IntVector& n, const IntVector& min, const IntVector& max) noexcept
	{
		IntVector realMin = Min(min, max);
		IntVector realMax = Max(min, max);
		return Max(realMin, Min(realMax, n));
	}

	template <>
	[[nodiscard]] IntVector Abs<IntVector>(const IntVector& n) noexcept
	{
		__m128i point = _mm_set_epi32(0, n.z, n.y, n.x);
		__m128i mask = _mm_cmplt_epi32(point, _mm_setzero_si128());
		point = _mm_xor_si128(point, mask);
		mask = _mm_and_si128(mask, _mm_set1_epi32(1));
		return IntVector{ _mm_add_epi32(point, mask) };
	}

	template <>
	[[nodiscard]] IntVector Sign<IntVector>(const IntVector& n) noexcept
	{
		const __m128i zero = _mm_setzero_si128();
		const __m128i simd = _mm_set_epi32(0, n.z, n.y, n.x);
		const __m128i positive = _mm_and_si128(_mm_cmpgt_epi32(simd, zero), _mm_set1_epi32(1));
		const __m128i negative = _mm_and_si128(_mm_cmplt_epi32(simd, zero), _mm_set1_epi32(-1));
		return IntVector{ _mm_or_si128(positive, negative) };
	}

	// IntPoint's Constructor
	IntPoint::IntPoint(const IntVector& vec)
		: x(vec.x), y(vec.y) {}
}