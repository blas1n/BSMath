#pragma once

#include "Utility.h"
#include "VectorBase.h"

namespace BSMath
{
	namespace Detail
	{
		template <class T>
		struct Vector2Base final : public VectorBase<T, 2>
		{
		public:
			using Super = Detail::VectorBase<T, 2>;
			using Super::Super;
			using Super::data;

			const static Vector2Base Right;
			const static Vector2Base Left;
			const static Vector2Base Up;
			const static Vector2Base Down;

		public:
			void Set(T inX, T inY) noexcept
			{
				x = inX; y = inY;
			}
		};

		template <class T>
		const Vector2Base<T> Vector2Base<T>::Right{ static_cast<T>(1), static_cast<T>(0) };

		template <class T>
		const Vector2Base<T> Vector2Base<T>::Left{ static_cast<T>(-1), static_cast<T>(0) };

		template <class T>
		const Vector2Base<T> Vector2Base<T>::Up{ static_cast<T>(0), static_cast<T>(1) };

		template <class T>
		const Vector2Base<T> Vector2Base <T>::Down{ static_cast<T>(0), static_cast<T>(-1) };
	}

	using Vector2 = Detail::Vector2Base<float>;
	using IntVector2 = Detail::Vector2Base<int>;
}