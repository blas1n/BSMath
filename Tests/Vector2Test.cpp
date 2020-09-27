#include <gtest/gtest.h>
#include "BSMath/Vector2.h"
#include "BSMath/Utility.h"

namespace BSMath
{
	constexpr bool IsEqual(const Vector2& lhs, const Vector2& rhs) noexcept
	{
		return IsNearlyEqual(lhs.x, rhs.x) && IsNearlyEqual(lhs.y, rhs.y);
	}

	TEST(Vector2Test, Construct)
	{
		constexpr static const float Zeros[2]{ 0.0f, 0.0f };
		
		constexpr Vector2 vec0{};
		constexpr Vector2 vec1{ 0.0f };
		constexpr Vector2 vec2{ 0.0f, 0.0f };
		constexpr Vector2 vec3{ Zeros };
		constexpr Vector2 vec4{ Vector2::Zero() };
	
		bool result = IsEqual(vec0, vec1) && IsEqual(vec0, vec2)
			&& IsEqual(vec0, vec3) && IsEqual(vec0, vec4);

		EXPECT_TRUE(result);
	}

	TEST(Vector2Test, Size)
	{
		Vector2 vec{ 1.0f, 2.0f };
		EXPECT_TRUE(IsNearlyEqual(vec.LengthSquared(), 5.0f));

		vec.Set(3.0f, 3.0f);
		EXPECT_TRUE(vec.Normalize());

		Vector2 target{ 1.0f, 1.0f };
		EXPECT_TRUE(target.Normalize());
		EXPECT_TRUE(IsEqual(vec, target));

		EXPECT_TRUE(IsNearlyZero(Vector2::Distance(vec, target)));
	}

	TEST(Vector2Test, Compare)
	{
		for (float i = -5.0f; i <= 5.1f; ++i)
		{
			for (float j = -5.0f; j <= 5.1f; ++j)
			{
				Vector2 lhs{ i, j };
				Vector2 rhs{ i, j };
				EXPECT_EQ(lhs, rhs);
			}
		}
	}

	TEST(Vector2Test, Operator)
	{
		Vector2 vec{ 1.0f, 0.0f };
		Vector2 target{ -1.0f, 0.0f };
		EXPECT_EQ(-vec, target);

		vec += Vector2{ 9.0f, 5.0f };
		target.Set(10.0f, 5.0f);
		EXPECT_EQ(vec, target);

		vec -= Vector2{ 5.0f, 3.0f };
		target.Set(5.0f, 2.0f);
		EXPECT_EQ(vec, target);

		vec *= Vector2{ 2.0f, 3.0f };
		target.Set(10.0f, 6.0f);
		EXPECT_EQ(vec, target);

		vec *= 2.0f;
		target.Set(20.0f, 12.0f);
		EXPECT_EQ(vec, target);

		vec /= 2.0f;
		target.Set(10.0f, 6.0f);
		EXPECT_EQ(vec, target);

		vec /= Vector2{ 2.0f, 3.0f };
		target.Set(5.0f, 2.0f);
		EXPECT_EQ(vec, target);

		target.Set(2.0f, 2.0f);
		EXPECT_EQ(vec | target, 14.0f);
	}

	TEST(Vector2Test, Accessor)
	{
		constexpr Vector2 vec{ 1.0f, 2.0f };
		EXPECT_EQ(vec[0], 1.0f);
		EXPECT_EQ(vec[1], 2.0f);
	}

	TEST(Vector2Test, Global)
	{
		Vector2 lhs{ 5.0f, 2.0f };
		Vector2 rhs{ 2.0f, 5.0f };

		Vector2 result = Min(lhs, rhs);
		Vector2 target{ 2.0f };
		EXPECT_EQ(result, target);

		result = Max(lhs, rhs);
		target.Set(5.0f, 5.0f);
		EXPECT_EQ(result, target);

		result.Set(1.0f, 6.0f);
		result = Clamp(result, lhs, rhs);
		target.Set(2.0f, 5.0f);
		EXPECT_EQ(result, target);

		result = Abs(Vector2{ -10.0f, 40.0f });
		target.Set(10.0f, 40.0f);
		EXPECT_EQ(result, target);

		result = Sign(Vector2{ -10.0f, 40.0f });
		target.Set(-1.0f, 1.0f);
		EXPECT_EQ(result, target);

		result.Set(0.0f, 0.0f);
		EXPECT_TRUE(IsNearlyEqual(result, Vector2::Zero()));
		EXPECT_TRUE(IsNearlyZero(result));

		result = Vector2::One() * 5.0f;
		lhs = Vector2::Zero();
		rhs = Vector2::One() * 10.0f;
		result = GetRangePct(result, lhs, rhs);
		EXPECT_TRUE(IsNearlyEqual(result, Vector2::One() * 0.5f));

		result = Lerp(lhs, rhs, 0.5f);
		EXPECT_TRUE(IsNearlyEqual(result, Vector2::One() * 5.0f));
	}
}