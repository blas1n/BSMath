#pragma once

#include <algorithm>
#include <type_traits>
#include "Random.h"
#include "SIMD.h"

namespace BSMath::Detail
{
	template <class T, size_t InRow, size_t InColumn>
	struct alignas(16) MatrixBase0
	{
		static_assert(std::is_arithmetic_v<T>, "T must be arithmetic!");
		static_assert(InRow > 0 && InRow <= 4, "InRow must in the range, 1 ~ 4");
		static_assert(InColumn > 0 && InColumn <= 4, "InColumn must in the range, 1 ~ 4");

	public:
		using Type = T;
		constexpr static size_t Row = InRow;
		constexpr static size_t Column = InColumn;

	public:
		MatrixBase0() noexcept : data() {}

		explicit MatrixBase0(T n) noexcept : data()
		{
			std::fill_n(*data, Row * Column, n);
		}

		explicit MatrixBase0(const T* ptr) noexcept : data()
		{
			std::memcpy(*data, ptr, Row * Column);
		}

		explicit MatrixBase0(std::initializer_list<T> list) noexcept : data()
		{
			std::copy_n(list.begin(), Row * Column, *data);
		}

		[[nodiscard]] bool operator==(const MatrixBase0& other) const noexcept;
		[[nodiscard]] inline bool operator!=(const MatrixBase0& other) const noexcept { return !(*this == other); }

		MatrixBase0& operator+=(const MatrixBase0& other) noexcept;
		MatrixBase0& operator-=(const MatrixBase0& other) noexcept;

		MatrixBase0& operator*=(T scaler) noexcept;
		MatrixBase0& operator/=(T divisor) noexcept;

		[[nodiscard]] constexpr T& operator()(size_t rowIdx, size_t columnIdx) noexcept { return data[rowIdx][columnIdx]; }
		[[nodiscard]] constexpr T operator()(size_t rowIdx, size_t columnIdx) const noexcept { return data[rowIdx][columnIdx]; }

	public:
		T data[InRow][InColumn];
	};

	template <class T, size_t InRow, size_t InColumn>
	bool MatrixBase0<T, InRow, InColumn>::operator==(const MatrixBase0<T, InRow, InColumn>& other) const noexcept
	{
		INVOKE_SIMD(T,
			VectorRegister lhs, rhs;
			bool tmp, ret = true;

			for (size_t i = 0; i < Row; ++i)
			{
				lhs = VectorLoad(data[i]);
				rhs = VectorLoad(other.data[i]);
				tmp = VectorMoveMask(VectorEqual(lhs, rhs)) == 0xF;
				ret = ret && tmp;
			}

			return ret;
		);
	}

	template <class T, size_t InRow, size_t InColumn>
	MatrixBase0<T, InRow, InColumn>& MatrixBase0<T, InRow, InColumn>::operator+=(const MatrixBase0<T, InRow, InColumn>& other) noexcept
	{
		INVOKE_SIMD(T,
			VectorRegister lhs, rhs;

			for (size_t i = 0; i < Row; ++i)
			{
				lhs = VectorLoad(data[i]);
				rhs = VectorLoad(other.data[i]);
				VectorStore(VectorAdd(lhs, rhs), data[i]);
			}

			return *this;
		);
	}

	template <class T, size_t InRow, size_t InColumn>
	MatrixBase0<T, InRow, InColumn>& MatrixBase0<T, InRow, InColumn>::operator-=(const MatrixBase0<T, InRow, InColumn>& other) noexcept
	{
		INVOKE_SIMD(T,
			VectorRegister lhs, rhs;
			
			for (size_t i = 0; i < Row; ++i)
			{
				lhs = VectorLoad(data[i]);
				rhs = VectorLoad(other.data[i]);
				VectorStore(VectorSubtract(lhs, rhs), data[i]);
			}

			return *this;
		);
	}

	template <class T, size_t InRow, size_t InColumn>
	MatrixBase0<T, InRow, InColumn>& MatrixBase0<T, InRow, InColumn>::operator*=(T scaler) noexcept
	{
		INVOKE_SIMD(T,
			VectorRegister lhs, rhs;
			
			for (size_t i = 0; i < Row; ++i)
			{
				lhs = VectorLoad(data[i]);
				rhs = VectorLoad1(scaler);
				VectorStore(VectorMultiply(lhs, rhs), data[i]);
			}
			
			return *this;
		);
	}

	template <class T, size_t InRow, size_t InColumn>
	MatrixBase0<T, InRow, InColumn>& MatrixBase0<T, InRow, InColumn>::operator/=(T divisor) noexcept
	{
		if (IsNearlyZero(divisor))
			return *this;

		INVOKE_SIMD(T,
			VectorRegister lhs, rhs;
			
			for (size_t i = 0; i < Row; ++i)
			{
				lhs = VectorLoad(data[i]);
				rhs = VectorLoad1(divisor);
				VectorStore(VectorDivide(lhs, rhs), data[i]);
			}
			
			return *this;
		);
	}

	// Global Operator

	template <class T, size_t InRow, size_t InColumn>
	[[nodiscard]] NO_ODR MatrixBase0<T, InRow, InColumn> operator+(const MatrixBase0<T, InRow, InColumn>& lhs, const MatrixBase0<T, InRow, InColumn>& rhs) noexcept
	{
		return MatrixBase0<T, InRow, InColumn>{ lhs } += rhs;
	}

	template <class T, size_t InRow, size_t InColumn>
	[[nodiscard]] NO_ODR MatrixBase0<T, InRow, InColumn> operator-(const MatrixBase0<T, InRow, InColumn>& lhs, const MatrixBase0<T, InRow, InColumn>& rhs) noexcept
	{
		return MatrixBase0<T, InRow, InColumn>{ lhs } -= rhs;
	}

	template <class T, size_t InRow, size_t InColumn>
	[[nodiscard]] NO_ODR MatrixBase0<T, InRow, InColumn> operator*(const MatrixBase0<T, InRow, InColumn>& mat, T scaler) noexcept
	{
		return MatrixBase0<T, InRow, InColumn>{ mat } *= scaler;
	}

	template <class T, size_t InRow, size_t InColumn>
	[[nodiscard]] NO_ODR MatrixBase0<T, InRow, InColumn> operator*(T scaler, const MatrixBase0<T, InRow, InColumn>& mat) noexcept
	{
		return MatrixBase0<T, InRow, InColumn>{ mat } *= scaler;
	}

	template <class T, size_t InRow, size_t InColumn>
	[[nodiscard]] NO_ODR MatrixBase0<T, InRow, InColumn> operator/(const MatrixBase0<T, InRow, InColumn>& mat, T divisor) noexcept
	{
		return MatrixBase0<T, InRow, InColumn>{ mat } /= divisor;
	}

	// Random

	template <class T, size_t Row, size_t Column>
	class UniformMatrixDistribution;
	
	template <size_t Row, size_t Column>
	class UniformMatrixDistribution<int, Row, Column> final : public std::uniform_int_distribution<int>
	{
		using Super = std::uniform_int_distribution<int>;

	public:
		using result_type = MatrixBase0<int, Row, Column>;

		template <class Engine>
		[[nodiscard]] result_type operator()(Engine& engine)
		{
			int data[Row][Column]{ 0 };

			for (size_t row = 0; row < Row; ++row)
				for (size_t column = 0; column < Column; ++column)
					data[row][column] = Super::operator()(engine);

			return result_type{ data };
		}

		template <class Engine>
		[[nodiscard]] result_type operator()(Engine& engine, const param_type& param)
		{
			T data[Row][Column]{ 0 };

			for (size_t row = 0; row < Row; ++row)
				for (size_t column = 0; column < Column; ++column)
					data[row][column] = Super::operator()(engine, param);

			return result_type{ data };
		}
	};

	template <size_t Row, size_t Column>
	class UniformMatrixDistribution<float, Row, Column> final : public std::uniform_real_distribution<float>
	{
		using Super = std::uniform_real_distribution<float>;

	public:
		using result_type = MatrixBase0<float, Row, Column>;

		template <class Engine>
		[[nodiscard]] result_type operator()(Engine& engine)
		{
			float data[Row][Column]{ 0.0f };
				
			for (size_t row = 0; row < Row; ++row)
				for (size_t column = 0; column < Column; ++column)
					data[row][column] = Super::operator()(engine);
	
			return result_type{ data };
		}
		
		template <class Engine>
		[[nodiscard]] result_type operator()(Engine& engine, const param_type& param)
		{
			float data[Row][Column]{ 0.0f };

			for (size_t row = 0; row < Row; ++row)
				for (size_t column = 0; column < Column; ++column)
					data[row][column] = Super::operator()(engine, param);

			return result_type{ data };
		}
	};

	template <class T, size_t Row, size_t Column>
	class NormalMatrixDistribution final : public std::normal_distribution<T>
	{
		static_assert(std::is_floating_point_v<T>, "T must be float type!");

		using Super = std::uniform_real_distribution<T>;

	public:
		using result_type = MatrixBase0<T, Row, Column>;

		template <class Engine>
		[[nodiscard]] result_type operator()(Engine& engine)
		{
			T data[Row][Column]{ 0 };

			for (size_t row = 0; row < Row; ++row)
				for (size_t column = 0; column < Column; ++column)
					data[row][column] = Super::operator()(engine);

			return result_type{ data };
		}

		template <class Engine>
		[[nodiscard]] result_type operator()(Engine& engine, const param_type& param)
		{
			T data[Row][Column]{ 0 };

			for (size_t row = 0; row < Row; ++row)
				for (size_t column = 0; column < Column; ++column)
					data[row][column] = Super::operator()(engine, param);

			return result_type{ data };
		}
	};
}