#pragma once

#include "MatrixBase0.h"
#include "Utility.h"

#ifndef x
#define x data[0][0]
#endif

#ifndef y
#define y data[0][1]
#endif

#ifndef z
#define z data[0][2]
#endif

#ifndef w
#define w data[0][3]
#endif

namespace BSMath
{
	namespace Detail
	{
		template <class T, size_t L>
		struct VectorBase : public MatrixBase0<T, 1, L>
		{
		public:
			using Super = MatrixBase0<T, 1, L>;
			using Super::Super;
			using Super::data;

			const static VectorBase Zero;
			const static VectorBase One;

		public:
			[[nodiscard]] constexpr T GetMin() const noexcept { return Min(*data, *data + L); }
			[[nodiscard]] constexpr T GetMax() const noexcept { return Max(*data, *data + L); }

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
				return VectorBase(lhs - rhs).LengthSquared();
			}

			[[nodiscard]] constexpr T& operator[](size_t idx) noexcept { return data[0][idx]; }
			[[nodiscard]] constexpr T operator[](size_t idx) const noexcept { return data[0][idx]; }

			[[nodiscard]] VectorBase operator-() const noexcept;
			VectorBase& operator*=(const VectorBase& other) noexcept;
			VectorBase& operator/=(const VectorBase& other) noexcept;

			[[nodiscard]] static T Dot(const VectorBase& lhs, const VectorBase& rhs) noexcept;
		};

		template <class T, size_t L>
		const VectorBase<T, L> VectorBase<T, L>::Zero;

		template <class T, size_t L>
		const VectorBase<T, L> VectorBase<T, L>::One(1.0f);

		template <class T, size_t L>
		bool VectorBase<T, L>::Normalize() noexcept
		{
			const float lengthSquared = LengthSquared();
			if (IsNearlyZero(lengthSquared))
				return false;

			using namespace SIMD;
			const auto vec = VectorLoad(data[0]);
			const auto size = VectorLoad1(InvSqrt(lengthSquared));
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
			using namespace SIMD;
			const auto lhs = VectorLoad(data[0]);
			const auto rhs = VectorLoad(other.data[0]);
			VectorStore(VectorMultiply(lhs, rhs), data[0]);
			return *this;
		}

		template <class T, size_t L>
		VectorBase<T, L>& VectorBase<T, L>:: operator/=(const VectorBase<T, L>& other) noexcept
		{
			for (size_t i = 0; i < L; ++i)
				if (IsNearlyZero(other.data[0][i]))
					return *this;

			using namespace SIMD;
			const auto lhs = VectorLoad(data[0]);
			const auto rhs = VectorLoad(other.data[0]);
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
		[[nodiscard]] NO_ODR VectorBase<T, L> operator*(const VectorBase<T, L>& lhs, const VectorBase<T, L>& rhs) noexcept
		{
			return VectorBase<T, L>{ lhs } *= rhs;
		}

		template <class T, size_t L>
		[[nodiscard]] NO_ODR VectorBase<T, L> operator/(const VectorBase<T, L>& lhs, const VectorBase<T, L>& rhs) noexcept
		{
			return VectorBase<T, L>{ lhs } /= rhs;
		}

		template <class T, size_t L>
		[[nodiscard]] NO_ODR T operator|(const VectorBase<T, L>& lhs, const VectorBase<T, L>& rhs) noexcept
		{
			return VectorBase<T, L>::Dot(lhs, rhs);
		}
	}

	// Global Function

	template <class T, size_t L>
	[[nodiscard]] NO_ODR Detail::VectorBase<T, L> Min(const Detail::VectorBase<T, L>& lhs, const Detail::VectorBase<T, L>& rhs) noexcept
	{
		using namespace SIMD;

		Detail::VectorBase<T, L> ret;
		VectorStore(VectorMin(VectorLoad(lhs.data[0]), VectorLoad(rhs.data[0])), ret.data[0]);
		return ret;
	}

	template <class T, size_t L>
	[[nodiscard]] NO_ODR Detail::VectorBase<T, L> Max(const Detail::VectorBase<T, L>& lhs, const Detail::VectorBase<T, L>& rhs) noexcept
	{
		using namespace SIMD;

		Detail::VectorBase<T, L> ret;
		VectorStore(VectorMax(VectorLoad(lhs.data[0]), VectorLoad(rhs.data[0])), ret.data[0]);
		return ret;
	}

	template <class T, size_t L>
	[[nodiscard]] NO_ODR Detail::VectorBase<T, L> Clamp(const Detail::VectorBase<T, L>& n, const Detail::VectorBase<T, L>& min, const Detail::VectorBase<T, L>& max) noexcept
	{
		auto realMin = Min(min, max);
		auto realMax = Max(min, max);
		return Max(realMin, Min(realMax, n));
	}

	template <size_t L>
	[[nodiscard]] NO_ODR Detail::VectorBase<int, L> Abs(const Detail::VectorBase<int, L>& n) noexcept
	{
		using namespace SIMD;
		auto point = VectorLoad(n.data[0]);
		auto mask = VectorLessThan(point, Zero<int>);
		point = VectorXor(point, mask);
		mask = VectorAnd(mask, One<int>);

		Detail::VectorBase<float, L> ret;
		VectorStore(VectorAdd(point, mask), ret.data[0]);
		return ret;
	}

	template <size_t L>
	[[nodiscard]] NO_ODR Detail::VectorBase<float, L> Abs(const Detail::VectorBase<float, L>& n) noexcept
	{
		using namespace SIMD;
		const auto vec = VectorLoad(n.data[0]);
		const auto mask = VectorLoad1(-0.0f);

		Detail::VectorBase<float, L> ret;
		VectorStore(VectorAndNot(mask, vec), ret.data[0]);
		return ret;
	}

	template <class T, size_t L>
	[[nodiscard]] NO_ODR Detail::VectorBase<T, L> Sign(const Detail::VectorBase<T, L>& n) noexcept
	{
		using namespace SIMD;
		const auto vec = VectorLoad(n.data[0]);
		const auto positive = VectorAnd(VectorGreaterThan(vec, Zero<T>), One<T>);
		const auto negative = VectorAnd(VectorLessThan(vec, Zero<T>), VectorLoad1(static_cast<T>(-1)));

		Detail::VectorBase<float, L> ret;
		VectorStore(VectorOr(positive, negative), ret.data[0]);
		return ret;
	}

	template <size_t L>
	[[nodiscard]] NO_ODR bool IsNearlyEqual(const Detail::VectorBase<float, L>& lhs,
		const Detail::VectorBase<float, L>& rhs, float tolerance = Epsilon) noexcept
	{
		using namespace SIMD;
		const auto epsilon = VectorLoad1(tolerance);
		const auto vec = VectorSubtract(VectorLoad(lhs.data[0]), VectorLoad(rhs.data[0]));
		return VectorMoveMask(VectorLessEqual(vec, epsilon)) == 0xF;
	}

	template <size_t L>
	[[nodiscard]] NO_ODR bool IsNearlyZero(const Detail::VectorBase<float, L>& vec, float tolerance = Epsilon) noexcept
	{
		using namespace SIMD;
		const auto epsilon = VectorLoad1(tolerance);
		const auto vecSimd = VectorLoad(vec.data[0]);
		return VectorMoveMask(VectorLessEqual(vecSimd, epsilon)) == 0xF;
	}

	template <size_t L>
	[[nodiscard]] NO_ODR Detail::VectorBase<float, L> GetRangePct(const Detail::VectorBase<float, L>& vec,
		const Detail::VectorBase<float, L>& min, const Detail::VectorBase<float, L>& max) noexcept
	{
		using namespace SIMD;
		const auto vecSimd = VectorLoad(vec.data[0]);
		const auto minSimd = VectorLoad(min.data[0]);
		const auto maxSimd = VectorLoad(max.data[0]);
		const auto numerator = VectorSubtract(vecSimd, minSimd);
		const auto denomirator = VectorSubtract(maxSimd, minSimd);

		Detail::VectorBase<float, L> ret;
		VectorStore(VectorDivide(numerator, denomirator), ret.data[0]);
		return ret;
	}
}