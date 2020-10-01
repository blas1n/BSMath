#pragma once

#include "Utility.h"
#include "VectorBase.h"

namespace BSMath
{
	struct Vector2 final : public VectorBase<float, 2>
	{
	public:
		using Super = VectorBase<float, 2>;

		inline const static Vector2 Right;
		inline const static Vector2 Left;
		inline const static Vector2 Up;
		inline const static Vector2 Down;

	public:
		using Super::Super;

		constexpr Vector2() noexcept 
			: Super(), x(data[0][0]), y(data[0][1]) {}

	public:
		float& x;
		float& y;
	};

	const Vector2 Vector2::Right{ 1.0f, 0.0f };
	const Vector2 Vector2::Left{ -1.0f, 0.0f };
	const Vector2 Vector2::Up{ 0.0f, 1.0f };
	const Vector2 Vector2::Down{ 0.0f, -1.0f };
}