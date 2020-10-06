#pragma once

#include "Utility.h"
#include "VectorBase.h"

namespace BSMath
{
	namespace Detail
	{
		template <class T>
		struct Vector3Base final : public VectorBase<T, 3>
		{
		public:
			using Super = Detail::VectorBase<T, 3>;
			using Super::Super;
			using Super::data;

			const static Vector3Base Right;
			const static Vector3Base Left;
			const static Vector3Base Up;
			const static Vector3Base Down;
			const static Vector3Base Forward;
			const static Vector3Base Backward;

		public:
			void Set(T inX, T inY, T inZ) noexcept
			{
				x = inX; y = inY; z = inZ;
			}

			Vector3Base& operator^=(const Vector3Base& other) noexcept;

			[[nodiscard]] static Vector3Base Cross(const Vector3Base& lhs, const Vector3Base& rhs) noexcept;
		};

		template <class T>
		const Vector3Base<T> Vector3Base<T>::Right{ static_cast<T>(1), static_cast<T>(0), static_cast<T>(0) };

		template <class T>
		const Vector3Base<T> Vector3Base<T>::Left{ static_cast<T>(-1), static_cast<T>(0), static_cast<T>(0) };

		template <class T>
		const Vector3Base<T> Vector3Base<T>::Up{ static_cast<T>(0), static_cast<T>(1), static_cast<T>(0) };

		template <class T>
		const Vector3Base<T> Vector3Base <T>::Down{ static_cast<T>(0), static_cast<T>(-1), static_cast<T>(0) };

		template <class T>
		const Vector3Base<T> Vector3Base<T>::Forward{ static_cast<T>(0), static_cast<T>(0), static_cast<T>(1) };

		template <class T>
		const Vector3Base<T> Vector3Base<T>::Backward{ static_cast<T>(0), static_cast<T>(0), static_cast<T>(-1) };

		template <class T>
		Vector3Base<T>& Vector3Base<T>::operator^=(const Vector3Base<T>& other) noexcept
		{
			const auto ret1 = Vector3Base<T>{ y, z, x } * Vector3Base<T>{ other.z, other.x, other.y };
			const auto ret2 = Vector3Base<T>{ z, x, y } * Vector3Base<T>{ other.y, other.z, other.x };
			return *this = ret1 - ret2;
		}

		template <class T>
		Vector3Base<T> Vector3Base<T>::Cross(const Vector3Base<T>& lhs, const Vector3Base<T>& rhs) noexcept
		{
			return Vector3Base<T>{ lhs } ^= rhs;
		}

		template <class T>
		[[nodiscard]] NO_ODR Vector3Base<T> operator^(const Vector3Base<T>& lhs, const Vector3Base<T>& rhs) noexcept
		{
			return Vector3Base<T>::Cross(lhs, rhs);
		}
	}

	using Vector3 = Detail::Vector3Base<float>;
	using IntVector3 = Detail::Vector3Base<int>;
}