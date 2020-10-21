#include <gtest/gtest.h>
#include "BSMath/Quaternion.h"

namespace BSMath
{
	TEST(QuaternionTest, Constructor)
	{
		const static float Zeros[4]{ 0.0f, 0.0f, 0.0f, 1.0f };

		Quaternion quat0;
		Quaternion quat1{ 0.0f, 0.0f, 0.0f, 1.0f };
		Quaternion quat2{ Quaternion::Identity };
		Quaternion quat3{ Zeros };
		Quaternion quat4{ Vector3::Zero, 0.0f };

		EXPECT_EQ(quat0, quat1);
		EXPECT_EQ(quat0, quat2);
		EXPECT_EQ(quat0, quat3);
		EXPECT_EQ(quat0, quat4);
	}

	TEST(QuaternionTest, Multiply)
	{
		Quaternion lhs{ 0.0f, 1.0f, 0.0f, 1.0f };
		Quaternion rhs{ 0.5f, 0.5f, 0.75f, 1.0f };
		Quaternion ret{ 1.25f, 1.5f, 0.25f, 0.5f };
		EXPECT_EQ(lhs * rhs, ret);
	}

	TEST(QuaternionTest, Global)
	{
		Quaternion lhs{ 0.0f, 1.0f, 0.0f, 1.0f };
		Quaternion rhs{ 0.5f, 0.5f, 0.75f, 1.0f };
		Quaternion ret{ 0.13333333f, 0.26666667f, 0.2f, 0.4f };
		EXPECT_EQ(Lerp(lhs, rhs, 0.5f), ret);
	}
}