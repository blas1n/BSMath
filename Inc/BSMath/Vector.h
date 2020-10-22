#pragma once

#include "Basic.h"
#include "Utility.h"

namespace BSMath
{
	namespace Detail
	{
		template <class T, size_t L>
		struct VectorBase;

		template <class T>
		struct alignas(16) VectorBase<T, 2>
		{
			static const Vector<T, 2> Zero;
			static const Vector<T, 2> One;
			static const Vector<T, 2> Right;
			static const Vector<T, 2> Left;
			static const Vector<T, 2> Up;
			static const Vector<T, 2> Down;

			constexpr VectorBase() noexcept : data() {}

			explicit constexpr VectorBase(T inX, T inY) noexcept
				: x(inX), y(inY) {}

			constexpr void Set(T inX, T inY) noexcept
			{
				x = inX; y = inY;
			}

			union
			{
				T data[2];
				struct
				{
					T x;
					T y;
				};
			};
		};

		template <class T>
		struct alignas(16) VectorBase<T, 3>
		{
			static const Vector<T, 3> Zero;
			static const Vector<T, 3> One;
			static const Vector<T, 3> Right;
			static const Vector<T, 3> Left;
			static const Vector<T, 3> Up;
			static const Vector<T, 3> Down;
			static const Vector<T, 3> Forward;
			static const Vector<T, 3> Backward;

			constexpr VectorBase() noexcept : data() {}

			explicit constexpr VectorBase(T inX, T inY, T inZ) noexcept
				: x(inX), y(inY), z(inZ) {}

			constexpr void Set(T inX, T inY, T inZ) noexcept
			{
				x = inX; y = inY; z = inZ;
			}

			union
			{
				T data[3];
				struct
				{
					T x;
					T y;
					T z;
				};
			};
		};

		template <class T>
		struct alignas(16) VectorBase<T, 4>
		{
			static const Vector<T, 4> Zero;
			static const Vector<T, 4> One;

			constexpr VectorBase() noexcept : data() {}

			explicit constexpr VectorBase(T inX, T inY, T inZ, T inW) noexcept
				: x(inX), y(inY), z(inZ), w(inW) {}

			constexpr void Set(T inX, T inY, T inZ, T inW) noexcept
			{
				x = inX; y = inY; z = inZ; w = inW;
			}

			union
			{
				T data[4];
				struct
				{
					T x;
					T y;
					T z;
					T w;
				};
			};
		};

		template <class T>
		const Vector<T, 2> VectorBase<T, 2>::    Zero{  static_cast<T>(0),  static_cast<T>(0) };

		template <class T>
		const Vector<T, 2> VectorBase<T, 2>::     One{  static_cast<T>(1),  static_cast<T>(1) };

		template <class T>
		const Vector<T, 2> VectorBase<T, 2>::   Right{  static_cast<T>(1),  static_cast<T>(0) };

		template <class T>
		const Vector<T, 2> VectorBase<T, 2>::    Left{ static_cast<T>(-1),  static_cast<T>(0) };

		template <class T>
		const Vector<T, 2> VectorBase<T, 2>::      Up{  static_cast<T>(0),  static_cast<T>(1) };

		template <class T>
		const Vector<T, 2> VectorBase<T, 2>::    Down{  static_cast<T>(0), static_cast<T>(-1) };

		template <class T>
		const Vector<T, 3> VectorBase<T, 3>::    Zero{  static_cast<T>(0),  static_cast<T>(0),  static_cast<T>(0) };

		template <class T>
		const Vector<T, 3> VectorBase<T, 3>::     One{  static_cast<T>(1),  static_cast<T>(1),  static_cast<T>(1) };

		template <class T>
		const Vector<T, 3> VectorBase<T, 3>::   Right{  static_cast<T>(1),  static_cast<T>(0),  static_cast<T>(0) };

		template <class T>
		const Vector<T, 3> VectorBase<T, 3>::    Left{ static_cast<T>(-1),  static_cast<T>(0),  static_cast<T>(0) };

		template <class T>
		const Vector<T, 3> VectorBase<T, 3>::      Up{  static_cast<T>(0),  static_cast<T>(1),  static_cast<T>(0) };

		template <class T>
		const Vector<T, 3> VectorBase<T, 3>::    Down{  static_cast<T>(0), static_cast<T>(-1),  static_cast<T>(0) };

		template <class T>
		const Vector<T, 3> VectorBase<T, 3>:: Forward{  static_cast<T>(0),  static_cast<T>(0),  static_cast<T>(1) };

		template <class T>
		const Vector<T, 3> VectorBase<T, 3>::Backward{  static_cast<T>(0),  static_cast<T>(0), static_cast<T>(-1) };

		template <class T>
		const Vector<T, 4> VectorBase<T, 4>::    Zero{ static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0) };

		template <class T>
		const Vector<T, 4> VectorBase<T, 4>::     One{ static_cast<T>(1), static_cast<T>(1), static_cast<T>(1), static_cast<T>(1) };
	}

	template <class T, size_t L>
	struct Vector final : public Detail::VectorBase<T, L>
	{
	public:
		using Super = Detail::VectorBase<T, L>;
		using Super::Super;
		using Super::data;

	public:
		explicit Vector(T n) noexcept : Vector()
		{
			std::fill_n(data, L, n);
		}

		explicit Vector(const T* ptr) noexcept : Vector()
		{
			std::copy_n(ptr, L, data);
		}

		template <class U, size_t L2>
		explicit Vector(const Vector<U, L2>& other) noexcept : Vector()
		{
#pragma warning(disable:4244)
			std::copy_n(other.data, Min(L, L2), data);
#pragma warning(default:4244)
		}

		[[nodiscard]] constexpr T GetMin() const noexcept { return Min(data, data + L); }
		[[nodiscard]] constexpr T GetMax() const noexcept { return Max(data, data + L); }

		[[nodiscard]] float Length() const noexcept
		{
			return Sqrt(LengthSquared());
		}

		[[nodiscard]] float LengthSquared() const noexcept
		{
			return *this | *this;
		}

		[[nodiscard]] Vector GetNormal() const noexcept
		{
			Vector ret = *this;
			return ret.Normalize() ? ret : this->Zero;
		}

		bool Normalize() noexcept;

		[[nodiscard]] static float Distance(const Vector& lhs, const Vector& rhs)
		{
			return Sqrt(DistanceSquared(lhs, rhs));
		}

		[[nodiscard]] static float DistanceSquared(const Vector& lhs, const Vector& rhs)
		{
			return Vector(lhs - rhs).LengthSquared();
		}

		[[nodiscard]] Vector operator-() const noexcept;

		Vector& operator+=(const Vector& other) noexcept;
		Vector& operator-=(const Vector& other) noexcept;

		Vector& operator*=(const Vector& other) noexcept;
		Vector& operator*=(T scaler) noexcept;

		Vector& operator/=(const Vector& other) noexcept;
		Vector& operator/=(T divisor) noexcept;

		[[nodiscard]] constexpr T& operator[](size_t idx) noexcept { return data[idx]; }
		[[nodiscard]] constexpr T operator[](size_t idx) const noexcept { return data[idx]; }
	};

	// Global Operators

	template <class T, size_t L>
	[[nodiscard]] NO_ODR bool operator==(const Vector<T, L>& lhs, const Vector<T, L>& rhs) noexcept
	{
		using namespace SIMD;
		const auto lhsVec = VectorLoad(lhs.data);
		const auto rhsVec = VectorLoad(rhs.data);
		return VectorMoveMask(VectorEqual(lhsVec, rhsVec)) == 0xF;
	}

	template <class T, size_t L>
	[[nodiscard]] NO_ODR bool operator!=(const Vector<T, L>& lhs, const Vector<T, L>& rhs) noexcept { return !(lhs == rhs); }

	template <class T, size_t L>
	[[nodiscard]] NO_ODR Vector<T, L> operator+(const Vector<T, L>& lhs, const Vector<T, L>& rhs) noexcept
	{
		return Vector<T, L>{ lhs } += rhs;
	}

	template <class T, size_t L>
	[[nodiscard]] NO_ODR Vector<T, L> operator-(const Vector<T, L>& lhs, const Vector<T, L>& rhs) noexcept
	{
		return Vector<T, L>{ lhs } -= rhs;
	}

	template <class T, size_t L>
	[[nodiscard]] NO_ODR Vector<T, L> operator*(const Vector<T, L>& lhs, const Vector<T, L>& rhs) noexcept
	{
		return Vector<T, L>{ lhs } *= rhs;
	}

	template <class T, size_t L>
	[[nodiscard]] NO_ODR Vector<T, L> operator*(const Vector<T, L>& vec, T scaler) noexcept
	{
		return Vector<T, L>{ vec } *= scaler;
	}

	template <class T, size_t L>
	[[nodiscard]] NO_ODR Vector<T, L> operator*(T scaler, const Vector<T, L>& vec) noexcept
	{
		return Vector<T, L>{ vec } *= scaler;
	}
	
	template <class T, size_t L>
	[[nodiscard]] NO_ODR Vector<T, L> operator/(const Vector<T, L>& lhs, const Vector<T, L>& rhs) noexcept
	{
		return Vector<T, L>{ lhs } /= rhs;
	}

	template <class T, size_t L>
	[[nodiscard]] NO_ODR Vector<T, L> operator/(const Vector<T, L>& vec, T divisor) noexcept
	{
		return Vector<T, L>{ vec } /= divisor;
	}

	template <class T>
	Vector<T, 3>& operator^=(Vector<T, 3>& lhs,  const Vector<T, 3>& rhs) noexcept
	{
		const auto ret1 = Vector<T, 3>{ lhs.y, lhs.z, lhs.x } *Vector<T, 3>{ rhs.z, rhs.x, rhs.y };
		const auto ret2 = Vector<T, 3>{ lhs.z, lhs.x, lhs.y } *Vector<T, 3>{ rhs.y, rhs.z, rhs.x };
		return lhs = ret1 - ret2;
	}

	template <class T, size_t L>
	[[nodiscard]] NO_ODR T operator|(const Vector<T, L>& lhs, const Vector<T, L>& rhs) noexcept
	{
		using namespace SIMD;
		const auto size = VectorMultiply(VectorLoad(lhs.data), VectorLoad(rhs.data));
		return VectorStore1(VectorHadd(VectorHadd(size, size), size));
	}

	template <class T>
	[[nodiscard]] NO_ODR Vector<T, 3> operator^(const Vector<T, 3>& lhs, const Vector<T, 3>& rhs) noexcept
	{
		Vector<T, 3> ret{ lhs };
		return ret ^= rhs;
	}

	template <class T, size_t L>
	bool Vector<T, L>::Normalize() noexcept
	{
		using namespace SIMD;
		auto vec = VectorLoad(data);
		auto size = VectorMultiply(vec, vec);
		size = VectorHadd(VectorHadd(size, size), size);
		vec = VectorMultiply(vec, VectorInvSqrt(size));
		VectorStore(vec, data);
		return true;
	}

	template <class T, size_t L>
	Vector<T, L> Vector<T, L>::operator-() const noexcept
	{
		return Vector<T, L>::Zero - *this;
	}

	template <class T, size_t L>
	Vector<T, L>& Vector<T, L>::operator+=(const Vector<T, L>& other) noexcept
	{
		using namespace SIMD;
		const auto lhs = VectorLoad(data);
		const auto rhs = VectorLoad(other.data);
		VectorStore(VectorAdd(lhs, rhs), data);
		return *this;
	}

	template <class T, size_t L>
	Vector<T, L>& Vector<T, L>::operator-=(const Vector<T, L>& other) noexcept
	{
		using namespace SIMD;
		const auto lhs = VectorLoad(data);
		const auto rhs = VectorLoad(other.data);
		VectorStore(VectorSubtract(lhs, rhs), data);
		return *this;
	}

	template <class T, size_t L>
	Vector<T, L>& Vector<T, L>:: operator*=(const Vector<T, L>& other) noexcept
	{
		using namespace SIMD;
		const auto lhs = VectorLoad(data);
		const auto rhs = VectorLoad(other.data);
		VectorStore(VectorMultiply(lhs, rhs), data);
		return *this;
	}

	template <class T, size_t L>
	Vector<T, L>& Vector<T, L>::operator*=(T scaler) noexcept
	{
		using namespace SIMD;
		const auto lhs = VectorLoad(data);
		const auto rhs = VectorLoad1(scaler);
		VectorStore(VectorMultiply(lhs, rhs), data);
		return *this;
	}

	template <class T, size_t L>
	Vector<T, L>& Vector<T, L>:: operator/=(const Vector<T, L>& other) noexcept
	{
		if (other == this->Zero) return *this;

		using namespace SIMD;
		const auto lhs = VectorLoad(data);
		const auto rhs = VectorLoad(other.data);
		VectorStore(VectorDivide(lhs, rhs), data);
		return *this;
	}

	template <class T, size_t L>
	Vector<T, L>& Vector<T, L>::operator/=(T divisor) noexcept
	{
		if (divisor == 0.0f) return *this;

		using namespace SIMD;
		const auto lhs = VectorLoad(data);
		const auto rhs = VectorLoad1(divisor);
		VectorStore(VectorDivide(lhs, rhs), data);
		return *this;
	}

	// Global Functions

	template <class T, size_t L>
	[[nodiscard]] NO_ODR Vector<T, L> Min(const Vector<T, L>& lhs, const Vector<T, L>& rhs) noexcept
	{
		using namespace SIMD;

		Vector<T, L> ret;
		VectorStore(VectorMin(VectorLoad(lhs.data), VectorLoad(rhs.data)), ret.data);
		return ret;
	}

	template <class T, size_t L>
	[[nodiscard]] NO_ODR Vector<T, L> Max(const Vector<T, L>& lhs, const Vector<T, L>& rhs) noexcept
	{
		using namespace SIMD;

		Vector<T, L> ret;
		VectorStore(VectorMax(VectorLoad(lhs.data), VectorLoad(rhs.data)), ret.data);
		return ret;
	}

	template <class T, size_t L>
	[[nodiscard]] NO_ODR Vector<T, L> Clamp(const Vector<T, L>& n, const Vector<T, L>& min, const Vector<T, L>& max) noexcept
	{
		auto realMin = Min(min, max);
		auto realMax = Max(min, max);
		return Max(realMin, Min(realMax, n));
	}

	template <size_t L>
	[[nodiscard]] NO_ODR Vector<int, L> Abs(const Vector<int, L>& n) noexcept
	{
		using namespace SIMD;
		auto point = VectorLoad(n.data);
		auto mask = VectorLessThan(point, Zero<int>);
		point = VectorXor(point, mask);
		mask = VectorAnd(mask, One<int>);

		Vector<float, L> ret;
		VectorStore(VectorAdd(point, mask), ret.data);
		return ret;
	}

	template <size_t L>
	[[nodiscard]] NO_ODR Vector<float, L> Abs(const Vector<float, L>& n) noexcept
	{
		using namespace SIMD;
		const auto vec = VectorLoad(n.data);
		const auto mask = VectorLoad1(-0.0f);

		Vector<float, L> ret;
		VectorStore(VectorAndNot(mask, vec), ret.data);
		return ret;
	}

	template <class T, size_t L>
	[[nodiscard]] NO_ODR Vector<T, L> Sign(const Vector<T, L>& n) noexcept
	{
		using namespace SIMD;
		const auto vec = VectorLoad(n.data);
		const auto positive = VectorAnd(VectorGreaterThan(vec, Zero<T>), One<T>);
		const auto negative = VectorAnd(VectorLessThan(vec, Zero<T>), VectorLoad1(static_cast<T>(-1)));

		Vector<float, L> ret;
		VectorStore(VectorOr(positive, negative), ret.data);
		return ret;
	}

	template <size_t L>
	[[nodiscard]] NO_ODR bool IsNearlyEqual(const Vector<float, L>& lhs,
		const Vector<float, L>& rhs, float tolerance = Epsilon) noexcept
	{
		using namespace SIMD;
		const auto epsilon = VectorLoad1(tolerance);
		const auto vec = VectorSubtract(VectorLoad(lhs.data), VectorLoad(rhs.data));
		return VectorMoveMask(VectorLessEqual(vec, epsilon)) == 0xF;
	}

	template <size_t L>
	[[nodiscard]] NO_ODR bool IsNearlyZero(const Vector<float, L>& vec, float tolerance = Epsilon) noexcept
	{
		using namespace SIMD;
		const auto epsilon = VectorLoad1(tolerance);
		const auto vecSimd = VectorLoad(vec.data);
		return VectorMoveMask(VectorLessEqual(vecSimd, epsilon)) == 0xF;
	}

	template <size_t L>
	[[nodiscard]] NO_ODR Vector<float, L> GetRangePct(const Vector<float, L>& vec,
		const Vector<float, L>& min, const Vector<float, L>& max) noexcept
	{
		using namespace SIMD;
		const auto vecSimd = VectorLoad(vec.data);
		const auto minSimd = VectorLoad(min.data);
		const auto maxSimd = VectorLoad(max.data);
		const auto numerator = VectorSubtract(vecSimd, minSimd);
		const auto denomirator = VectorSubtract(maxSimd, minSimd);

		Vector<float, L> ret;
		VectorStore(VectorDivide(numerator, denomirator), ret.data);
		return ret;
	}
}