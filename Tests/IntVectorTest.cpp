#include <gtest/gtest.h>
#include "BSMath/IntVector.h"

namespace BSMath
{
	constexpr bool IsEqual(const IntVector& lhs, const IntVector& rhs) noexcept
	{
		return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
	}

	TEST(IntVectorTest, Construct)
	{
		constexpr static const int Zeros[3]{ 0, 0, 0 };

		constexpr IntVector vec0{};
		constexpr IntVector vec1{ 0 };
		constexpr IntVector vec2{ 0, 0, 0 };
		constexpr IntVector vec3{ Zeros };
		constexpr IntVector vec4{ IntVector::Zero() };

		bool result = IsEqual(vec0, vec1) && IsEqual(vec0, vec2)
			&& IsEqual(vec0, vec3) && IsEqual(vec0, vec4);

		EXPECT_TRUE(result);
	}

	TEST(IntVectorTest, Operator)
	{
		IntVector vec{ 1, 0, 0 };
		IntVector target{ -1, 0, 0 };
		EXPECT_EQ(-vec, target);

		vec += IntVector{ 9, 5, 3 };
		target.Set(10, 5, 3);
		EXPECT_EQ(vec, target);

		vec -= IntVector{ 5, 3, 1 };
		target.Set(5, 2, 2);
		EXPECT_EQ(vec, target);

		vec *= IntVector{ 2, 3, 4 };
		target.Set(10, 6, 8);
		EXPECT_EQ(vec, target);

		vec *= 2;
		target.Set(20, 12, 16);
		EXPECT_EQ(vec, target);

		vec /= 2;
		target.Set(10, 6, 8);
		EXPECT_EQ(vec, target);

		vec /= IntVector{ 2, 3, 4 };
		target.Set(5, 2, 2);
		EXPECT_EQ(vec, target);

		target.Set(2, 2, 3);
		EXPECT_EQ(vec | target, 20);

		vec ^= IntVector{ 1, 2, 3 };
		target.Set(2, -13, 8);
		EXPECT_EQ(vec, target);
	}

	TEST(IntVectorTest, Global)
	{
		IntVector lhs{ 5, 2, 5 };
		IntVector rhs{ 2, 5, 2 };

		IntVector result = Min(lhs, rhs);
		IntVector target{ 2 };
		EXPECT_EQ(result, target);

		result = Max(lhs, rhs);
		target.Set(5, 5, 5);
		EXPECT_EQ(result, target);

		result.Set(1, 6, 1);
		result = Clamp(result, lhs, rhs);
		target.Set(2, 5, 2);
		EXPECT_EQ(result, target);

		result = Abs(IntVector{ -10, 40, -15 });
		target.Set(10, 40, 15);
		EXPECT_EQ(result, target);

		result = Sign(IntVector{ -10, 40, -15 });
		target.Set(-1, 1, -1);
		EXPECT_EQ(result, target);
	}
}