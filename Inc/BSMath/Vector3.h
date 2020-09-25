#pragma once

#include <emmintrin.h>
#include "Utility.h"

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
			float ret[4];
			_mm_store_ps(ret, simd);
			x = ret[0]; y = ret[1]; z = ret[2];
		}

		explicit Vector3(const struct Vector2& vec2, float inZ = 0.0f);
		explicit Vector3(const struct Vector4& vec4);

		explicit Vector3(const struct IntPoint& intPoint, float inZ = 0.0f);
		explicit Vector3(const struct IntVector& intVec);

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

	float Vector3::LengthSquared() const noexcept
	{
		const __m128 vec = _mm_set_ps(0.0f, z, y, x);
		Vector3 ret{ _mm_mul_ps(vec, vec) };
		return ret.x + ret.y + ret.z;
	}

	bool Vector3::Normalize() noexcept
	{
		const float lengthSquared = LengthSquared();
		if (IsNearlyZero(lengthSquared))
			return false;

		__m128 vec = _mm_set_ps(0.0f, z, y, x);
		const __m128 size = _mm_set_ps1(InvSqrt(lengthSquared));
		*this = Vector3{ _mm_mul_ps(vec, size) };
		return true;
	}

	bool Vector3::operator==(const Vector3& other) const noexcept
	{
		const __m128 lhs = _mm_set_ps(0.0f, z, y, x);
		const __m128 rhs = _mm_set_ps(0.0f, other.z, other.y, other.x);
		const auto val = _mm_movemask_ps(_mm_cmpeq_ps(lhs, rhs));
		return val == 15;
	}

	[[nodiscard]] Vector3 Vector3::operator-() const noexcept
	{
		__m128 zero = _mm_setzero_ps();
		__m128 vec = _mm_set_ps(0.0f, z, y, x);
		return Vector3{ _mm_sub_ps(zero, vec) };
	}

	Vector3& Vector3::operator+=(const Vector3& other) noexcept
	{
		const __m128 lhs = _mm_set_ps(0.0f, z, y, x);
		const __m128 rhs = _mm_set_ps(0.0f, other.z, other.y, other.x);
		return *this = Vector3{ _mm_add_ps(lhs, rhs) };
	}

	Vector3& Vector3::operator-=(const Vector3& other) noexcept
	{
		const __m128 lhs = _mm_set_ps(0.0f, z, y, x);
		const __m128 rhs = _mm_set_ps(0.0f, other.z, other.y, other.x);
		return *this = Vector3{ _mm_sub_ps(lhs, rhs) };
	}

	Vector3& Vector3::operator*=(const Vector3& other) noexcept
	{
		const __m128 lhs = _mm_set_ps(0.0f, z, y, x);
		const __m128 rhs = _mm_set_ps(0.0f, other.z, other.y, other.x);
		return *this = Vector3{ _mm_mul_ps(lhs, rhs) };
	}

	Vector3& Vector3::operator*=(float scaler) noexcept
	{
		const __m128 lhs = _mm_set_ps(0.0f, z, y, x);
		const __m128 rhs = _mm_set_ps1(scaler);
		return *this = Vector3{ _mm_mul_ps(lhs, rhs) };
	}

	Vector3& Vector3::operator/=(const Vector3& other) noexcept
	{
		const __m128 lhs = _mm_set_ps(0.0f, z, y, x);
		const __m128 rhs = _mm_set_ps(0.0f, other.z, other.y, other.x);
		return *this = Vector3{ _mm_div_ps(lhs, rhs) };
	}

	Vector3& Vector3::operator/=(float divisor) noexcept
	{
		const __m128 lhs = _mm_set_ps(0.0f, z, y, x);
		const __m128 rhs = _mm_set_ps1(divisor);
		return *this = Vector3{ _mm_div_ps(lhs, rhs) };
	}

	Vector3& Vector3::operator^=(const Vector3& other) noexcept
	{
		const __m128 a_yzx = _mm_set_ps(0.0f, x, z, y);
		const __m128 a_zxy = _mm_set_ps(0.0f, y, x, z);
		const __m128 b_zxy = _mm_set_ps(0.0f, other.y, other.x, other.z);
		const __m128 b_yzx = _mm_set_ps(0.0f, other.x, other.z, other.y);

		*this = Vector3{ _mm_sub_ps(_mm_mul_ps(a_yzx, b_zxy), _mm_mul_ps(a_zxy, b_yzx)) };
		return *this;
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

	[[nodiscard]] float operator|(const Vector3& lhs, const Vector3& rhs) noexcept
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

	inline float Vector3::Dot(const Vector3& lhs, const Vector3& rhs) noexcept { return lhs | rhs; }
	inline Vector3 Vector3::Cross(const Vector3& lhs, const Vector3& rhs) noexcept { return lhs ^ rhs; }

	template <>
	[[nodiscard]] auto Min<Vector3>(const Vector3& lhs, const Vector3& rhs) noexcept
	{
		const __m128 lhsSimd = _mm_set_ps(0.0f, lhs.z, lhs.y, lhs.x);
		const __m128 rhsSimd = _mm_set_ps(0.0f, rhs.z, rhs.y, rhs.x);
		return Vector3{ _mm_min_ps(lhsSimd, rhsSimd) };
	}

	template <>
	[[nodiscard]] auto Max<Vector3>(const Vector3& lhs, const Vector3& rhs) noexcept
	{
		const __m128 lhsSimd = _mm_set_ps(0.0f, lhs.z, lhs.y, lhs.x);
		const __m128 rhsSimd = _mm_set_ps(0.0f, rhs.z, rhs.y, rhs.x);
		return Vector3{ _mm_max_ps(lhsSimd, rhsSimd) };
	}

	template <>
	[[nodiscard]] auto Clamp<Vector3>(const Vector3& n, const Vector3& min, const Vector3& max) noexcept
	{
		const __m128 minSimd = _mm_set_ps(0.0f, min.z, min.y, min.x);
		const __m128 maxSimd = _mm_set_ps(0.0f, max.z, max.y, max.x);

		__m128 ret = _mm_set_ps(0.0f, n.z, n.y, n.x);
		ret = _mm_max_ps(ret, _mm_min_ps(minSimd, maxSimd));
		ret = _mm_min_ps(ret, _mm_max_ps(minSimd, maxSimd));
		return Vector3{ ret };
	}

	template <>
	[[nodiscard]] Vector3 Abs<Vector3>(const Vector3& n) noexcept
	{
		const __m128 signMask = _mm_set_ps1(-0.0f);
		const __m128 simd = _mm_set_ps(0.0f, n.z, n.y, n.x);
		return Vector3{ _mm_andnot_ps(signMask, simd) };
	}

	template <>
	[[nodiscard]] Vector3 Sign<Vector3>(const Vector3& n) noexcept
	{
		const __m128 signMask = _mm_set_ps1(-0.0f);
		const __m128 simd = _mm_set_ps(0.0f, n.z, n.y, n.x);
		return Vector3{ _mm_and_ps(signMask, simd) };
	}

	namespace Detail
	{
		[[nodiscard]] bool IsLessEpsilon(const __m128& vec, const Vector3& epsilon)
		{
			const __m128 tolerance = _mm_set_ps(0.0f, epsilon.z, epsilon.y, epsilon.x);
			return _mm_movemask_ps(_mm_cmple_ps(vec, tolerance)) == 15;
		}
	}

	[[nodiscard]] bool IsNearlyEqual(const Vector3& lhs, const Vector3& rhs, const Vector3& tolerance = Vector3{ Epsilon }) noexcept
	{
		__m128 vec = _mm_set_ps(0.0f, lhs.z, lhs.y, lhs.x);
		vec = _mm_sub_ps(vec, _mm_set_ps(0.0f, rhs.z, rhs.y, rhs.x));
		return Detail::IsLessEpsilon(vec, tolerance);
	}

	[[nodiscard]] inline bool IsNearlyZero(const Vector3& vec, const Vector3& tolerance = Vector3{ Epsilon }) noexcept
	{
		return Detail::IsLessEpsilon(_mm_set_ps(0.0f, vec.z, vec.y, vec.x), tolerance);
	}

	[[nodiscard]] Vector3 GetRangePct(const Vector3& vec, const Vector3& min, const Vector3& max) noexcept
	{
		const __m128 vecSimd = _mm_set_ps(0.0f, vec.z, vec.y, vec.x);
		const __m128 minSimd = _mm_set_ps(0.0f, min.z, min.y, min.x);
		const __m128 maxSimd = _mm_set_ps(0.0f, max.z, max.y, max.x);
		return Vector3{ _mm_div_ps(_mm_sub_ps(vecSimd, minSimd), _mm_sub_ps(maxSimd, minSimd)) };
	}
}