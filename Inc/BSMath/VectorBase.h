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
		VectorBase<T, L> realMin = Min(min, max);
		VectorBase<T, L> realMax = Max(min, max);
		return Max(realMin, Min(realMax, n));
	}

	template <class T, size_t L>
	[[nodiscard]] NO_ODR VectorBase<T, L> Abs<VectorBase<T, L>>(const VectorBase<T, L>& n) noexcept
	{
		const __m128 signMask = _mm_set_ps1(-0.0f);
		const __m128 simd = _mm_set_ps(0.0f, 0.0f, n.y, n.x);
		return VectorBase<T, L>{ _mm_andnot_ps(signMask, simd) };
	}

	template <class T, size_t L>
	[[nodiscard]] NO_ODR VectorBase<T, L> Sign<VectorBase<T, L>>(const VectorBase<T, L>& n) noexcept
	{
		const __m128 zero = _mm_setzero_ps();
		const __m128 simd = _mm_set_ps(0.0f, 0.0f, n.y, n.x);
		const __m128 positive = _mm_and_ps(_mm_cmpgt_ps(simd, zero), _mm_set1_ps(1.0f));
		const __m128 negative = _mm_and_ps(_mm_cmplt_ps(simd, zero), _mm_set1_ps(-1.0f));
		return VectorBase<T, L>{ _mm_or_ps(positive, negative) };
	}

	template <size_t L>
	[[nodiscard]] NO_ODR bool IsNearlyEqual<VectorBase<float, L>>(const VectorBase<float, L>& lhs, const VectorBase<float, L>& rhs, float tolerance = Epsilon) noexcept
	{
		__m128 vec = _mm_set_ps(0.0f, 0.0f, lhs.y, lhs.x);
		vec = _mm_sub_ps(vec, _mm_set_ps(0.0f, 0.0f, rhs.y, rhs.x));
		return Detail::IsLessEpsilon(vec, tolerance);
	}

	[[nodiscard]] NO_ODR bool IsNearlyZero(const VectorBase<float, L>& vec, float tolerance = Epsilon) noexcept
	{
		return Detail::IsLessEpsilon(_mm_set_ps(0.0f, 0.0f, vec.y, vec.x), tolerance);
	}

	[[nodiscard]] NO_ODR VectorBase<T, L> GetRangePct(const VectorBase<float, L>& vec, const VectorBase<float, L>& min, const VectorBase<float, L>& max) noexcept
	{
		const __m128 vecSimd = _mm_set_ps(0.0f, 0.0f, vec.y, vec.x);
		const __m128 minSimd = _mm_set_ps(0.0f, 0.0f, min.y, min.x);
		const __m128 maxSimd = _mm_set_ps(0.0f, 0.0f, max.y, max.x);
		return VectorBase<T, L>{ _mm_div_ps(_mm_sub_ps(vecSimd, minSimd), _mm_sub_ps(maxSimd, minSimd)) };
	}

	[[nodiscard]] NO_ODR VectorBase<T, L> Lerp(const VectorBase<float, L>& a, const VectorBase<float, L>& b, float t) noexcept
	{
		return a + t * (b - a);
	}
}