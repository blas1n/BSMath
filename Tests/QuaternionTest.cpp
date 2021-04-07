#include "gtest/gtest.h"
#include "BSMath/Quaternion.h"

using namespace BSMath;

TEST(QuaternionTest, Constructor)
{
	const static float Zeros[4]{ 0.0f, 0.0f, 0.0f, 1.0f };

	Quaternion quat0;
	Quaternion quat1{ 0.0f, 0.0f, 0.0f, 1.0f };
	Quaternion quat2{ Quaternion::Identity };
	Quaternion quat3{ Zeros };

	EXPECT_EQ(quat0, quat1);
	EXPECT_EQ(quat0, quat2);
	EXPECT_EQ(quat0, quat3);
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

	Quaternion ret{ 0.24806947f, 0.49613894f, 0.3721042f , 0.74420841f };
	EXPECT_TRUE(IsNearlyEqual(Lerp(lhs, rhs, 0.5f), ret));

	ret.Set(0.18814417f, 0.56443252f, 0.28221626f, 0.75257669f);
	EXPECT_TRUE(IsNearlyEqual(Slerp(lhs, rhs, 0.5f), ret));
}
