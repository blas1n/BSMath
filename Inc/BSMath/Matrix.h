#pragma once

#include <array>
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
		bool Invert() noexcept;

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

	namespace Detail
	{
		template <size_t Idx>
		[[nodiscard]] NO_ODR decltype(auto) GetExpression
			(SIMD::VectorRegister<float> r2, SIMD::VectorRegister<float> r3) noexcept
		{
			using namespace SIMD;

			constexpr static Swizzle Swizzles[3][4][4]
			{
				{
					{ Swizzle::Z, Swizzle::Z, Swizzle::Y, Swizzle::Y },
					{ Swizzle::W, Swizzle::W, Swizzle::W, Swizzle::Z },
					{ Swizzle::Z, Swizzle::Z, Swizzle::Y, Swizzle::Y },
					{ Swizzle::W, Swizzle::W, Swizzle::W, Swizzle::Z }
				},
				{
					{ Swizzle::Y, Swizzle::X, Swizzle::X, Swizzle::X },
					{ Swizzle::W, Swizzle::W, Swizzle::W, Swizzle::Z },
					{ Swizzle::Y, Swizzle::X, Swizzle::X, Swizzle::X },
					{ Swizzle::W, Swizzle::W, Swizzle::W, Swizzle::Z }
				},
				{
					{ Swizzle::Y, Swizzle::X, Swizzle::X, Swizzle::X },
					{ Swizzle::Z, Swizzle::Z, Swizzle::Y, Swizzle::Y },
					{ Swizzle::Y, Swizzle::X, Swizzle::X, Swizzle::X },
					{ Swizzle::Z, Swizzle::Z, Swizzle::Y, Swizzle::Y }
				}
			};

			const auto v0 = VectorSwizzle<Swizzles[Idx][0][0],
				Swizzles[Idx][0][1], Swizzles[Idx][0][2], Swizzles[Idx][0][3]>(r2);

			const auto v1 = VectorSwizzle<Swizzles[Idx][1][0],
				Swizzles[Idx][1][1], Swizzles[Idx][1][2], Swizzles[Idx][1][3]>(r3);

			const auto v2 = VectorSwizzle<Swizzles[Idx][2][0],
				Swizzles[Idx][2][1], Swizzles[Idx][2][2], Swizzles[Idx][2][3]>(r3);

			const auto v3 = VectorSwizzle<Swizzles[Idx][3][0],
				Swizzles[Idx][3][1], Swizzles[Idx][3][2], Swizzles[Idx][3][3]>(r2);

			return VectorSubtract(VectorMultiply(v0, v1), VectorMultiply(v2, v3));
		}

		template <size_t L>
		decltype(auto) LoadMatrix(const Matrix<L>& mat)
		{
			using namespace SIMD;
			std::array<VectorRegister<float>, 4> ret;
			ret[0] = VectorLoad(mat.data[0]);

			if constexpr (L > 1)
				ret[1] = VectorLoad(mat.data[1]);
			else
				ret[1] = SIMD::Zero<float>;

			if constexpr (L > 2)
				ret[2] = VectorLoad(mat.data[2]);
			else
				ret[2] = SIMD::Zero<float>;

			if constexpr (L > 3)
				ret[3] = VectorLoad(mat.data[3]);
			else
				ret[3] = SIMD::Zero<float>;

			return ret;
		}
	}

	template <size_t L>
	float Matrix<L>::Determinant() const noexcept
	{
		using namespace SIMD;
		const auto mat = Detail::LoadMatrix(GetTranspose());

		auto e0 = Detail::GetExpression<0>(mat[2], mat[3]);
		auto e1 = Detail::GetExpression<1>(mat[2], mat[3]);
		auto e2 = Detail::GetExpression<2>(mat[2], mat[3]);
		
		e0 = VectorMultiply(e0, VectorSwizzle<Swizzle::Y, Swizzle::X, Swizzle::X, Swizzle::X>(mat[1]));
		e1 = VectorMultiply(e1, VectorSwizzle<Swizzle::Z, Swizzle::Z, Swizzle::Y, Swizzle::Y>(mat[1]));
		e2 = VectorMultiply(e2, VectorSwizzle<Swizzle::W, Swizzle::W, Swizzle::W, Swizzle::Z>(mat[1]));

		const auto cofactor = VectorAdd(VectorSubtract(e0, e1), e2);
		const auto det = VectorMultiply(cofactor, mat[0]);
		
		float ret[4];
		VectorStore(det, ret);
		return ret[0] - ret[1] + ret[2] - ret[3];
	}

	template <size_t L>
	Matrix<L> Matrix<L>::GetInvert() const noexcept
	{
		if (auto ret = *this; ret.Invert())
			return ret;
		return Identity;
	}
	
	namespace Detail
	{
		decltype(auto) Mat2Mul(SIMD::VectorRegister<float> lhs, SIMD::VectorRegister<float> rhs) noexcept
		{
			using namespace SIMD;
			const auto swi0 = VectorSwizzle<Swizzle::X, Swizzle::W, Swizzle::X, Swizzle::W>(rhs);
			const auto swi1 = VectorSwizzle<Swizzle::Y, Swizzle::X, Swizzle::W, Swizzle::Z>(lhs);
			const auto swi2 = VectorSwizzle<Swizzle::Z, Swizzle::Y, Swizzle::Z, Swizzle::Y>(rhs);
			const auto vec0 = VectorMultiply(lhs, swi0);
			const auto vec1 = VectorMultiply(swi1, swi2);
			return VectorAdd(vec0, vec1);
		};

		decltype(auto) Mat2AdjMul(SIMD::VectorRegister<float> lhs, SIMD::VectorRegister<float> rhs) noexcept
		{
			using namespace SIMD;
			const auto swi0 = VectorSwizzle<Swizzle::W, Swizzle::W, Swizzle::X, Swizzle::X>(lhs);
			const auto swi1 = VectorSwizzle<Swizzle::Y, Swizzle::Y, Swizzle::Z, Swizzle::Z>(lhs);
			const auto swi2 = VectorSwizzle<Swizzle::Z, Swizzle::W, Swizzle::X, Swizzle::Y>(rhs);
			const auto vec0 = VectorMultiply(swi0, rhs);
			const auto vec1 = VectorMultiply(swi1, swi2);
			return VectorSubtract(vec0, vec1);
		};

		decltype(auto) Mat2MulAdj(SIMD::VectorRegister<float> lhs, SIMD::VectorRegister<float> rhs) noexcept
		{
			using namespace SIMD;
			const auto swi0 = VectorSwizzle<Swizzle::W, Swizzle::X, Swizzle::W, Swizzle::X>(rhs);
			const auto swi1 = VectorSwizzle<Swizzle::Y, Swizzle::X, Swizzle::W, Swizzle::Z>(lhs);
			const auto swi2 = VectorSwizzle<Swizzle::Z, Swizzle::Y, Swizzle::Z, Swizzle::Y>(rhs);
			const auto vec0 = VectorMultiply(lhs, swi0);
			const auto vec1 = VectorMultiply(swi1, swi2);
			return VectorSubtract(vec0, vec1);
		};
	}

	template <size_t L>
	bool Matrix<L>::Invert() noexcept
	{
		// Source: https://lxjk.github.io/2017/09/03/Fast-4x4-Matrix-Inverse-with-SSE-SIMD-Explained.html

		using namespace SIMD;
		using namespace Detail;

		if (Determinant() == 0.0f)
			return false;

		const auto mat = LoadMatrix(*this);

		const auto a = VectorShuffle0101(mat[0], mat[1]);
		const auto b = VectorShuffle2323(mat[0], mat[1]);
		const auto c = VectorShuffle0101(mat[2], mat[3]);
		const auto d = VectorShuffle2323(mat[2], mat[3]);

		const auto shuffle0 = VectorShuffle<Swizzle::X, Swizzle::Z, Swizzle::X, Swizzle::Z>(mat[0], mat[2]);
		const auto shuffle1 = VectorShuffle<Swizzle::Y, Swizzle::W, Swizzle::Y, Swizzle::W>(mat[1], mat[3]);
		const auto shuffle2 = VectorShuffle<Swizzle::Y, Swizzle::W, Swizzle::Y, Swizzle::W>(mat[0], mat[2]);
		const auto shuffle3 = VectorShuffle<Swizzle::X, Swizzle::Z, Swizzle::X, Swizzle::Z>(mat[1], mat[3]);
		const auto detSub = VectorSubtract(VectorMultiply(shuffle0, shuffle1), VectorMultiply(shuffle2, shuffle3));

		const auto detA = VectorSwizzle<Swizzle::X, Swizzle::X, Swizzle::X, Swizzle::X>(detSub);
		const auto detB = VectorSwizzle<Swizzle::Y, Swizzle::Y, Swizzle::Y, Swizzle::Y>(detSub);
		const auto detC = VectorSwizzle<Swizzle::Z, Swizzle::Z, Swizzle::Z, Swizzle::Z>(detSub);
		const auto detD = VectorSwizzle<Swizzle::W, Swizzle::W, Swizzle::W, Swizzle::W>(detSub);

		const auto ab = Mat2AdjMul(a, b);
		const auto dc = Mat2AdjMul(d, c);
		
		auto x = VectorSubtract(VectorMultiply(detD, a), Mat2Mul(b, dc));
		auto w = VectorSubtract(VectorMultiply(detA, d), Mat2Mul(c, ab));
		auto y = VectorSubtract(VectorMultiply(detB, c), Mat2MulAdj(d, ab));
		auto z = VectorSubtract(VectorMultiply(detC, b), Mat2MulAdj(a, dc));

		auto tr = VectorMultiply(ab, VectorSwizzle<Swizzle::X, Swizzle::Z, Swizzle::Y, Swizzle::W>(dc));
		tr = VectorHadd(tr, tr);
		tr = VectorHadd(tr, tr);

		const auto detM = VectorSubtract(VectorAdd(VectorMultiply(detA, detD), VectorMultiply(detB, detC)), tr);
		const auto adjSignMask = VectorLoad(1.0f, -1.0f, -1.0f, 1.0f);
		const auto rDetM = VectorDivide(adjSignMask, detM);

		x = VectorMultiply(x, rDetM);
		y = VectorMultiply(y, rDetM);
		z = VectorMultiply(z, rDetM);
		w = VectorMultiply(w, rDetM);

		Matrix<4> ret;
		VectorStore(VectorShuffle<Swizzle::W, Swizzle::Y, Swizzle::W, Swizzle::Y>(x, y), ret.data[0]);
		VectorStore(VectorShuffle<Swizzle::Z, Swizzle::X, Swizzle::Z, Swizzle::X>(x, y), ret.data[1]);
		VectorStore(VectorShuffle<Swizzle::W, Swizzle::Y, Swizzle::W, Swizzle::Y>(z, w), ret.data[2]);
		VectorStore(VectorShuffle<Swizzle::Z, Swizzle::X, Swizzle::Z, Swizzle::X>(z, w), ret.data[3]);
		*this = Matrix<L>(ret);
		return true;
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
				const auto rhs = VectorLoad(operand.data[j]);

				alignas(16) float tmp[L];
				VectorStore(VectorMultiply(lhs, rhs), tmp);

				float sum = 0.0f;
				for (const auto elem : tmp)
					sum += elem;
				data[i][j] = sum;
			}
		}
		return *this;
	}

	// Global Operator

	template <size_t L>
	[[nodiscard]] NO_ODR Matrix<L> operator*(const Matrix<L>& lhs, const Matrix<L>& rhs) noexcept
	{
		return Matrix<L>{ lhs } *= rhs;
	}

	// Global Function

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