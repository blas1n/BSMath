#include <gtest/gtest.h>
#include "BSMath/Rotator.h"

namespace BSMath
{
	TEST(RotatorTest, Constructor)
	{
		const static float Zeros[3]{ 0.0f, 0.0f, 0.0f };

		Rotator quat0;
		Rotator quat1{ 0.0f };
		Rotator quat2{ 0.0f, 0.0f, 0.0f };
		Rotator quat3{ Zeros };
		Rotator quat4{ Rotator::Zero };
		Rotator quat5{ Vector3::Zero };

		EXPECT_EQ(quat0, quat1);
		EXPECT_EQ(quat0, quat2);
		EXPECT_EQ(quat0, quat3);
		EXPECT_EQ(quat0, quat4);
		EXPECT_EQ(quat0, quat5);
	}

	TEST(RotatorTest, Operator)
	{
		Rotator lhs{ 1.0f, 2.0f, 3.0f };
		Rotator rhs{ 3.0f, 2.0f, 1.0f };
		Rotator target{ 4.0f, 4.0f, 4.0f };

		EXPECT_EQ(lhs + rhs, target);

		target.Set(-2.0f, 0.0f, 2.0f);
		EXPECT_EQ(lhs - rhs, target);

		target.Set(3.0f, 6.0f, 9.0f);
		EXPECT_EQ(lhs * 3.0f, target);

		target.Set(2.0f, 4.0f, 6.0f);
		EXPECT_EQ(lhs / 0.5f, target);

		EXPECT_EQ(lhs / 0.0f, lhs);

		EXPECT_NEAR(lhs[0], 1.0f, Epsilon);
		EXPECT_NEAR(lhs[1], 2.0f, Epsilon);
	}

	TEST(RotatorTest, Global)
	{
		Rotator lhs{ 1.0f };
		Rotator rhs{ 2.0f };

		EXPECT_TRUE(IsNearlyEqual(lhs, rhs, 1.0f));
		EXPECT_TRUE(IsNearlyZero(lhs, 1.0f));
	}
}