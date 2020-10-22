#pragma once

#include <array>
#include "Utility.h"

namespace BSMath
{
	template <class T, size_t L>
	struct Matrix final
	{
	public:
		static const Matrix Zero;
		static const Matrix One;
		static const Matrix Identity;

	public:
		constexpr Matrix() noexcept : data() {}

		explicit Matrix(T n) noexcept : data()
		{
			std::fill_n(*data, L * L, n);
		}

		explicit Matrix(const T* ptr) noexcept : data()
		{
			std::copy_n(ptr, L * L, *data);
		}

		template <class... Args>
		explicit Matrix(T x, T y, Args ... args) noexcept : data()
		{
			static_assert(sizeof...(Args) + 2 == L * L, "Too many arguments");
			const std::initializer_list<T> list{ x, y, static_cast<T>(args)... };
			std::copy(list.begin(), list.end(), *data);
		}

		[[nodiscard]] float Determinant() const noexcept;

		[[nodiscard]] Matrix GetInvert() const noexcept;
		bool Invert() noexcept;

		[[nodiscard]] Matrix GetTranspose() const noexcept;
		void Transpose() noexcept;

		Matrix& operator*=(const Matrix& other) noexcept;

		[[nodiscard]] constexpr T* operator[](size_t idx) noexcept { return data[idx]; }
		[[nodiscard]] constexpr const T* operator[](size_t idx) const noexcept { return data[idx]; }

	public:
		T data[L][L];
	};

	namespace Detail
	{
		template <class T, size_t L>
		static Matrix<T, L> GetIdentity() noexcept
		{
			Matrix<T, L> ret;
			for (size_t i = 0; i < L; ++i)
				ret.data[i][i] = static_cast<T>(1);
			return ret;
		}
	}

	template <class T, size_t L>
	const Matrix<T, L> Matrix<T, L>::Zero;

	template <class T, size_t L>
	const Matrix<T, L> Matrix<T, L>::One(1.0f);

	template <class T, size_t L>
	const Matrix<T, L> Matrix<T, L>::Identity = Detail::GetIdentity<T, L>();

	namespace Detail
	{
		template <class T, size_t Idx>
		[[nodiscard]] NO_ODR decltype(auto) GetExpression
			(SIMD::VectorRegister<T> r2, SIMD::VectorRegister<T> r3) noexcept
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

		template <class T, size_t L>
		decltype(auto) LoadMatrix(const Matrix<T, L>& mat)
		{
			using namespace SIMD;
			std::array<VectorRegister<T>, 4> ret;
			ret[0] = VectorLoad(mat.data[0]);

			if constexpr (L > 1)
				ret[1] = VectorLoad(mat.data[1]);
			else
				ret[1] = SIMD::Zero<T>;

			if constexpr (L > 2)
				ret[2] = VectorLoad(mat.data[2]);
			else
				ret[2] = SIMD::Zero<T>;

			if constexpr (L > 3)
				ret[3] = VectorLoad(mat.data[3]);
			else
				ret[3] = SIMD::Zero<T>;

			return ret;
		}
	}

	template <class T, size_t L>
	float Matrix<T, L>::Determinant() const noexcept
	{
		using namespace SIMD;
		const auto mat = Detail::LoadMatrix(GetTranspose());

		auto e0 = Detail::GetExpression<T, 0>(mat[2], mat[3]);
		auto e1 = Detail::GetExpression<T, 1>(mat[2], mat[3]);
		auto e2 = Detail::GetExpression<T, 2>(mat[2], mat[3]);
		
		e0 = VectorMultiply(e0, VectorSwizzle<Swizzle::Y, Swizzle::X, Swizzle::X, Swizzle::X>(mat[1]));
		e1 = VectorMultiply(e1, VectorSwizzle<Swizzle::Z, Swizzle::Z, Swizzle::Y, Swizzle::Y>(mat[1]));
		e2 = VectorMultiply(e2, VectorSwizzle<Swizzle::W, Swizzle::W, Swizzle::W, Swizzle::Z>(mat[1]));

		const auto cofactor = VectorAdd(VectorSubtract(e0, e1), e2);
		const auto det = VectorMultiply(cofactor, mat[0]);
		
		T ret[4];
		VectorStore(det, ret);
		return ret[0] - ret[1] + ret[2] - ret[3];
	}

	template <class T, size_t L>
	Matrix<T, L> Matrix<T, L>::GetInvert() const noexcept
	{
		if (auto ret = *this; ret.Invert())
			return ret;
		return Identity;
	}
	
	namespace Detail
	{
		template <class T>
		[[nodiscard]] NO_ODR decltype(auto) VECTOR_CALL Mat2Mul(SIMD::VectorRegister<T> lhs, SIMD::VectorRegister<T> rhs) noexcept
		{
			using namespace SIMD;
			const auto swi0 = VectorSwizzle<Swizzle::X, Swizzle::W, Swizzle::X, Swizzle::W>(rhs);
			const auto swi1 = VectorSwizzle<Swizzle::Y, Swizzle::X, Swizzle::W, Swizzle::Z>(lhs);
			const auto swi2 = VectorSwizzle<Swizzle::Z, Swizzle::Y, Swizzle::Z, Swizzle::Y>(rhs);
			const auto vec0 = VectorMultiply(lhs, swi0);
			const auto vec1 = VectorMultiply(swi1, swi2);
			return VectorAdd(vec0, vec1);
		};

		template <class T>
		[[nodiscard]] NO_ODR decltype(auto) VECTOR_CALL Mat2AdjMul(SIMD::VectorRegister<T> lhs, SIMD::VectorRegister<T> rhs) noexcept
		{
			using namespace SIMD;
			const auto swi0 = VectorSwizzle<Swizzle::W, Swizzle::W, Swizzle::X, Swizzle::X>(lhs);
			const auto swi1 = VectorSwizzle<Swizzle::Y, Swizzle::Y, Swizzle::Z, Swizzle::Z>(lhs);
			const auto swi2 = VectorSwizzle<Swizzle::Z, Swizzle::W, Swizzle::X, Swizzle::Y>(rhs);
			const auto vec0 = VectorMultiply(swi0, rhs);
			const auto vec1 = VectorMultiply(swi1, swi2);
			return VectorSubtract(vec0, vec1);
		};

		template <class T>
		[[nodiscard]] NO_ODR decltype(auto) VECTOR_CALL Mat2MulAdj(SIMD::VectorRegister<T> lhs, SIMD::VectorRegister<T> rhs) noexcept
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

	template <class T, size_t L>
	bool Matrix<T, L>::Invert() noexcept
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

		const auto detA = VectorReplicate<Swizzle::X>(detSub);
		const auto detB = VectorReplicate<Swizzle::Y>(detSub);
		const auto detC = VectorReplicate<Swizzle::Z>(detSub);
		const auto detD = VectorReplicate<Swizzle::W>(detSub);

		const auto ab = Mat2AdjMul<T>(a, b);
		const auto dc = Mat2AdjMul<T>(d, c);
		
		auto x = VectorSubtract(VectorMultiply(detD, a), Mat2Mul<T>(b, dc));
		auto w = VectorSubtract(VectorMultiply(detA, d), Mat2Mul<T>(c, ab));
		auto y = VectorSubtract(VectorMultiply(detB, c), Mat2MulAdj<T>(d, ab));
		auto z = VectorSubtract(VectorMultiply(detC, b), Mat2MulAdj<T>(a, dc));

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

		Matrix<T, 4> ret;
		VectorStore(VectorShuffle<Swizzle::W, Swizzle::Y, Swizzle::W, Swizzle::Y>(x, y), ret.data[0]);
		VectorStore(VectorShuffle<Swizzle::Z, Swizzle::X, Swizzle::Z, Swizzle::X>(x, y), ret.data[1]);
		VectorStore(VectorShuffle<Swizzle::W, Swizzle::Y, Swizzle::W, Swizzle::Y>(z, w), ret.data[2]);
		VectorStore(VectorShuffle<Swizzle::Z, Swizzle::X, Swizzle::Z, Swizzle::X>(z, w), ret.data[3]);
		*this = Matrix<T, L>(ret);
		return true;
	}

	template <class T, size_t L>
	Matrix<T, L> Matrix<T, L>::GetTranspose() const noexcept
	{
		Matrix<T, L> ret;
		for (size_t i = 0; i < L; ++i)
			for (size_t j = 0; j < L; ++j)
				ret[j][i] = data[i][j];
		return ret;
	}
	
	template <class T, size_t L>
	void Matrix<T, L>::Transpose() noexcept
	{
		for (size_t i = 1; i < L; ++i)
			for (size_t j = 0; j < i; ++j)
				std::swap(data[i][j], data[j][i]);
	}

	template <class T, size_t L>
	Matrix<T, L>& Matrix<T, L>::operator*=(const Matrix<T, L>& other) noexcept
	{
		using namespace SIMD;
		const auto operand = other.GetTranspose();
		for (size_t i = 0; i < L; ++i)
		{
			const auto lhs = VectorLoad(data[i]);
			for (size_t j = 0; j < L; ++j)
			{
				const auto rhs = VectorLoad(operand.data[j]);
				auto result = VectorMultiply(lhs, rhs);
				result = VectorHadd(result, result);
				result = VectorHadd(result, result);
				data[i][j] = VectorStore1(result);
			}
		}
		return *this;
	}

	// Global Operator

	template <class T, size_t L>
	[[nodiscard]] bool operator==(const Matrix<T, L>& lhs, const Matrix<T, L>& rhs) noexcept
	{
		using namespace SIMD;
		const auto lhsMat = Detail::LoadMatrix(lhs);
		const auto rhsMat = Detail::LoadMatrix(rhs);

		for (size_t i = 0; i < L; ++i)
			if (VectorMoveMask(VectorEqual(lhsMat[i], rhsMat[i])) != 0xF)
				return false;

		return true;
	}

	template <class T, size_t L>
	[[nodiscard]] NO_ODR bool operator!=(const Matrix<T, L>& lhs, const Matrix<T, L>& rhs) noexcept { return !(lhs == rhs); }

	template <class T, size_t L>
	[[nodiscard]] NO_ODR Matrix<T, L> operator*(const Matrix<T, L>& lhs, const Matrix<T, L>& rhs) noexcept
	{
		return Matrix<T, L>{ lhs } *= rhs;
	}

	// Global Function

	template <class T, size_t L>
	[[nodiscard]] NO_ODR bool IsNearlyEqual(const Matrix<T, L>& lhs,
		const Matrix<T, L>& rhs, float tolerance = Epsilon) noexcept
	{
		using namespace SIMD;
		const auto epsilon = VectorLoad1(tolerance);
		for (size_t i = 0; i < L; ++i)
		{
			const auto vec = VectorSubtract(VectorLoad(lhs.data[i]), VectorLoad(rhs.data[i]));
			if (VectorMoveMask(VectorLessEqual(vec, epsilon)) != 0xF)
				return false;
		}
		return true;
	}

	template <class T, size_t L>
	[[nodiscard]] NO_ODR bool IsNearlyZero(const Matrix<T, L>& mat, float tolerance = Epsilon) noexcept
	{
		using namespace SIMD;
		const auto epsilon = VectorLoad1(tolerance);
		for (size_t i = 0; i < L; ++i)
		{
			const auto vecSimd = VectorLoad(mat.data[0]);
			if (VectorMoveMask(VectorLessEqual(vecSimd, epsilon)) != 0xF)
				return false;
		}
		return true;
	}
}