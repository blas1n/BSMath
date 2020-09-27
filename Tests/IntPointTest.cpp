#include <gtest/gtest.h>
#include "BSMath/IntPoint.h"

namespace BSMath
{
	constexpr bool IsEqual(const IntPoint& lhs, const IntPoint& rhs) noexcept
	{
		return lhs.x == rhs.x && lhs.y == rhs.y;
	}

	TEST(IntPointTest, Construct)
	{
		constexpr static const int Zeros[2]{ 0, 0 };

		constexpr IntPoint vec0{};
		constexpr IntPoint vec1{ 0 };
		constexpr IntPoint vec2{ 0, 0 };
		constexpr IntPoint vec3{ Zeros };
		constexpr IntPoint vec4{ IntPoint::Zero() };

		const bool result = IsEqual(vec0, vec1) && IsEqual(vec0, vec2)
			&& IsEqual(vec0, vec3) && IsEqual(vec0, vec4);

		EXPECT_TRUE(result);
	}

	TEST(IntPointTest, Operator)
	{
		IntPoint vec{ 1, 0 };
		IntPoint target{ -1, 0 };
		EXPECT_EQ(-vec, target);

		vec += IntPoint{ 9, 5 };
		target.Set(10, 5);
		EXPECT_EQ(vec, target);

		vec -= IntPoint{ 5, 3 };
		target.Set(5, 2);
		EXPECT_EQ(vec, target);

		vec *= IntPoint{ 2, 3 };
		target.Set(10, 6);
		EXPECT_EQ(vec, target);

		vec *= 2;
		target.Set(20, 12);
		EXPECT_EQ(vec, target);

		vec /= 2;
		target.Set(10, 6);
		EXPECT_EQ(vec, target);

		vec /= IntPoint{ 2, 3 };
		target.Set(5, 2);
		EXPECT_EQ(vec, target);

		target = IntPoint::One() * 2;
		EXPECT_EQ(vec | target, 14);
	}

	TEST(IntPointTest, Global)
	{
		IntPoint lhs{ 5, 2 };
		IntPoint rhs{ 2, 5 };

		IntPoint result = Min(lhs, rhs);
		IntPoint target{ 2 };
		EXPECT_EQ(result, target);

		result = Max(lhs, rhs);
		target = IntPoint::One() * 5;
		EXPECT_EQ(result, target);

		result.Set(1, 6);
		result = Clamp(result, lhs, rhs);
		target.Set(2, 5);
		EXPECT_EQ(result, target);

		result = Abs(IntPoint{ -10, 40 });
		target.Set(10, 40);
		EXPECT_EQ(result, target);

		result = Sign(IntPoint{ -10, 40 });
		target.Set(-1, 1);
		EXPECT_EQ(result, target);
	}
}