#pragma once

#include "MatrixBase0.h"

namespace BSMath::Detail
{
	template <class T, size_t L>
	struct alignas(16) VectorBase : public MatrixBase0<T, L, 1>
	{
	public:
		using Super = MatrixBase0<T, L, 1>;

		NO_ODR const static VectorBase Zero;
		NO_ODR const static VectorBase One;

	public:
		using Super::Super;

		template <class U, size_t L2>
		explicit VectorBase(const VectorBase<U, L2>& other) noexcept
			: Super()
		{
			std::copy_n(data, L2, other.data);
		}

		void Set(std::initializer_list<T> list) noexcept
		{
			std::copy_n(data, L, list.begin());
		}

		[[nodiscard]] constexpr T GetMin() const noexcept { return Min(data, data + L); }
		[[nodiscard]] constexpr T GetMax() const noexcept { return Max(data, data + L); }

		[[nodiscard]] NO_ODR float Length() const noexcept
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

	// Global Function

	template <class T, size_t L>
	[[nodiscard]] NO_ODR auto Min<VectorBase<T, L>>(const VectorBase<T, L>& lhs, const VectorBase<T, L>& rhs) noexcept
	{
		BRANCH_SIMD(T);
		Vector<T, L> ret;
		VectorStore(VectorMin(LoadRow(lhs.data[0]), LoadRow(rhs.data[0])), ret.data[0]);
		return ret;
	}

	template <class T, size_t L>
	[[nodiscard]] NO_ODR auto Max<VectorBase<T, L>>(const VectorBase<T, L>& lhs, const VectorBase<T, L>& rhs) noexcept
	{
		BRANCH_SIMD(T);
		Vector<T, L> ret;
		VectorStore(VectorMax(LoadRow(lhs.data[0]), LoadRow(rhs.data[0])), ret.data[0]);
		return ret;
	}

	template <class T, size_t L>
	[[nodiscard]] NO_ODR auto Clamp<VectorBase<T, L>>(const VectorBase<T, L>& n, const VectorBase<T, L>& min, const VectorBase<T, L>& max) noexcept
	{
		auto realMin = Min(min, max);
		auto realMax = Max(min, max);
		return Max(realMin, Min(realMax, n));
	}

	template <size_t L>
	[[nodiscard]] NO_ODR VectorBase<int, L> Abs<VectorBase<int, L>>(const VectorBase<int, L>& n) noexcept
	{
		using namespace SIMD::Integer;

		auto point = LoadRow(n.data[0]);
		auto mask = VectorLessThan(point, Zero);
		point = VectorXor(point, mask);
		mask = VectorAnd(mask, One);
		return VectorAdd(point, mask);
	}

	template <size_t L>
	[[nodiscard]] NO_ODR VectorBase<float, L> Abs<VectorBase<float, L>>(const VectorBase<float, L>& n) noexcept
	{
		using namespace SIMD::Real;
		const auto vec = LoadRow(n.data[0]);
		const auto mask = VectorLoad1(-0.0f);
		return VectorAndNot(mask, vec);
	}

	template <class T, size_t L>
	[[nodiscard]] NO_ODR VectorBase<T, L> Sign<VectorBase<T, L>>(const VectorBase<T, L>& n) noexcept
	{
		BRANCH_SIMD(T);
		const auto vec = LoadRow(n.data[0]);
		const auto positive = VectorAnd(VectorGreaterThan(vec, Zero), One);
		const auto negative = VectorAnd(VectorLessThan(vec, Zero), VectorLoadOne(static_cast<T>(-1));
		return VectorOr(positive, negative);
	}

	template <size_t L>
	[[nodiscard]] NO_ODR bool IsNearlyEqual<VectorBase<float, L>>(const VectorBase<float, L>& lhs, const VectorBase<float, L>& rhs, float tolerance = Epsilon) noexcept
	{
		using namespace SIMD::Real;
		const auto epsilon = VectorLoad1(tolerance);
		const auto vec = VectorSubtract(LoadRow(lhs.data[0]), LoadRow(rhs.data[0]);
		return VectorMoveMask(VectorLessEqual(vec, epsilon)) == 0xF;
	}

	[[nodiscard]] NO_ODR bool IsNearlyZero(const VectorBase<float, L>& vec, float tolerance = Epsilon) noexcept
	{
		using namespace SIMD::Real;
		const auto epsilon = VectorLoad1(tolerance);
		const auto vecSimd = LoadRow(vec.data[0]);
		return VectorMoveMask(VectorLessEqual(vecSimd, epsilon)) == 0xF;
	}

	[[nodiscard]] NO_ODR VectorBase<T, L> GetRangePct(const VectorBase<float, L>& vec, const VectorBase<float, L>& min, const VectorBase<float, L>& max) noexcept
	{
		using namespace SIMD::Real;
		const auto vecSimd = LoadRow(vec.data[0]);
		const auto minSimd = LoadRow(min.data[0]);
		const auto maxSimd = LoadRow(max.data[0]);
		return VectorDivide(VectorSubtract(vecSimd, minSimd), VectorSubtract(maxSimd, minSimd));
	}
}