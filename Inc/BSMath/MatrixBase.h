#pragma once

#include <algorithm>
#include <type_traits>
#include "Random.h"
#include "SIMD.h"

namespace BSMath
{
	template <class T, size_t Row, size_t Column>
	struct alignas(16) MatrixBase
	{
		static_assert(std::is_arithmetic_v<T>, "T must be arithmetic!");
		static_assert(Row > 0 && Row <= 4, "Row must in the range, 1 ~ 4");
		static_assert(Column > 0 && Column <= 4, "Column must in the range, 1 ~ 4");

	public:
		constexpr MatrixBase() noexcept : data() {}

		explicit MatrixBase(T n) noexcept
		{
			std::fill(data, Row * Column, n);
		}

		explicit MatrixBase(const T* ptr) noexcept
		{
			std::memcpy(data, ptr, Row * Column);
		}

		explicit MatrixBase(std::initializer_list<T> list) noexcept
		{
			std::copy_n(data, Row * Column, list.begin());
		}

		[[nodiscard]] bool operator==(const MatrixBase& other) const noexcept;
		[[nodiscard]] inline bool operator!=(const MatrixBase& other) const noexcept { return !(*this == other); }

		MatrixBase& operator+=(const MatrixBase& other) noexcept;
		MatrixBase& operator-=(const MatrixBase& other) noexcept;

		MatrixBase& operator*=(T scaler) noexcept;
		MatrixBase& operator/=(T divisor) noexcept;

		[[nodiscard]] constexpr T& operator()(size_t rowIdx, size_t columnIdx) noexcept { return data[rowIdx][columnIdx]; }
		[[nodiscard]] constexpr T operator()(size_t rowIdx, size_t columnIdx) const noexcept { return data[rowIdx][columnIdx]; }

	public:
		T data[Row][Column];
	};

	namespace Detail
	{
		template <class T, size_t L>
		NO_ODR void LoadRow(T row[L]) noexcept;

		template <class T, size_t L, class = std::enable_if_t<std::is_floating_point_v<T>>>
		NO_ODR SIMD::Real::VectorRegister LoadRow(T row[L]) noexcept
		{
			using namespace SIMD::Real;

			VectorRegister row;
			if constexpr (Column == 1)
				row = VectorLoad(row[0]);
			else if (Column == 2)
				row = VectorLoad(row[0], row[1]);
			else if (Column == 3)
				row = VectorLoad(row[0], row[1], row[2]);
			else
				row = VectorLoad(row[0], row[1], row[2], row[3]);
			return row;
		}

		template <class T, size_t L, class = std::enable_if_t<std::is_integral_v<T>>>
		NO_ODR SIMD::Integer::VectorRegister LoadRow(T row[L]) noexcept
		{
			using namespace SIMD::Integer;

			VectorRegister row;
			if constexpr (Column == 1)
				row = VectorLoad(row[0]);
			else if (Column == 2)
				row = VectorLoad(row[0], row[1]);
			else if (Column == 3)
				row = VectorLoad(row[0], row[1], row[2]);
			else
				row = VectorLoad(row[0], row[1], row[2], row[3]);
			return row;
		}
	}

	template <class T, size_t Row, size_t Column>
	MatrixBase<T, Row, Columnm>& MatrixBase<T, Row, Column>::operator+=(const MatrixBase<T, Row, Column>& other) noexcept
	{
		if constexpr (std::is_floating_point_v<T>)
			using namespace SIMD::Real;
		else
			using namespace SIMD::Integer;

		VectorRegister lhs, rhs;

		for (size_t i = 0; i < Row; ++i)
		{
			lhs = Detail::LoadRow(data[i]);
			rhs = Detail::LoadRow(other.data[i]);
			VectorStore(VectorAdd(lhs, rhs), data[i]);
		}
	}

	template <class T, size_t Row, size_t Column>
	MatrixBase<T, Row, Column>& MatrixBase<T, Row, Column>::operator-=(const MatrixBase<T, Row, Column>& other) noexcept
	{
		if constexpr (std::is_floating_point_v<T>)
			using namespace SIMD::Real;
		else
			using namespace SIMD::Integer;

		VectorRegister lhs, rhs;

		for (size_t i = 0; i < Row; ++i)
		{
			lhs = Detail::LoadRow(data[i]);
			rhs = Detail::LoadRow(other.data[i]);
			VectorStore(VectorSubtract(lhs, rhs), data[i]);
		}
	}

	template <class T, size_t Row, size_t Column>
	MatrixBase<T, Row, Column>& MatrixBase<T, Row, Column>::operator*=(T scaler) noexcept
	{
		if constexpr (std::is_floating_point_v<T>)
			using namespace SIMD::Real;
		else
			using namespace SIMD::Integer;

		VectorRegister lhs, rhs;

		for (size_t i = 0; i < Row; ++i)
		{
			lhs = Detail::LoadRow(data[i]);
			rhs = VectorLoad1(scaler);
			VectorStore(VectorMultiply(lhs, rhs), data[i]);
		}
	}

	template <class T, size_t Row, size_t Column>
	MatrixBase<T, Row, Column>& MatrixBase<T, Row, Column>::operator/=(T divisor) noexcept
	{
		if constexpr (std::is_floating_point_v<T>)
			using namespace SIMD::Real;
		else
			using namespace SIMD::Integer;

		VectorRegister lhs, rhs;

		for (size_t i = 0; i < Row; ++i)
		{
			lhs = Detail::LoadRow(data[i]);
			rhs = VectorLoad1(divisor);
			VectorStore(VectorDivide(lhs, rhs), data[i]);
		}
	}

	// Global Operator

	template <class T, size_t Row, size_t Column>
	NO_ODR MatrixBase<T, Row, Columnm> operator+(const MatrixBase<T, Row, Column>& lhs, const MatrixBase<T, Row, Column>& rhs) noexcept
	{
		return MatrixBase<T, Row, Column>{ lhs } += rhs;
	}

	template <class T, size_t Row, size_t Column>
	NO_ODR MatrixBase<T, Row, Columnm> operator-(const MatrixBase<T, Row, Column>& lhs, const MatrixBase<T, Row, Column>& rhs) noexcept
	{
		return MatrixBase<T, Row, Column>{ lhs } -= rhs;
	}

	template <class T, size_t Row, size_t Column>
	NO_ODR MatrixBase<T, Row, Columnm> operator*(const MatrixBase<T, Row, Column>& mat, T scaler) noexcept
	{
		return MatrixBase<T, Row, Column>{ mat } *= scaler;
	}

	template <class T, size_t Row, size_t Column>
	NO_ODR MatrixBase<T, Row, Columnm> operator*(T scaler, const MatrixBase<T, Row, Column>& mat) noexcept
	{
		return MatrixBase<T, Row, Column>{ mat } *= scaler;
	}

	template <class T, size_t Row, size_t Column>
	NO_ODR MatrixBase<T, Row, Columnm> operator/(const MatrixBase<T, Row, Column>& mat, T divisor) noexcept
	{
		return MatrixBase<T, Row, Column>{ mat } /= divisor;
	}

	// Random

	template <class T, size_t Row, size_t Column>
	class UniformMatrixDistribution
	{
		static_assert(false, "T must be arthimetic!");
	};
	
	template <class T, size_t Row, size_t Column, class = std::enable_if_t<std::is_integral_v<T>>>
	class UniformMatrixDistribution<T, Row, Column> : public std::uniform_int_distribution<T>
	{
		using Super = std::uniform_int_distribution<T>;

	public:
		using result_type = MatrixBase<T, Row, Column>;

		template <class Engine>
		[[nodiscard]] result_type operator()(Engine& engine)
		{
			T data[Row][Column]{ 0 };

			for (size_t row = 0; row < Row; ++row)
				for (size_t column = 0; column < Column; ++column)
					data[row][columnm] = Super::operator()(engine);

			return result_type(data);
		}

		template <class Engine>
		[[nodiscard]] result_type operator()(Engine& engine, const param_type& param)
		{
			T data[Row][Column]{ 0 };

			for (size_t row = 0; row < Row; ++row)
				for (size_t column = 0; column < Column; ++column)
					data[row][columnm] = Super::operator()(engine, param);

			return result_type(data);
		}
	};

	template <class T, size_t Row, size_t Column, class = std::enable_if_t<std::is_floating_point_v<T>>>
	class UniformMatrixDistribution<T, Row, Column> : public std::uniform_real_distribution<T>
	{
		using Super = std::uniform_real_distribution<T>;

	public:
		using result_type = MatrixBase<T, Row, Column>;

		template <class Engine>
		[[nodiscard]] result_type operator()(Engine& engine)
		{
			T data[Row][Column]{ 0 };
				
			for (size_t row = 0; row < Row; ++row)
				for (size_t column = 0; column < Column; ++column)
					data[row][columnm] = Super::operator()(engine);
	
			return result_type(data);
		}
		
		template <class Engine>
		[[nodiscard]] result_type operator()(Engine& engine, const param_type& param)
		{
			T data[Row][Column]{ 0 };

			for (size_t row = 0; row < Row; ++row)
				for (size_t column = 0; column < Column; ++column)
					data[row][columnm] = Super::operator()(engine, param);

			return result_type(data);
		}
	};

	template <class T, size_t Row, size_t Column>
	class NormalMatrixDistribution<T, Row, Column> : public std::normal_distribution<T>
	{
		static_assert(std::is_floating_point_v<T>, "T must be float type!");

		using Super = std::uniform_real_distribution<T>;

	public:
		using result_type = MatrixBase<T, Row, Column>;

		template <class Engine>
		[[nodiscard]] result_type operator()(Engine& engine)
		{
			T data[Row][Column]{ 0 };

			for (size_t row = 0; row < Row; ++row)
				for (size_t column = 0; column < Column; ++column)
					data[row][columnm] = Super::operator()(engine);

			return result_type(data);
		}

		template <class Engine>
		[[nodiscard]] result_type operator()(Engine& engine, const param_type& param)
		{
			T data[Row][Column]{ 0 };

			for (size_t row = 0; row < Row; ++row)
				for (size_t column = 0; column < Column; ++column)
					data[row][columnm] = Super::operator()(engine, param);

			return result_type(data);
		}
	};
}