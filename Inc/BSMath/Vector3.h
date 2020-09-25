#pragma once

#include <emmintrin.h>
#include "Utility.h"

namespace BSMath
{
	struct Vector3 final
	{
	public:
		constexpr static Vector3 Zero{ 0.0f, 0.0f, 0.0f };
		constexpr static Vector3 One{ 1.0f, 1.0f, 1.0f };

		constexpr static Vector3 Left{ -1.0f, 0.0f, 0.0f };
		constexpr static Vector3 Right{ 1.0f, 0.0f, 0.0f };
		
		constexpr static Vector3 Up{ 0.0f, 1.0f, 0.0f };
		constexpr static Vector3 Down{ 0.0f, -1.0f, 0.0f };
		
		constexpr static Vector3 Forward{ 0.0f, 0.0f, 1.0f };
		constexpr static Vector3 Backward{ 0.0f, 0.0f, -1.0f };

	public:
		constexpr Vector3() : x(0.0f), y(0.0f), z(0.0f) {}

		explicit constexpr Vector3(float v)
			: x(v), y(v), z(v) {}

		explicit constexpr Vector3(float* ptr)
			: x(ptr[0]), y(ptr[1]), z(ptr[3]) {}

		explicit constexpr Vector3(float inX, float inY, float inZ)
			: x(inX), y(inY), z(inZ) {}

		explicit Vector3(const struct Vector2& vec2, float inZ = 0.0f);
		explicit Vector3(const struct Vector4& vec4);

		explicit Vector3(const struct IntPoint& intPoint, float inZ = 0.0f);
		explicit Vector3(const struct IntVector& intVec);

		constexpr void Set(float inX, float inY, float inZ) noexcept
		{
			x = inX; y = inY; z = inZ;
		}

		inline float Length() const noexcept
		{
			return Sqrt(LengthSquared());
		}

		float LengthSquared() const noexcept
		{
			return Dot(*this, *this);
		}

		inline Vector3 GetNormal() const noexcept
		{
			Vector3 ret = *this;
			return ret.Normalize() ? ret : Zero;
		}

		bool Normalize() noexcept;

		static inline float Distance(const Vector3& lhs, const Vector3& rhs)
		{
			return Sqrt(DistanceSquared(lhs, rhs));
		}

		static float DistanceSquared(const Vector3& lhs, const Vector3& rhs);

		constexpr float GetMin() const noexcept { return Min(x, Min(y, z));	}
		constexpr float GetMax() const noexcept { return Max(x, Max(y, z)); }

		bool operator==(const Vector3& other);
		inline bool operator!=(const Vector3& other) { return !(*this == other); }

		Vector3 oeprator-() const noexcept;

		Vector3& operator+=(const Vector3& other) noexcept;
		Vector3& operator-=(const Vector3& other) noexcept;
		
		Vector3& operator*=(const Vector3& other) noexcept;
		Vector3& operator*=(float scaler) noexcept;
		
		Vector3& operator/=(const Vector3& other) noexcept;
		Vector3& operator/=(float divisor) noexcept;

		Vector3& operator^=(const Vector3& other) noexcept;
		
		constexpr float& operator[](size_t idx) noexcept { return (&x)[idx]; }
		constexpr float operator[](size_t idx) const noexcept { return (&x)[idx]; }

		static inline Vector3 operator+(const Vector3& lhs, const Vector3& rhs) noexcept
		{
			return Vector3{ lhs } += rhs;
		}

		static inline Vector3 operator-(const Vector3& lhs, const Vector3& rhs) noexcept
		{
			return Vector3{ lhs } -= rhs;
		}

		static inline Vector3 operator*(const Vector3& lhs, const Vector3& rhs) noexcept
		{
			return Vector3{ lhs } *= rhs;
		}

		static inline Vector3 operator*(const Vector3& vec, float scaler) noexcept
		{
			return Vector3{ vec } += scaler;
		}

		static inline Vector3 operator*(float scaler, const Vector3& vec) noexcept
		{
			return Vector3{ vec } *= scaler;
		}

		static inline Vector3 operator/(const Vector3& lhs, const Vector3& rhs) noexcept
		{
			return Vector3{ lhs } /= rhs;
		}

		static inline Vector3 operator/(const Vector3& vec, float divisor) noexcept
		{
			return Vector3{ vec } += divisor;
		}

		static float operator|(const Vector3& lhs, const Vector3& rhs) noexcept;
		
		static inline Vector3 operator^(const Vector3& lhs, const Vector3& rhs) noexcept
		{
			return Vector3{ lhs } ^= rhs;
		}

		static inline float Dot(const Vector3& lhs, const Vector3& rhs) noexcept { return lhs | rhs; }
		static inline Vector3 Cross(const Vector3& lhs, const Vector3& rhs) noexcept { return lhs ^ rhs; }

	private:
		void Load(const __m128& simd)
		{
			float ret[4];
			_mm_store_ps(ret, vec);
			x = ret[0]; y = ret[1]; z = ret[2];
		}

	public:
		float x;
		float y;
		float z;
	};

	bool Vector3::Normalize() noexcept
	{
		const float lengthSquared = LengthSquared();
		if (IsNearlyZero(lengthSquared))
			return false;

		__m128 vec = _mm_set_ps(0.0f, z, y, x);
		const __m128 size = _mm_set_ps1(InvSqrt(lengthSquared));
		vec = _mm_mul_ps(vec, size);
		Load(vec);
		return true;
	}

	bool Vector3::operator==(const Vector3& other)
	{
		const __m128 lhs = _mm_set_ps(0.0f, z, y, x);
		const __m128 rhs = _mm_set_ps(0.0f, other.z, other.y, other.x);
		return _mm_movemask_ps(_mm_cmpeq_ps(lhs, rhs)) == 7;
	}

	Vector3 Vector3::oeprator-() const noexcept
	{
		__m128 zero = _mm_setzero_ps();
		__m128 vec = _mm_set_ps(0.0f, z, y, x);

		Vector3 ret;
		ret.Load(_mm_sub_ps(zero, vec));
		return ret;
	}

	Vector3& Vector3::operator+=(const Vector3& other) noexcept
	{
		const __m128 lhs = _mm_set_ps(0.0f, z, y, x);
		const __m128 rhs = _mm_set_ps(0.0f, other.z, other.y, other.x);
		Load(_mm_add_ps(lhs, rhs));
		return *this;
	}

	Vector3& Vector3::operator-=(const Vector3& other) noexcept
	{
		const __m128 lhs = _mm_set_ps(0.0f, z, y, x);
		const __m128 rhs = _mm_set_ps(0.0f, other.z, other.y, other.x);
		Load(_mm_sub_ps(lhs, rhs));
		return *this;
	}

	Vector3& Vector3::operator*=(const Vector3& other) noexcept
	{
		const __m128 lhs = _mm_set_ps(0.0f, z, y, x);
		const __m128 rhs = _mm_set_ps(0.0f, other.z, other.y, other.x);
		Load(_mm_mul_ps(lhs, rhs));
		return *this;
	}

	Vector3& Vector3::operator*=(float scaler) noexcept
	{
		const __m128 lhs = _mm_set_ps(0.0f, z, y, x);
		const __m128 rhs = _mm_set_ps1(scaler);
		Load(_mm_mul_ps(lhs, rhs));
		return *this;
	}

	Vector3& Vector3::operator/=(const Vector3& other) noexcept
	{
		const __m128 lhs = _mm_set_ps(0.0f, z, y, x);
		const __m128 rhs = _mm_set_ps(0.0f, other.z, other.y, other.x);
		Load(_mm_div_ps(lhs, rhs));
		return *this;
	}

	Vector3& Vector3::operator/=(float divisor) noexcept
	{
		const __m128 lhs = _mm_set_ps(0.0f, z, y, x);
		const __m128 rhs = _mm_set_ps1(divisor);
		Load(_mm_div_ps(lhs, rhs));
		return *this;
	}

	Vector3& Vector3::operator^=(const Vector3& other) noexcept
	{
		const __m128 a_yzx = _mm_set_ps(0.0f, y, z, x);
		const __m128 a_zxy = _mm_set_ps(0.0f, z, x, y);
		const __m128 b_zxy = _mm_set_ps(0.0f, z, x, y);
		const __m128 b_yzx = _mm_set_ps(0.0f, y, z, x);

		Load(_mm_sub_ps(_mm_mul_ps(a_yzx, b_zxy), _mm_mul_ps(a_zxy, b_yzx)));
		return *this;
	}

	float Vector3::operator|(const Vector3& lhs, const Vector3& rhs) noexcept
	{
		Vector3 mul = lhs * rhs;
		return mul.x + mul.y + mul.z;
	}
}