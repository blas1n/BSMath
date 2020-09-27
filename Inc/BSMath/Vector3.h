#pragma once

#include "Vector2.h"
#include "IntVector.h"

namespace BSMath
{
	struct Vector3 final
	{
	public: // Templates
		[[nodiscard]] constexpr static Vector3 Zero() noexcept
		{
			return Vector3{ 0.0f };
		}

		[[nodiscard]] constexpr static Vector3 One() noexcept
		{
			return Vector3{ 1.0f };
		}

		[[nodiscard]] constexpr static Vector3 Right() noexcept
		{
			return Vector3{ 1.0f, 0.0f, 0.0f };
		}

		[[nodiscard]] constexpr static Vector3 Left() noexcept
		{
			return Vector3{ -1.0f, 0.0f, 0.0f };
		}

		[[nodiscard]] constexpr static Vector3 Up() noexcept
		{
			return Vector3{ 0.0f, 1.0f, 0.0f };
		}

		[[nodiscard]] constexpr static Vector3 Down() noexcept
		{
			return Vector3{ 0.0f, -1.0f, 0.0f };
		}

		[[nodiscard]] constexpr static Vector3 Forward() noexcept
		{
			return Vector3{ 0.0f, 0.0f, 1.0f };
		}

		[[nodiscard]] constexpr static Vector3 Backward() noexcept
		{
			return Vector3{ 0.0f, 0.0f, -1.0f };
		}


	public:
		constexpr Vector3() : x(0.0f), y(0.0f), z(0.0f) {}

		explicit constexpr Vector3(float n)
			: x(n), y(n), z(n) {}

		explicit constexpr Vector3(const float* ptr)
			: x(ptr[0]), y(ptr[1]), z(ptr[2]) {}

		explicit constexpr Vector3(float inX, float inY, float inZ)
			: x(inX), y(inY), z(inZ) {}

		explicit Vector3(const __m128& simd)
		{
			alignas(16) float ret[4];
			_mm_store_ps(ret, simd);
			x = ret[0]; y = ret[1]; z = ret[2];
		}

		explicit constexpr Vector3(const Vector2& vec2, float inZ = 0.0f)
			: x(vec2.x), y(vec2.y), z(inZ) {}

		explicit constexpr Vector3(const struct Vector4& vec4);

		explicit constexpr Vector3(const IntPoint& point, float inZ = 0.0f)
			: x(static_cast<float>(point.x)), y(static_cast<float>(point.y)), z(inZ) {}

		explicit constexpr Vector3(const IntVector& vec)
			: x(static_cast<float>(vec.x)),
			y(static_cast<float>(vec.y)),
			z(static_cast<float>(vec.z)) {}

		constexpr void Set(float inX, float inY, float inZ) noexcept
		{
			x = inX; y = inY; z = inZ;
		}

		[[nodiscard]] inline float Length() const noexcept
		{
			return Sqrt(LengthSquared());
		}

		[[nodiscard]] float LengthSquared() const noexcept;

		[[nodiscard]] inline Vector3 GetNormal() const noexcept
		{
			Vector3 ret = *this;
			return ret.Normalize() ? ret : Zero();
		}

		bool Normalize() noexcept;

		[[nodiscard]] static inline float Distance(const Vector3& lhs, const Vector3& rhs)
		{
			return Sqrt(DistanceSquared(lhs, rhs));
		}

		[[nodiscard]] static float DistanceSquared(const Vector3& lhs, const Vector3& rhs);

		[[nodiscard]] constexpr float GetMin() const noexcept { return Min(x, Min(y, z));	}
		[[nodiscard]] constexpr float GetMax() const noexcept { return Max(x, Max(y, z)); }

		[[nodiscard]] bool operator==(const Vector3& other) const noexcept;
		[[nodiscard]] inline bool operator!=(const Vector3& other) const noexcept { return !(*this == other); }

		Vector3 operator-() const noexcept;

		Vector3& operator+=(const Vector3& other) noexcept;
		Vector3& operator-=(const Vector3& other) noexcept;
		
		Vector3& operator*=(const Vector3& other) noexcept;
		Vector3& operator*=(float scaler) noexcept;
		
		Vector3& operator/=(const Vector3& other) noexcept;
		Vector3& operator/=(float divisor) noexcept;

		Vector3& operator^=(const Vector3& other) noexcept;
		
		[[nodiscard]] constexpr float& operator[](size_t idx) noexcept { return (&x)[idx]; }
		[[nodiscard]] constexpr float operator[](size_t idx) const noexcept { return (&x)[idx]; }

		[[nodiscard]] static float Dot(const Vector3& lhs, const Vector3& rhs) noexcept;
		[[nodiscard]] static Vector3 Cross(const Vector3& lhs, const Vector3& rhs) noexcept;

	public:
		float x;
		float y;
		float z;
	};

	inline float Vector3::LengthSquared() const noexcept
	{
		const __m128 vec = _mm_set_ps(0.0f, z, y, x);
		Vector3 ret{ _mm_mul_ps(vec, vec) };
		return ret.x + ret.y + ret.z;
	}

	inline bool Vector3::Normalize() noexcept
	{
		const float lengthSquared = LengthSquared();
		if (IsNearlyZero(lengthSquared))
			return false;

		const __m128 vec = _mm_set_ps(0.0f, z, y, x);
		const __m128 size = _mm_set_ps1(InvSqrt(lengthSquared));
		*this = Vector3{ _mm_mul_ps(vec, size) };
		return true;
	}

	inline bool Vector3::operator==(const Vector3& other) const noexcept
	{
		const __m128 lhs = _mm_set_ps(0.0f, z, y, x);
		const __m128 rhs = _mm_set_ps(0.0f, other.z, other.y, other.x);
		return _mm_movemask_ps(_mm_cmpeq_ps(lhs, rhs)) == 0xF;
	}

	[[nodiscard]] inline Vector3 Vector3::operator-() const noexcept
	{
		const __m128 zero = _mm_setzero_ps();
		const __m128 vec = _mm_set_ps(0.0f, z, y, x);
		return Vector3{ _mm_sub_ps(zero, vec) };
	}

	inline Vector3& Vector3::operator+=(const Vector3& other) noexcept
	{
		const __m128 lhs = _mm_set_ps(0.0f, z, y, x);
		const __m128 rhs = _mm_set_ps(0.0f, other.z, other.y, other.x);
		return *this = Vector3{ _mm_add_ps(lhs, rhs) };
	}

	inline Vector3& Vector3::operator-=(const Vector3& other) noexcept
	{
		const __m128 lhs = _mm_set_ps(0.0f, z, y, x);
		const __m128 rhs = _mm_set_ps(0.0f, other.z, other.y, other.x);
		return *this = Vector3{ _mm_sub_ps(lhs, rhs) };
	}

	inline Vector3& Vector3::operator*=(const Vector3& other) noexcept
	{
		const __m128 lhs = _mm_set_ps(0.0f, z, y, x);
		const __m128 rhs = _mm_set_ps(0.0f, other.z, other.y, other.x);
		return *this = Vector3{ _mm_mul_ps(lhs, rhs) };
	}

	inline Vector3& Vector3::operator*=(float scaler) noexcept
	{
		return *this *= Vector3{ scaler };
	}

	inline Vector3& Vector3::operator/=(const Vector3& other) noexcept
	{
		const __m128 lhs = _mm_set_ps(0.0f, z, y, x);
		const __m128 rhs = _mm_set_ps(0.0f, other.z, other.y, other.x);
		return *this = Vector3{ _mm_div_ps(lhs, rhs) };
	}

	inline Vector3& Vector3::operator/=(float divisor) noexcept
	{
		return *this /= Vector3{ divisor };
	}

	[[nodiscard]] inline Vector3 operator+(const Vector3& lhs, const Vector3& rhs) noexcept
	{
		return Vector3{ lhs } += rhs;
	}

	[[nodiscard]] inline Vector3 operator-(const Vector3& lhs, const Vector3& rhs) noexcept
	{
		return Vector3{ lhs } -= rhs;
	}

	[[nodiscard]] inline Vector3 operator*(const Vector3& lhs, const Vector3& rhs) noexcept
	{
		return Vector3{ lhs } *= rhs;
	}

	[[nodiscard]] inline Vector3 operator*(const Vector3& vec, float scaler) noexcept
	{
		return Vector3{ vec } *= scaler;
	}

	[[nodiscard]] inline Vector3 operator*(float scaler, const Vector3& vec) noexcept
	{
		return Vector3{ vec } *= scaler;
	}

	[[nodiscard]] inline Vector3 operator/(const Vector3& lhs, const Vector3& rhs) noexcept
	{
		return Vector3{ lhs } /= rhs;
	}

	[[nodiscard]] inline Vector3 operator/(const Vector3& vec, float divisor) noexcept
	{
		return Vector3{ vec } /= divisor;
	}

	[[nodiscard]] inline float operator|(const Vector3& lhs, const Vector3& rhs) noexcept
	{
		Vector3 mul = lhs * rhs;
		return mul.x + mul.y + mul.z;
	}

	[[nodiscard]] inline Vector3 operator^(const Vector3& lhs, const Vector3& rhs) noexcept
	{
		return Vector3{ lhs } ^= rhs;
	}

	inline float Vector3::DistanceSquared(const Vector3& lhs, const Vector3& rhs)
	{
		return (lhs - rhs).LengthSquared();
	}

	inline Vector3& Vector3::operator^=(const Vector3& other) noexcept
	{
		auto ret1 = Vector3{ y, z, x } *Vector3{ other.z, other.x, other.y };
		auto ret2 = Vector3{ z, x, y } *Vector3{ other.y, other.z, other.x };
		return *this = ret1 - ret2;
	}

	inline float Vector3::Dot(const Vector3& lhs, const Vector3& rhs) noexcept { return lhs | rhs; }
	inline Vector3 Vector3::Cross(const Vector3& lhs, const Vector3& rhs) noexcept { return lhs ^ rhs; }

	template <>
	[[nodiscard]] inline auto Min<Vector3>(const Vector3& lhs, const Vector3& rhs) noexcept
	{
		const __m128 lhsSimd = _mm_set_ps(0.0f, lhs.z, lhs.y, lhs.x);
		const __m128 rhsSimd = _mm_set_ps(0.0f, rhs.z, rhs.y, rhs.x);
		return Vector3{ _mm_min_ps(lhsSimd, rhsSimd) };
	}

	template <>
	[[nodiscard]] inline auto Max<Vector3>(const Vector3& lhs, const Vector3& rhs) noexcept
	{
		const __m128 lhsSimd = _mm_set_ps(0.0f, lhs.z, lhs.y, lhs.x);
		const __m128 rhsSimd = _mm_set_ps(0.0f, rhs.z, rhs.y, rhs.x);
		return Vector3{ _mm_max_ps(lhsSimd, rhsSimd) };
	}

	template <>
	[[nodiscard]] inline auto Clamp<Vector3>(const Vector3& n, const Vector3& min, const Vector3& max) noexcept
	{
		Vector3 realMin = Min(min, max);
		Vector3 realMax = Max(min, max);
		return Max(realMin, Min(realMax, n));
	}

	template <>
	[[nodiscard]] inline Vector3 Abs<Vector3>(const Vector3& n) noexcept
	{
		const __m128 signMask = _mm_set_ps1(-0.0f);
		const __m128 simd = _mm_set_ps(0.0f, n.z, n.y, n.x);
		return Vector3{ _mm_andnot_ps(signMask, simd) };
	}

	template <>
	[[nodiscard]] inline Vector3 Sign<Vector3>(const Vector3& n) noexcept
	{
		const __m128 zero = _mm_setzero_ps();
		const __m128 simd = _mm_set_ps(0.0f, n.z, n.y, n.x);
		const __m128 positive = _mm_and_ps(_mm_cmpgt_ps(simd, zero), _mm_set1_ps(1.0f));
		const __m128 negative = _mm_and_ps(_mm_cmplt_ps(simd, zero), _mm_set1_ps(-1.0f));
		return Vector3{ _mm_or_ps(positive, negative) };
	}

	[[nodiscard]] inline bool IsNearlyEqual(const Vector3& lhs, const Vector3& rhs, float tolerance = Epsilon) noexcept
	{
		__m128 vec = _mm_set_ps(0.0f, lhs.z, lhs.y, lhs.x);
		vec = _mm_sub_ps(vec, _mm_set_ps(0.0f, rhs.z, rhs.y, rhs.x));
		return Detail::IsLessEpsilon(vec, tolerance);
	}

	[[nodiscard]] inline bool IsNearlyZero(const Vector3& vec, float tolerance = Epsilon) noexcept
	{
		return Detail::IsLessEpsilon(_mm_set_ps(0.0f, vec.z, vec.y, vec.x), tolerance);
	}

	[[nodiscard]] inline Vector3 GetRangePct(const Vector3& vec, const Vector3& min, const Vector3& max) noexcept
	{
		const __m128 vecSimd = _mm_set_ps(0.0f, vec.z, vec.y, vec.x);
		const __m128 minSimd = _mm_set_ps(0.0f, min.z, min.y, min.x);
		const __m128 maxSimd = _mm_set_ps(0.0f, max.z, max.y, max.x);
		return Vector3{ _mm_div_ps(_mm_sub_ps(vecSimd, minSimd), _mm_sub_ps(maxSimd, minSimd)) };
	}

	[[nodiscard]] inline Vector3 Lerp(const Vector3& a, const Vector3& b, float t) noexcept
	{
		return a + t * (b - a);
	}

	// Vector2's Constructor
	constexpr Vector2::Vector2(const Vector3& vec3)
		: x(vec3.x), y(vec3.y) {}
}