#pragma once

#include "Vector.h"

namespace BSMath
{
	struct alignas(16) Quaternion final
	{
	public:
		const static Quaternion Identity;

	public:
		constexpr Quaternion() noexcept : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}

		explicit constexpr Quaternion(float inX, float inY, float inZ, float inW) noexcept
			: x(inX), y(inY), z(inZ), w(inW) {}

		explicit constexpr Quaternion(const float* ptr) noexcept
			: x(ptr[0]), y(ptr[1]), z(ptr[2]), w(ptr[3]) {}

		constexpr void Set(float inX, float inY, float inZ, float inW) noexcept
		{
			x = inX; y = inY; z = inZ;	w = inW;
		}

		[[nodiscard]] constexpr Quaternion operator-() const noexcept
		{
			return Quaternion{ -x, -y, -z, w };
		}

		Quaternion& operator*=(const Quaternion& other) noexcept;

		[[nodiscard]] constexpr float& operator[](size_t i) noexcept { return (&x)[i]; }
		[[nodiscard]] constexpr float operator[](size_t i) const noexcept { return (&x)[i]; }

	public:
		float x;
		float y;
		float z;
		float w;
	};

	inline const Quaternion Quaternion::Identity{};

	NO_ODR Quaternion& Quaternion::operator*=(const Quaternion& other) noexcept
	{
		using namespace SIMD;
		
		const static auto SignMask0 = VectorLoad( 1.f, -1.f,  1.f, -1.f);
		const static auto SignMask1 = VectorLoad( 1.f,  1.f, -1.f, -1.f);
		const static auto SignMask2 = VectorLoad(-1.f,  1.f,  1.f, -1.f);

		const auto lhs = VectorLoadPtr(&x);
		const auto rhs = VectorLoadPtr(&other.x);
		auto result = VectorMultiply(VectorReplicate<Swizzle::W>(lhs), rhs);

		auto tmp = VectorMultiply(VectorReplicate<Swizzle::X>(lhs), VectorSwizzle<Swizzle::W, Swizzle::Z, Swizzle::Y, Swizzle::X>(rhs));
		result = VectorAdd(VectorMultiply(tmp, SignMask0), result);

		tmp = VectorMultiply(VectorReplicate<Swizzle::Y>(lhs), VectorSwizzle<Swizzle::Z, Swizzle::W, Swizzle::X, Swizzle::Y>(rhs));
		result = VectorAdd(VectorMultiply(tmp, SignMask1), result);

		tmp = VectorMultiply(VectorReplicate<Swizzle::Z>(lhs), VectorSwizzle<Swizzle::Y, Swizzle::X, Swizzle::W, Swizzle::Z>(rhs));
		result = VectorAdd(VectorMultiply(tmp, SignMask2), result);

		VectorStorePtr(result, &x);
		return *this;
	}

	[[nodiscard]] NO_ODR bool operator==(const Quaternion& lhs, const Quaternion& rhs) noexcept
	{
		using namespace SIMD;
		return VectorMoveMask(VectorEqual(VectorLoadPtr(&lhs.x), VectorLoadPtr(&rhs.x))) == 0xF;
	}

	[[nodiscard]] NO_ODR bool operator!=(const Quaternion& lhs, const Quaternion& rhs) noexcept
	{
		return !(lhs == rhs);
	}

	[[nodiscard]] NO_ODR Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs) noexcept
	{
		return Quaternion{ lhs } *= rhs;
	}

	[[nodiscard]] NO_ODR float operator|(const Quaternion& lhs, const Quaternion& rhs) noexcept
	{
		using namespace SIMD;
		float ret[4];
		VectorStorePtr(VectorMultiply(VectorLoadPtr(&lhs.x), VectorLoadPtr(&rhs.x)), ret);
		return ret[0] + ret[1] + ret[2] + ret[3];
	}

	// Global

	[[nodiscard]] NO_ODR bool IsNearlyEqual(const Quaternion& lhs,
		const Quaternion& rhs, float tolerance = Epsilon) noexcept
	{
		using namespace SIMD;
		const auto epsilon = VectorLoad1(tolerance);
		const auto vec = VectorSubtract(VectorLoadPtr(&lhs.x), VectorLoadPtr(&rhs.x));
		return VectorMoveMask(VectorLessEqual(vec, epsilon)) == 0xF;
	}

	[[nodiscard]] NO_ODR Quaternion Lerp(const Quaternion& a, const Quaternion& b, float t) noexcept
	{
		using namespace SIMD;
		const auto ratio = VectorLoad1(1.0f - t);
		const auto lhs = VectorLoadPtr(&a.x);
		const auto rhs = VectorLoadPtr(&b.x);
		
		auto result = VectorAdd(VectorMultiply(ratio, lhs), rhs);

		auto size = VectorMultiply(result, result);
		size = VectorHadd(size, size);
		size = VectorHadd(size, size);

		result = VectorMultiply(result, VectorInvSqrt(size));

		Quaternion ret;
		VectorStorePtr(result, &ret.x);
		return ret;
	}

	[[nodiscard]] NO_ODR Quaternion Slerp(const Quaternion& a, const Quaternion& b, float t) noexcept
	{
		using namespace SIMD;
		
		const float rawCosm = a | b;
		const float cosm = Abs(rawCosm);

		float scale0, scale1;
		if (cosm < 0.9999f)
		{
			const float omega = ACos(cosm);
			const float invSin = 1.f / Sin(omega);
			scale0 = Sin((1.f - t) * omega) * invSin;
			scale1 = Sin(t * omega) * invSin;
		}
		else
		{
			// Use linear interpolation.
			scale0 = 1.0f - t;
			scale1 = t;
		}

		const auto lhs = VectorLoadPtr(&a.x);
		const auto rhs = VectorLoadPtr(&b.x);
		const auto scaleLhs = VectorLoad1(scale0);
		const auto scaleRhs = VectorLoad1(rawCosm >= 0.0f ? scale1 : -scale1);

		auto result = VectorAdd(VectorMultiply(lhs, scaleLhs), VectorMultiply(rhs, scaleRhs));

		auto size = VectorMultiply(result, result);
		size = VectorHadd(size, size);
		size = VectorHadd(size, size);

		result = VectorMultiply(result, VectorInvSqrt(size));

		Quaternion ret;
		VectorStorePtr(result, &ret.x);
		return ret;
	}
}