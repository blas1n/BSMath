#pragma once

#include "Utility.h"
#include "VectorBase.h"

namespace BSMath
{
	namespace Detail
	{
		template <class T>
		struct Vector4Base final : public VectorBase<T, 4>
		{
		public:
			using Super = Detail::VectorBase<T, 4>;
			using Super::Super;
			using Super::data;

		public:
			void Set(T inX, T inY, T inZ, T inW) noexcept
			{
				x = inX; y = inY; z = inZ; w = inW;
			}
		};
	}

	using Vector4 = Detail::Vector4Base<float>;
	using IntVector4 = Detail::Vector4Base<int>;
}