#pragma once

#include "Vector.h"

namespace BSMath
{
	struct alignas(16) Rotator final
	{
	public:
		const static Rotator Zero;

	public:
		constexpr Rotator() noexcept
			: roll(0.0f), pitch(0.0f), yaw(0.0f) {}

		explicit constexpr Rotator(float angle) noexcept
			: roll(angle), pitch(angle), yaw(angle) {}

		explicit constexpr Rotator(float inRoll, float inPitch, float inYaw) noexcept
			: roll(inRoll), pitch(inPitch), yaw(inYaw) {}

		explicit constexpr Rotator(const Vector3& vec) noexcept
			: roll(vec.x), pitch(vec.y), yaw(vec.z) {}

		explicit constexpr Rotator(const float* ptr) noexcept
			: roll(ptr[0]), pitch(ptr[1]), yaw(ptr[2]) {}

		void Set(float inRoll, float inPitch, float inYaw) noexcept
		{
			roll = inRoll; pitch = inPitch; yaw = inYaw;
		}

		[[nodiscard]] Rotator operator-() const noexcept;

		Rotator& operator+=(const Rotator& other) noexcept;
		Rotator& operator-=(const Rotator& other) noexcept;

		Rotator& operator*=(float scaler) noexcept;
		Rotator& operator/=(float divisor) noexcept;

		[[nodiscard]] constexpr float& operator[](size_t idx) noexcept { return (&roll)[idx]; }
		[[nodiscard]] constexpr float operator[](size_t idx) const noexcept { return (&roll)[idx]; }

	public:
		float roll;
		float pitch;
		float yaw;
	};

	// Global Operators

	[[nodiscard]] NO_ODR bool operator==(const Rotator& lhs, const Rotator& rhs) noexcept
	{
		using namespace SIMD;
		const auto lhsVec = VectorLoadPtr(&lhs.roll, 3);
		const auto rhsVec = VectorLoadPtr(&rhs.roll, 3);
		return VectorMoveMask(VectorEqual(lhsVec, rhsVec)) == 0xF;
	}

	[[nodiscard]] NO_ODR bool operator!=(const Rotator& lhs, const Rotator& rhs) noexcept { return !(lhs == rhs); }

	[[nodiscard]] NO_ODR Rotator operator+(const Rotator& lhs, const Rotator& rhs) noexcept
	{
		return Rotator{ lhs } += rhs;
	}

	[[nodiscard]] NO_ODR Rotator operator-(const Rotator& lhs, const Rotator& rhs) noexcept
	{
		return Rotator{ lhs } -= rhs;
	}

	[[nodiscard]] NO_ODR Rotator operator*(const Rotator& vec, float scaler) noexcept
	{
		return Rotator{ vec } *= scaler;
	}

	[[nodiscard]] NO_ODR Rotator operator*(float scaler, const Rotator& vec) noexcept
	{
		return Rotator{ vec } *= scaler;
	}

	[[nodiscard]] NO_ODR Rotator operator/(const Rotator& vec, float divisor) noexcept
	{
		return Rotator{ vec } /= divisor;
	}

	const Rotator Rotator::Zero{};

	Rotator Rotator::operator-() const noexcept
	{
		return Rotator::Zero - *this;
	}

	Rotator& Rotator::operator+=(const Rotator& other) noexcept
	{
		using namespace SIMD;
		const auto lhs = VectorLoadPtr(&roll, 3);
		const auto rhs = VectorLoadPtr(&other.roll, 3);
		VectorStorePtr(VectorAdd(lhs, rhs), &roll, 3);
		return *this;
	}

	Rotator& Rotator::operator-=(const Rotator& other) noexcept
	{
		using namespace SIMD;
		const auto lhs = VectorLoadPtr(&roll, 3);
		const auto rhs = VectorLoadPtr(&other.roll, 3);
		VectorStorePtr(VectorSubtract(lhs, rhs), &roll, 3);
		return *this;
	}

	Rotator& Rotator::operator*=(float scaler) noexcept
	{
		using namespace SIMD;
		const auto lhs = VectorLoadPtr(&roll, 3);
		const auto rhs = VectorLoad1(scaler);
		VectorStorePtr(VectorMultiply(lhs, rhs), &roll, 3);
		return *this;
	}

	Rotator& Rotator::operator/=(float divisor) noexcept
	{
		if (divisor == 0.0f) return *this;

		using namespace SIMD;
		const auto lhs = VectorLoadPtr(&roll, 3);
		const auto rhs = VectorLoad1(divisor);
		VectorStorePtr(VectorDivide(lhs, rhs), &roll, 3);
		return *this;
	}
}