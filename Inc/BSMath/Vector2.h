#pragma once

#include "Utility.h"
#include "VectorBase.h"

namespace BSMath
{
	struct Vector2 final : public Detail::VectorBase<float, 2>
	{
	public:
		using Super = Detail::VectorBase<float, 2>;

		const static Vector2 Right;
		const static Vector2 Left;
		const static Vector2 Up;
		const static Vector2 Down;

	public:
		using Super::Super;

		void Set(float inX, float inY) noexcept
		{
			x = inX; y = inY;
		}
	};

	const Vector2 Vector2::Right{ 1.0f, 0.0f };
	const Vector2 Vector2::Left{ -1.0f, 0.0f };
	const Vector2 Vector2::Up{ 0.0f, 1.0f };
	const Vector2 Vector2::Down{ 0.0f, -1.0f };
}