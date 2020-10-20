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
			float& X() noexcept { return data[0][0]; }
			const float& X() const noexcept { return data[0][0]; }

			float& Y() noexcept { return data[0][1]; }
			const float& Y() const noexcept { return data[0][1]; }

			float& Z() noexcept { return data[0][2]; }
			const float& Z() const noexcept { return data[0][2]; }

			float& W() noexcept { return data[0][3]; }
			const float& W() const noexcept { return data[0][3]; }

			void Set(T inX, T inY, T inZ, T inW) noexcept
			{
				X() = inX; Y() = inY; Z() = inZ; W() = inW;
			}
		};
	}

	using Vector4 = Detail::Vector4Base<float>;
	using IntVector4 = Detail::Vector4Base<int>;

	using UniformIntVector4Random = Detail::UniformMatrixRandom<int, 1, 4>;
	using UniformVector4Random = Detail::UniformMatrixRandom<float, 1, 4>;
	using NormalVector4Random = Detail::NormalMatrixRandom<1, 4>;
}