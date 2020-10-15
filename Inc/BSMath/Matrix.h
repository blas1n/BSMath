#pragma once

#include "MatrixBase0.h"
#include "Utility.h"

namespace BSMath
{
	template <size_t L>
	struct Matrix final : public Detail::MatrixBase0<float, L, L>
	{
	public:
		using Super = MatrixBase0<float, L, L>;
		using Super::Super;
		using Super::data;

		const static Matrix Zero;
		const static Matrix One;
		const static Matrix Identity;

	public:
		[[nodiscard]] float Determinant() const noexcept;

		[[nodiscard]] Matrix GetInvert() const noexcept;
		void Invert() noexcept;

		[[nodiscard]] Matrix GetTranspose() const noexcept;
		void Transpose() noexcept;

		Matrix& operator*=(const Matrix& other) noexcept;

		[[nodiscard]] float* operator[](size_t idx) noexcept { return data[idx]; }
		[[nodiscard]] const float* operator[](size_t idx) const noexcept { return data[idx]; }
	};

	namespace Detail
	{
		template <size_t L>
		static Matrix<L> GetIdentity() noexcept
		{
			Matrix<L> ret;
			for (size_t i = 0; i < L; ++i)
				ret.data[i][i] = 1.0f;
			return ret;
		}
	}

	template <size_t L>
	const Matrix<L> Matrix<L>::Zero;

	template <size_t L>
	const Matrix<L> Matrix<L>::One(1.0f);

	template <size_t L>
	const Matrix<L> Matrix<L>::Identity = Detail::GetIdentity<L>();

	template <size_t L>
	float Matrix<L>::Determinant() const noexcept
	{

	}

	template <size_t L>
	Matrix<L> Matrix<L>::GetInvert() const noexcept
	{

	}
	
	template <size_t L>
	void Matrix<L>::Invert() noexcept
	{

	}

	template <size_t L>
	Matrix<L> Matrix<L>::GetTranspose() const noexcept
	{
		Matrix<L> ret;
		for (size_t i = 0; i < L; ++i)
			for (size_t j = 0; j < L; ++j)
				ret[j][i] = data[i][j];
		return ret;
	}
	
	template <size_t L>
	void Matrix<L>::Transpose() noexcept
	{
		for (size_t i = 1; i < L; ++i)
			for (size_t j = 0; j < i; ++j)
				std::swap(data[i][j], data[j][i]);
	}

	template <size_t L>
	Matrix<L>& Matrix<L>::operator*=(const Matrix<L>& other) noexcept
	{
		using namespace SIMD;
		const auto operand = other.GetTranspose();
		for (size_t i = 0; i < L; ++i)
		{
			const auto lhs = VectorLoad(data[i]);
			for (size_t j = 0; j < L; ++j)
			{
				const auto rhs = VectorLoad(operand[i]);

				alignas(16) float tmp[L];
				VectorStore(VectorMultiply(lhs, rhs), tmp);

				float sum = 0.0f;
				for (const auto elem : tmp)
					sum += elem;
				data[i][j] = sum;
			}
		}
	}

	template <size_t L>
	[[nodiscard]] NO_ODR bool IsNearlyEqual(const Matrix<L>& lhs,
		const Matrix<L>& rhs, float tolerance = Epsilon) noexcept
	{
		using namespace SIMD;
		const auto epsilon = VectorLoad1(tolerance);
		const auto vec = VectorSubtract(VectorLoad(lhs.data[0]), VectorLoad(rhs.data[0]));
		return VectorMoveMask(VectorLessEqual(vec, epsilon)) == 0xF;
	}

	template <size_t L>
	[[nodiscard]] NO_ODR bool IsNearlyZero(const Matrix<L>& mat, float tolerance = Epsilon) noexcept
	{
		using namespace SIMD;
		const auto epsilon = VectorLoad1(tolerance);
		const auto vecSimd = VectorLoad(vec.data[0]);
		return VectorMoveMask(VectorLessEqual(vecSimd, epsilon)) == 0xF;
	}
}