#pragma once

#include "MatrixBase0.h"

namespace BSMath::Detail
{
	template <class T, size_t L>
	struct alignas(16) VectorBase : public MatrixBase0<T, L, 1>
	{
	public:
		using Super = MatrixBase0<T, L, 1>;

		inline const static VectorBase Zero;
		inline const static VectorBase One;

	public:
		using Super::Super;

		template <class U, size_t L2>
		VectorBase(const VectorBase<U, L2>& other) explicit
			: Super()
		{
			std::copy_n(data, L2, other.data);
		}

		[[nodiscard]] constexpr T GetMin() const noexcept { return Min(data, data + L); }
		[[nodiscard]] constexpr T GetMax() const noexcept { return Max(data, data + L); }

		[[nodiscard]] inline float Length() const noexcept
		{
			return Sqrt(LengthSquared());
		}

		[[nodiscard]] float LengthSquared() const noexcept
		{
			return VectorBase<T, L>::Dot(*this, *this);
		}

		[[nodiscard]] VectorBase GetNormal() const noexcept
		{
			VectorBase ret = *this;
			return ret.Normalize() ? ret : Zero;
		}

		bool Normalize() noexcept;

		[[nodiscard]] static float Distance(const VectorBase& lhs, const VectorBase& rhs)
		{
			return Sqrt(DistanceSquared(lhs, rhs));
		}

		[[nodiscard]] static float DistanceSquared(const VectorBase& lhs, const VectorBase& rhs)
		{
			return (lhs - rhs).LengthSquared();
		}

		[[nodiscard]] constexpr T& operator[](size_t idx) noexcept { return data[idx]; }
		[[nodiscard]] constexpr T operator[](size_t idx) const noexcept { return data[idx]; }

		[[nodiscard]] VectorBase operator-() const noexcept;
		VectorBase& operator*=(const VectorBase& other) noexcept;
		VectorBase& operator/=(const VectorBase& other) noexcept;

		[[nodiscard]] static T Dot(const VectorBase& lhs, const VectorBase& rhs) noexcept;
	};

	template <class T, size_t L>
	const VectorBase<T, L> VectorBase<T, L>::Zero{ 0.0f };

	template <class T, size_t L>
	const VectorBase<T, L> VectorBase<T, L>::One{ 1.0f };

	template <class T, size_t L>
	bool VectorBase<T, L>::Normalize() noexcept
	{
		BRANCH_SIMD(T);

		const float lengthSquared = LengthSquared();
		if (IsNearlyZero(lengthSquared))
			return false;

		const auto vec = LoadRow(data[0]);
		const auto size = SIMD::VectorLoad1(InvSqrt(lengthSquared));
		VectorStore(VectorMultiply(vec, size), data[0]);
		return true;
	}

	template <class T, size_t L>
	VectorBase<T, L> VectorBase<T, L>::operator-() const noexcept
	{
		return VectorBase<T, L>::Zero - *this;
	}

	template <class T, size_t L>
	VectorBase<T, L>& VectorBase<T, L>:: operator*=(const VectorBase<T, L>& other) noexcept
	{
		BRANCH_SIMD(T);
		const auto lhs = LoadRow(data[0]);
		const auto rhs = LoadRow(other.data[0]);
		VectorStore(VectorMultiply(lhs, rhs), data[0]);
		return *this;
	}

	template <class T, size_t L>
	VectorBase<T, L>& VectorBase<T, L>:: operator/=(const VectorBase<T, L>& other) noexcept
	{
		BRANCH_SIMD(T);
		const auto lhs = LoadRow(data[0]);
		const auto rhs = LoadRow(other.data[0]);
		VectorStore(VectorDivide(lhs, rhs), data[0]);
		return *this;
	}

	template <class T, size_t L>
	T VectorBase<T, L>::Dot(const VectorBase<T, L>& lhs, const VectorBase<T, L>& rhs) noexcept
	{
		auto mul = lhs; mul *= rhs;

		T ret{ 0 };
		for (size_t i = 0; i < L; ++i)
			ret += mul.data[0][i];
		return ret;
	}

	// Global Operator

	template <class T, size_t L>
	NO_ODR VectorBase<T, L> operator*(const VectorBase<T, L>& lhs, const VectorBase<T, L>& rhs) noexcept
	{
		return VectorBase<T, L>{ lhs } *= rhs;
	}

	template <class T, size_t L>
	NO_ODR VectorBase<T, L> operator/(const VectorBase<T, L>& lhs, const VectorBase<T, L>& rhs) noexcept
	{
		return VectorBase<T, L>{ lhs } *= rhs;
	}

	template <class T, size_t L>
	NO_ODR T operator|(const VectorBase<T, L>& lhs, const VectorBase<T, L>& rhs) noexcept
	{
		return VectorBase<T, L>::Dot(lhs, rhs);
	}
}