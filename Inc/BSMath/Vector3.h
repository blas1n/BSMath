#pragma once

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

		inline float GetLength() const noexcept
		{
			return Sqrt(GetLengthSquared());
		}

		float GetLengthSquared() const noexcept;

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

		float GetMin() const noexcept;
		float GetMax() const noexcept;

		static inline  bool operator==(const Vector3& lhs, const Vector3& rhs);
		inline bool operator!=(const Vector3& lhs, const Vector3& rhs) { return !(lhs == rhs); }

		Vector3 oeprator-() const noexcept;

		Vector3& operator+=(const Vector3& other) noexcept;
		Vector3& operator-=(const Vector3& other) noexcept;
		
		Vector3& operator*=(const Vector3& other) noexcept;
		Vector3& operator*=(float scaler) noexcept;
		
		Vector3& operator/=(const Vector3& other) noexcept;
		Vector3& operator-=(float scaler) noexcept;

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

	public:
		float x;
		float y;
		float z;
	};
}