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
	}

	template <size_t L>
	float Matrix<L>::Determinant() const noexcept
	{
		using namespace SIMD;
		const auto self = GetTranspose();

		VectorRegister<float> r0 = VectorLoad(self.data[0]);
		VectorRegister<float> r1, r2, r3;

		if constexpr (L > 1)
			r1 = VectorLoad(self.data[1]);
		else
			r1 = SIMD::Zero<float>;

		if constexpr (L > 2)
			r2 = VectorLoad(self.data[2]);
		else
			r2 = SIMD::Zero<float>;

		if constexpr (L > 3)
			r3 = VectorLoad(self.data[3]);
		else
			r3 = SIMD::Zero<float>;

		auto e0 = Detail::GetExpression<0>(r2, r3);
		auto e1 = Detail::GetExpression<1>(r2, r3);
		auto e2 = Detail::GetExpression<2>(r2, r3);
		
		e0 = VectorMultiply(e0, VectorSwizzle<Swizzle::Y, Swizzle::X, Swizzle::X, Swizzle::X>(r1));
		e1 = VectorMultiply(e1, VectorSwizzle<Swizzle::Z, Swizzle::Z, Swizzle::Y, Swizzle::Y>(r1));
		e2 = VectorMultiply(e2, VectorSwizzle<Swizzle::W, Swizzle::W, Swizzle::W, Swizzle::Z>(r1));

		const auto cofactor = VectorAdd(VectorSubtract(e0, e1), e2);
		const auto det = VectorMultiply(cofactor, r0);
		
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

		auto A = VecShuffle_	0101(inM.mVec[0], inM.mVec[1]);
		auto B = VecShuffle_2323(inM.mVec[0], inM.mVec[1]);
		auto C = VecShuffle_0101(inM.mVec[2], inM.mVec[3]);
		auto D = VecShuffle_2323(inM.mVec[2], inM.mVec[3]);

		auto detSub = _mm_sub_ps(
			_mm_mul_ps(VecShuffle(inM.mVec[0], inM.mVec[2], 0, 2, 0, 2), VecShuffle(inM.mVec[1], inM.mVec[3], 1, 3, 1, 3)),
			_mm_mul_ps(VecShuffle(inM.mVec[0], inM.mVec[2], 1, 3, 1, 3), VecShuffle(inM.mVec[1], inM.mVec[3], 0, 2, 0, 2))
		);
		auto detA = VecSwizzle1(detSub, 0);
		auto detB = VecSwizzle1(detSub, 1);
		auto detC = VecSwizzle1(detSub, 2);
		auto detD = VecSwizzle1(detSub, 3);

		auto D_C = Mat2AdjMul(D, C);
		// A#B
		auto A_B = Mat2AdjMul(A, B);
		// X# = |D|A - B(D#C)
		auto X_ = _mm_sub_ps(_mm_mul_ps(detD, A), Mat2Mul(B, D_C));
		// W# = |A|D - C(A#B)
		auto W_ = _mm_sub_ps(_mm_mul_ps(detA, D), Mat2Mul(C, A_B));

		// |M| = |A|*|D| + ... (continue later)
		auto detM = _mm_mul_ps(detA, detD);

		// Y# = |B|C - D(A#B)#
		auto Y_ = _mm_sub_ps(_mm_mul_ps(detB, C), Mat2MulAdj(D, A_B));
		// Z# = |C|B - A(D#C)#
		auto Z_ = _mm_sub_ps(_mm_mul_ps(detC, B), Mat2MulAdj(A, D_C));

		// |M| = |A|*|D| + |B|*|C| ... (continue later)
		detM = _mm_add_ps(detM, _mm_mul_ps(detB, detC));

		// tr((A#B)(D#C))
		auto tr = _mm_mul_ps(A_B, VecSwizzle(D_C, 0, 2, 1, 3));
		tr = _mm_hadd_ps(tr, tr);
		tr = _mm_hadd_ps(tr, tr);
		// |M| = |A|*|D| + |B|*|C| - tr((A#B)(D#C)
		detM = _mm_sub_ps(detM, tr);

		const auto adjSignMask = _mm_setr_ps(1.f, -1.f, -1.f, 1.f);
		// (1/|M|, -1/|M|, -1/|M|, 1/|M|)
		auto rDetM = _mm_div_ps(adjSignMask, detM);

		X_ = _mm_mul_ps(X_, rDetM);
		Y_ = _mm_mul_ps(Y_, rDetM);
		Z_ = _mm_mul_ps(Z_, rDetM);
		W_ = _mm_mul_ps(W_, rDetM);

		Matrix<L> ret;

		// apply adjugate and store, here we combine adjugate shuffle and store shuffle
		ret.mVec[0] = VecShuffle(X_, Y_, 3, 1, 3, 1);
		ret.mVec[1] = VecShuffle(X_, Y_, 2, 0, 2, 0);
		ret.mVec[2] = VecShuffle(Z_, W_, 3, 1, 3, 1);
		ret.mVec[3] = VecShuffle(Z_, W_, 2, 0, 2, 0);

		return r;
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