#pragma once

#include "Vector3.h"

namespace BSMath
{
	struct Vector4 final
	{
	public: // Templates
		[[nodiscard]] constexpr static Vector4 Zero() noexcept
		{
			return Vector4{ 0.0f };
		}

		[[nodiscard]] constexpr static Vector4 One() noexcept
		{
			return Vector4{ 1.0f };
		}

	public:
		constexpr Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}

		explicit constexpr Vector4(float n)
			: x(n), y(n), z(n), w(n) {}

		explicit constexpr Vector4(const float* ptr)
			: x(ptr[0]), y(ptr[1]), z(ptr[2]), w(ptr[3]) {}

		explicit constexpr Vector4(float inX, float inY, float inZ, float inW)
			: x(inX), y(inY), z(inZ), w(inW) {}

		explicit Vector4(const __m128& simd)
		{
			alignas(16) float ret[4];
			_mm_store_ps(ret, simd);
			x = ret[0]; y = ret[1]; z = ret[2]; w = ret[3];
		}

		explicit constexpr Vector4(const Vector2& vec2, float inZ = 0.0f, float inW = 0.0f)
			: x(vec2.x), y(vec2.y), z(inZ), w(inW) {}

		explicit constexpr Vector4(const Vector3& vec3, float inW = 0.0f)
			: x(vec3.x), y(vec3.y), z(vec3.z), w(inW) {}

		explicit constexpr Vector4(const IntPoint& point, float inZ = 0.0f, float inW = 0.0f)
			: x(static_cast<float>(point.x)), y(static_cast<float>(point.y)), z(inZ), w(inW) {}

		explicit constexpr Vector4(const IntVector& vec, float inW = 0.0f)
			: x(static_cast<float>(vec.x)),
			y(static_cast<float>(vec.y)),
			z(static_cast<float>(vec.z)),
			w(static_cast<float>(inW)) {}

		constexpr void Set(float inX, float inY, float inZ, float inW) noexcept
		{
			x = inX; y = inY; z = inZ; w = inW;
		}

		[[nodiscard]] inline float Length() const noexcept
		{
			return Sqrt(LengthSquared());
		}

		[[nodiscard]] float LengthSquared() const noexcept;

		[[nodiscard]] inline Vector4 GetNormal() const noexcept
		{
			Vector4 ret = *this;
			return ret.Normalize() ? ret : Zero();
		}

		bool Normalize() noexcept;

		[[nodiscard]] static inline float Distance(const Vector4& lhs, const Vector4& rhs)
		{
			return Sqrt(DistanceSquared(lhs, rhs));
		}

		[[nodiscard]] static float DistanceSquared(const Vector4& lhs, const Vector4& rhs);

		[[nodiscard]] constexpr float GetMin() const noexcept { return Min(x, Min(y, Min(z, w))); }
		[[nodiscard]] constexpr float GetMax() const noexcept { return Max(x, Max(y, Max(z, w))); }

		[[nodiscard]] bool operator==(const Vector4& other) const noexcept;
		[[nodiscard]] inline bool operator!=(const Vector4& other) const noexcept { return !(*this == other); }

		Vector4 operator-() const noexcept;

		Vector4& operator+=(const Vector4& other) noexcept;
		Vector4& operator-=(const Vector4& other) noexcept;

		Vector4& operator*=(const Vector4& other) noexcept;
		Vector4& operator*=(float scaler) noexcept;

		Vector4& operator/=(const Vector4& other) noexcept;
		Vector4& operator/=(float divisor) noexcept;

		[[nodiscard]] constexpr float& operator[](size_t idx) noexcept { return (&x)[idx]; }
		[[nodiscard]] constexpr float operator[](size_t idx) const noexcept { return (&x)[idx]; }

		[[nodiscard]] static float Dot(const Vector4& lhs, const Vector4& rhs) noexcept;

	public:
		float x;
		float y;
		float z;
		float w;
	};

	inline float Vector4::LengthSquared() const noexcept
	{
		const __m128 vec = _mm_set_ps(w, z, y, x);
		Vector4 ret{ _mm_mul_ps(vec, vec) };
		return ret.x + ret.y + ret.z + ret.w;
	}

	inline bool Vector4::Normalize() noexcept
	{
		const float lengthSquared = LengthSquared();
		if (IsNearlyZero(lengthSquared))
			return false;

		const __m128 vec = _mm_set_ps(w, z, y, x);
		const __m128 size = _mm_set_ps1(InvSqrt(lengthSquared));
		*this = Vector4{ _mm_mul_ps(vec, size) };
		return true;
	}

	inline bool Vector4::operator==(const Vector4& other) const noexcept
	{
		const __m128 lhs = _mm_set_ps(w, z, y, x);
		const __m128 rhs = _mm_set_ps(other.w, other.z, other.y, other.x);
		return _mm_movemask_ps(_mm_cmpeq_ps(lhs, rhs)) == 15;
	}

	[[nodiscard]] inline Vector4 Vector4::operator-() const noexcept
	{
		const __m128 zero = _mm_setzero_ps();
		const __m128 vec = _mm_set_ps(w, z, y, x);
		return Vector4{ _mm_sub_ps(zero, vec) };
	}

	inline Vector4& Vector4::operator+=(const Vector4& other) noexcept
	{
		const __m128 lhs = _mm_set_ps(w, z, y, x);
		const __m128 rhs = _mm_set_ps(other.w, other.z, other.y, other.x);
		return *this = Vector4{ _mm_add_ps(lhs, rhs) };
	}

	inline Vector4& Vector4::operator-=(const Vector4& other) noexcept
	{
		const __m128 lhs = _mm_set_ps(w, z, y, x);
		const __m128 rhs = _mm_set_ps(other.w, other.z, other.y, other.x);
		return *this = Vector4{ _mm_sub_ps(lhs, rhs) };
	}

	inline Vector4& Vector4::operator*=(const Vector4& other) noexcept
	{
		const __m128 lhs = _mm_set_ps(w, z, y, x);
		const __m128 rhs = _mm_set_ps(other.w, other.z, other.y, other.x);
		return *this = Vector4{ _mm_mul_ps(lhs, rhs) };
	}

	inline Vector4& Vector4::operator*=(float scaler) noexcept
	{
		return *this *= Vector4{ scaler };
	}

	inline Vector4& Vector4::operator/=(const Vector4& other) noexcept
	{
		const __m128 lhs = _mm_set_ps(w, z, y, x);
		const __m128 rhs = _mm_set_ps(other.w, other.z, other.y, other.x);
		return *this = Vector4{ _mm_div_ps(lhs, rhs) };
	}

	inline Vector4& Vector4::operator/=(float divisor) noexcept
	{
		return *this /= Vector4{ divisor };
	}

	[[nodiscard]] inline Vector4 operator+(const Vector4& lhs, const Vector4& rhs) noexcept
	{
		return Vector4{ lhs } += rhs;
	}

	[[nodiscard]] inline Vector4 operator-(const Vector4& lhs, const Vector4& rhs) noexcept
	{
		return Vector4{ lhs } -= rhs;
	}

	[[nodiscard]] inline Vector4 operator*(const Vector4& lhs, const Vector4& rhs) noexcept
	{
		return Vector4{ lhs } *= rhs;
	}

	[[nodiscard]] inline Vector4 operator*(const Vector4& vec, float scaler) noexcept
	{
		return Vector4{ vec } *= scaler;
	}

	[[nodiscard]] inline Vector4 operator*(float scaler, const Vector4& vec) noexcept
	{
		return Vector4{ vec } *= scaler;
	}

	[[nodiscard]] inline Vector4 operator/(const Vector4& lhs, const Vector4& rhs) noexcept
	{
		return Vector4{ lhs } /= rhs;
	}

	[[nodiscard]] inline Vector4 operator/(const Vector4& vec, float divisor) noexcept
	{
		return Vector4{ vec } /= divisor;
	}

	[[nodiscard]] inline float operator|(const Vector4& lhs, const Vector4& rhs) noexcept
	{
		Vector4 mul = lhs * rhs;
		return mul.x + mul.y + mul.z + mul.w;
	}

	inline float Vector4::DistanceSquared(const Vector4& lhs, const Vector4& rhs)
	{
		return (lhs - rhs).LengthSquared();
	}

	inline float Vector4::Dot(const Vector4& lhs, const Vector4& rhs) noexcept { return lhs | rhs; }

	template <>
	[[nodiscard]] inline auto Min<Vector4>(const Vector4& lhs, const Vector4& rhs) noexcept
	{
		const __m128 lhsSimd = _mm_set_ps(lhs.w, lhs.z, lhs.y, lhs.x);
		const __m128 rhsSimd = _mm_set_ps(rhs.w, rhs.z, rhs.y, rhs.x);
		return Vector4{ _mm_min_ps(lhsSimd, rhsSimd) };
	}

	template <>
	[[nodiscard]] inline auto Max<Vector4>(const Vector4& lhs, const Vector4& rhs) noexcept
	{
		const __m128 lhsSimd = _mm_set_ps(lhs.w, lhs.z, lhs.y, lhs.x);
		const __m128 rhsSimd = _mm_set_ps(rhs.w, rhs.z, rhs.y, rhs.x);
		return Vector4{ _mm_max_ps(lhsSimd, rhsSimd) };
	}

	template <>
	[[nodiscard]] inline auto Clamp<Vector4>(const Vector4& n, const Vector4& min, const Vector4& max) noexcept
	{
		Vector4 realMin = Min(min, max);
		Vector4 realMax = Max(min, max);
		return Max(realMin, Min(realMax, n));
	}

	template <>
	[[nodiscard]] inline Vector4 Abs<Vector4>(const Vector4& n) noexcept
	{
		const __m128 signMask = _mm_set_ps1(-0.0f);
		const __m128 simd = _mm_set_ps(n.w, n.z, n.y, n.x);
		return Vector4{ _mm_andnot_ps(signMask, simd) };
	}

	template <>
	[[nodiscard]] inline Vector4 Sign<Vector4>(const Vector4& n) noexcept
	{
		const __m128 zero = _mm_setzero_ps();
		const __m128 simd = _mm_set_ps(n.w, n.z, n.y, n.x);
		const __m128 positive = _mm_and_ps(_mm_cmpgt_ps(simd, zero), _mm_set1_ps(1.0f));
		const __m128 negative = _mm_and_ps(_mm_cmplt_ps(simd, zero), _mm_set1_ps(-1.0f));
		return Vector4{ _mm_or_ps(positive, negative) };
	}

	[[nodiscard]] inline bool IsNearlyEqual(const Vector4& lhs, const Vector4& rhs, float tolerance = Epsilon) noexcept
	{
		__m128 vec = _mm_set_ps(lhs.w, lhs.z, lhs.y, lhs.x);
		vec = _mm_sub_ps(vec, _mm_set_ps(rhs.w, rhs.z, rhs.y, rhs.x));
		return Detail::IsLessEpsilon(vec, tolerance);
	}

	[[nodiscard]] inline bool IsNearlyZero(const Vector4& vec, float tolerance = Epsilon) noexcept
	{
		return Detail::IsLessEpsilon(_mm_set_ps(vec.w, vec.z, vec.y, vec.x), tolerance);
	}

	[[nodiscard]] inline Vector4 GetRangePct(const Vector4& vec, const Vector4& min, const Vector4& max) noexcept
	{
		const __m128 vecSimd = _mm_set_ps(vec.w, vec.z, vec.y, vec.x);
		const __m128 minSimd = _mm_set_ps(min.w, min.z, min.y, min.x);
		const __m128 maxSimd = _mm_set_ps(max.w, max.z, max.y, max.x);
		return Vector4{ _mm_div_ps(_mm_sub_ps(vecSimd, minSimd), _mm_sub_ps(maxSimd, minSimd)) };
	}

	[[nodiscard]] inline Vector4 Lerp(const Vector4& a, const Vector4& b, float t) noexcept
	{
		return a + t * (b - a);
	}

	// Vector 2and3's Constructor
	constexpr Vector2::Vector2(const Vector4& vec4)
		: x(vec4.x), y(vec4.y) {}

	constexpr Vector3::Vector3(const Vector4& vec4)
		: x(vec4.x), y(vec4.y), z(vec4.z) {}

	// Random
	class UniformVector4Distribution : public std::uniform_real_distribution<float>
	{
		using Super = std::uniform_real_distribution<float>;

	public:
		using result_type = Vector4;

		template <class Engine>
		[[nodiscard]] result_type operator()(Engine& engine)
		{
			const float x = Super::operator()(engine);
			const float y = Super::operator()(engine);
			const float z = Super::operator()(engine);
			const float w = Super::operator()(engine);
			return Vector4{ x, y, z, w };
		}

		template <class Engine>
		[[nodiscard]] result_type operator()(Engine& engine, const param_type& param)
		{
			const float x = Super::operator()(engine, param);
			const float y = Super::operator()(engine, param);
			const float z = Super::operator()(engine, param);
			const float w = Super::operator()(engine, param);
			return Vector4{ x, y, z, w };
		}
	};

	class NormalVector4Distribution : public std::normal_distribution<float>
	{
		using Super = std::normal_distribution<float>;

	public:
		using result_type = Vector3;

		template <class Engine>
		[[nodiscard]] result_type operator()(Engine& engine)
		{
			const float x = Super::operator()(engine);
			const float y = Super::operator()(engine);
			const float z = Super::operator()(engine);
			const float w = Super::operator()(engine);
			return Vector4{ x, y, z, w };
		}

		template <class Engine>
		[[nodiscard]] result_type operator()(Engine& engine, const param_type& param)
		{
			const float x = Super::operator()(engine, param);
			const float y = Super::operator()(engine, param);
			const float z = Super::operator()(engine, param);
			const float w = Super::operator()(engine, param);
			return Vector4{ x, y, z, w };
		}
	};

	using UniformVector4Random = Random<Vector4, std::mt19937, UniformVector4Distribution>;
	using NormalVector4Random = Random<Vector4, std::mt19937, NormalVector4Distribution>;
}