#pragma once

#include "IntPoint.h"

namespace BSMath
{
	// All vector's helper function
	namespace Detail
	{
		[[nodiscard]] inline bool IsLessEpsilon(const __m128& vec, float epsilon)
		{
			const __m128 tolerance = _mm_set_ps1(epsilon);
			return _mm_movemask_ps(_mm_cmple_ps(vec, tolerance)) == 0xF;
		}
	}

	struct Vector2 final
	{
	public: // Templates
		[[nodiscard]] constexpr static Vector2 Zero() noexcept
		{
			return Vector2{ 0.0f };
		}

		[[nodiscard]] constexpr static Vector2 One() noexcept
		{
			return Vector2{ 1.0f };
		}

		[[nodiscard]] constexpr static Vector2 Right() noexcept
		{
			return Vector2{ 1.0f, 0.0f };
		}

		[[nodiscard]] constexpr static Vector2 Left() noexcept
		{
			return Vector2{ -1.0f, 0.0f };
		}

		[[nodiscard]] constexpr static Vector2 Up() noexcept
		{
			return Vector2{ 0.0f, 1.0f };
		}

		[[nodiscard]] constexpr static Vector2 Down() noexcept
		{
			return Vector2{ 0.0f, -1.0f };
		}

	public:
		constexpr Vector2() : x(0.0f), y(0.0f) {}

		explicit constexpr Vector2(float n)
			: x(n), y(n) {}

		explicit constexpr Vector2(const float* ptr)
			: x(ptr[0]), y(ptr[1]) {}

		explicit constexpr Vector2(float inX, float inY)
			: x(inX), y(inY) {}

		explicit Vector2(const __m128& simd)
		{
			alignas(16) float ret[4];
			_mm_store_ps(ret, simd);
			x = ret[0]; y = ret[1];
		}

		explicit constexpr Vector2(const struct Vector3& vec3);
		explicit constexpr Vector2(const struct Vector4& vec4);

		explicit constexpr Vector2(const IntPoint& point)
			: x(static_cast<float>(point.x)), y(static_cast<float>(point.y)) {}

		constexpr void Set(float inX, float inY) noexcept
		{
			x = inX; y = inY;
		}

		[[nodiscard]] inline float Length() const noexcept
		{
			return Sqrt(LengthSquared());
		}

		[[nodiscard]] float LengthSquared() const noexcept;

		[[nodiscard]] inline Vector2 GetNormal() const noexcept
		{
			Vector2 ret = *this;
			return ret.Normalize() ? ret : Zero();
		}

		bool Normalize() noexcept;

		[[nodiscard]] static inline float Distance(const Vector2& lhs, const Vector2& rhs)
		{
			return Sqrt(DistanceSquared(lhs, rhs));
		}

		[[nodiscard]] static float DistanceSquared(const Vector2& lhs, const Vector2& rhs);

		[[nodiscard]] constexpr float GetMin() const noexcept { return Min(x, y); }
		[[nodiscard]] constexpr float GetMax() const noexcept { return Max(x, y); }

		[[nodiscard]] bool operator==(const Vector2& other) const noexcept;
		[[nodiscard]] inline bool operator!=(const Vector2& other) const noexcept { return !(*this == other); }

		Vector2 operator-() const noexcept;

		Vector2& operator+=(const Vector2& other) noexcept;
		Vector2& operator-=(const Vector2& other) noexcept;

		Vector2& operator*=(const Vector2& other) noexcept;
		Vector2& operator*=(float scaler) noexcept;

		Vector2& operator/=(const Vector2& other) noexcept;
		Vector2& operator/=(float divisor) noexcept;

		[[nodiscard]] constexpr float& operator[](size_t idx) noexcept { return (&x)[idx]; }
		[[nodiscard]] constexpr float operator[](size_t idx) const noexcept { return (&x)[idx]; }

		[[nodiscard]] static float Dot(const Vector2& lhs, const Vector2& rhs) noexcept;

	public:
		float x;
		float y;
	};

	inline float Vector2::LengthSquared() const noexcept
	{
		const __m128 vec = _mm_set_ps(0.0f, 0.0f, y, x);
		Vector2 ret{ _mm_mul_ps(vec, vec) };
		return ret.x + ret.y;
	}

	inline bool Vector2::Normalize() noexcept
	{
		const float lengthSquared = LengthSquared();
		if (IsNearlyZero(lengthSquared))
			return false;

		__m128 vec = _mm_set_ps(0.0f, 0.0f, y, x);
		const __m128 size = _mm_set_ps1(InvSqrt(lengthSquared));
		*this = Vector2{ _mm_mul_ps(vec, size) };
		return true;
	}

	inline bool Vector2::operator==(const Vector2& other) const noexcept
	{
		const __m128 lhs = _mm_set_ps(0.0f, 0.0f, y, x);
		const __m128 rhs = _mm_set_ps(0.0f, 0.0f, other.y, other.x);
		return _mm_movemask_ps(_mm_cmpeq_ps(lhs, rhs)) == 0xF;
	}

	[[nodiscard]] inline Vector2 Vector2::operator-() const noexcept
	{
		const __m128 zero = _mm_setzero_ps();
		const __m128 vec = _mm_set_ps(0.0f, 0.0f, y, x);
		return Vector2{ _mm_sub_ps(zero, vec) };
	}

	inline Vector2& Vector2::operator+=(const Vector2& other) noexcept
	{
		const __m128 lhs = _mm_set_ps(0.0f, 0.0f, y, x);
		const __m128 rhs = _mm_set_ps(0.0f, 0.0f, other.y, other.x);
		return *this = Vector2{ _mm_add_ps(lhs, rhs) };
	}

	inline Vector2& Vector2::operator-=(const Vector2& other) noexcept
	{
		const __m128 lhs = _mm_set_ps(0.0f, 0.0f, y, x);
		const __m128 rhs = _mm_set_ps(0.0f, 0.0f, other.y, other.x);
		return *this = Vector2{ _mm_sub_ps(lhs, rhs) };
	}

	inline Vector2& Vector2::operator*=(const Vector2& other) noexcept
	{
		const __m128 lhs = _mm_set_ps(0.0f, 0.0f, y, x);
		const __m128 rhs = _mm_set_ps(0.0f, 0.0f, other.y, other.x);
		return *this = Vector2{ _mm_mul_ps(lhs, rhs) };
	}

	inline Vector2& Vector2::operator*=(float scaler) noexcept
	{
		return *this *= Vector2{ scaler };
	}

	inline Vector2& Vector2::operator/=(const Vector2& other) noexcept
	{
		const __m128 lhs = _mm_set_ps(0.0f, 0.0f, y, x);
		const __m128 rhs = _mm_set_ps(0.0f, 0.0f, other.y, other.x);
		return *this = Vector2{ _mm_div_ps(lhs, rhs) };
	}

	inline Vector2& Vector2::operator/=(float divisor) noexcept
	{
		return *this /= Vector2{ divisor };
	}

	[[nodiscard]] inline Vector2 operator+(const Vector2& lhs, const Vector2& rhs) noexcept
	{
		return Vector2{ lhs } += rhs;
	}

	[[nodiscard]] inline Vector2 operator-(const Vector2& lhs, const Vector2& rhs) noexcept
	{
		return Vector2{ lhs } -= rhs;
	}

	[[nodiscard]] inline Vector2 operator*(const Vector2& lhs, const Vector2& rhs) noexcept
	{
		return Vector2{ lhs } *= rhs;
	}

	[[nodiscard]] inline Vector2 operator*(const Vector2& vec, float scaler) noexcept
	{
		return Vector2{ vec } *= scaler;
	}

	[[nodiscard]] inline Vector2 operator*(float scaler, const Vector2& vec) noexcept
	{
		return Vector2{ vec } *= scaler;
	}

	[[nodiscard]] inline Vector2 operator/(const Vector2& lhs, const Vector2& rhs) noexcept
	{
		return Vector2{ lhs } /= rhs;
	}

	[[nodiscard]] inline Vector2 operator/(const Vector2& vec, float divisor) noexcept
	{
		return Vector2{ vec } /= divisor;
	}

	[[nodiscard]] inline float operator|(const Vector2& lhs, const Vector2& rhs) noexcept
	{
		Vector2 mul = lhs * rhs;
		return mul.x + mul.y;
	}

	inline float Vector2::DistanceSquared(const Vector2& lhs, const Vector2& rhs)
	{
		return (lhs - rhs).LengthSquared();
	}

	inline float Vector2::Dot(const Vector2& lhs, const Vector2& rhs) noexcept { return lhs | rhs; }

	template <>
	[[nodiscard]] inline auto Min<Vector2>(const Vector2& lhs, const Vector2& rhs) noexcept
	{
		const __m128 lhsSimd = _mm_set_ps(0.0f, 0.0f, lhs.y, lhs.x);
		const __m128 rhsSimd = _mm_set_ps(0.0f, 0.0f, rhs.y, rhs.x);
		return Vector2{ _mm_min_ps(lhsSimd, rhsSimd) };
	}

	template <>
	[[nodiscard]] inline auto Max<Vector2>(const Vector2& lhs, const Vector2& rhs) noexcept
	{
		const __m128 lhsSimd = _mm_set_ps(0.0f, 0.0f, lhs.y, lhs.x);
		const __m128 rhsSimd = _mm_set_ps(0.0f, 0.0f, rhs.y, rhs.x);
		return Vector2{ _mm_max_ps(lhsSimd, rhsSimd) };
	}

	template <>
	[[nodiscard]] inline auto Clamp<Vector2>(const Vector2& n, const Vector2& min, const Vector2& max) noexcept
	{
		Vector2 realMin = Min(min, max);
		Vector2 realMax = Max(min, max);
		return Max(realMin, Min(realMax, n));
	}

	template <>
	[[nodiscard]] inline Vector2 Abs<Vector2>(const Vector2& n) noexcept
	{
		const __m128 signMask = _mm_set_ps1(-0.0f);
		const __m128 simd = _mm_set_ps(0.0f, 0.0f, n.y, n.x);
		return Vector2{ _mm_andnot_ps(signMask, simd) };
	}

	template <>
	[[nodiscard]] inline Vector2 Sign<Vector2>(const Vector2& n) noexcept
	{
		const __m128 zero = _mm_setzero_ps();
		const __m128 simd = _mm_set_ps(0.0f, 0.0f, n.y, n.x);
		const __m128 positive = _mm_and_ps(_mm_cmpgt_ps(simd, zero), _mm_set1_ps(1.0f));
		const __m128 negative = _mm_and_ps(_mm_cmplt_ps(simd, zero), _mm_set1_ps(-1.0f));
		return Vector2{ _mm_or_ps(positive, negative) };
	}

	[[nodiscard]] inline bool IsNearlyEqual(const Vector2& lhs, const Vector2& rhs, float tolerance = Epsilon) noexcept
	{
		__m128 vec = _mm_set_ps(0.0f, 0.0f, lhs.y, lhs.x);
		vec = _mm_sub_ps(vec, _mm_set_ps(0.0f, 0.0f, rhs.y, rhs.x));
		return Detail::IsLessEpsilon(vec, tolerance);
	}

	[[nodiscard]] inline bool IsNearlyZero(const Vector2& vec, float tolerance = Epsilon) noexcept
	{
		return Detail::IsLessEpsilon(_mm_set_ps(0.0f, 0.0f, vec.y, vec.x), tolerance);
	}

	[[nodiscard]] inline Vector2 GetRangePct(const Vector2& vec, const Vector2& min, const Vector2& max) noexcept
	{
		const __m128 vecSimd = _mm_set_ps(0.0f, 0.0f, vec.y, vec.x);
		const __m128 minSimd = _mm_set_ps(0.0f, 0.0f, min.y, min.x);
		const __m128 maxSimd = _mm_set_ps(0.0f, 0.0f, max.y, max.x);
		return Vector2{ _mm_div_ps(_mm_sub_ps(vecSimd, minSimd), _mm_sub_ps(maxSimd, minSimd)) };
	}

	[[nodiscard]] inline Vector2 Lerp(const Vector2& a, const Vector2& b, float t) noexcept
	{
		return a + t * (b - a);
	}
}