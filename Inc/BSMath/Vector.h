#pragma once

namespace BSMath
{
	struct Vector3
	{
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

		constexpr void Set(float inX, float inY, float inZ) noexcept;

		constexpr float GetLength() const noexcept;
		constexpr float GetLengthSquared() const noexcept;

		bool Normalize() noexcept;
		Vector3 GetNormal() const noexcept;

		static float Distance(const Vector3& lhs, const Vector3& rhs);
		static float DistanceSquared(const Vector3& lhs, const Vector3& rhs);

		float GetMin() const noexcept;
		float GetMax() const noexcept;

		friend bool operator==(const Vector3& lhs, const Vector3& rhs);
		inline bool operator!=(const Vector3& lhs, const Vector3& rhs) { return !(lhs == rhs); }

		Vector3 oeprator-() const noexcept;

		Vector3& operator+=(const Vector3& other) noexcept;
		Vector3& operator-=(const Vector3& other) noexcept;
		
		Vector3& operator*=(const Vector3& other) noexcept;
		Vector3& operator*=(float scaler) noexcept;
		
		Vector3& operator/=(const Vector3& other) noexcept;
		Vector3& operator-=(float scaler) noexcept;

		Vector3& operator^=(const Vector3& other) noexcept;
		
		constexpr float& operator[](size_t idx) noexcept;
		constexpr float operator[](size_t idx) const noexcept;

		friend Vector3 operator+(const Vector3& lhs, const Vector3& rhs) noexcept;
		friend Vector3 operator-(const Vector3& lhs, const Vector3& rhs) noexcept;
		friend Vector3 operator*(const Vector3& lhs, const Vector3& rhs) noexcept;
		friend Vector3 operator*(const Vector3& vec, float scaler) noexcept;
		friend Vector3 operator*(float scaler, const Vector3& vec) noexcept;
		friend Vector3 operator/(const Vector3& lhs, const Vector3& rhs) noexcept;
		friend Vector3 operator/(const Vector3& vec, float scaler) noexcept;
		friend float operator|(const Vector3& lhs, const Vector3& rhs) noexcept;
		friend Vector3 operator^(const Vector3& lhs, const Vector3& rhs) noexcept;

		static float Dot(const Vector3& lhs, const Vector3& rhs) noexcept;
		static Vector3 Cross(const Vector3& lhs, const Vector3& rhs) noexcept;

	public:
		float x;
		float y;
		float z;
	};
}