#include <cmath>
#include "gtest/gtest.h"
#include "BSMath/Utility.h"

using namespace BSMath;

TEST(UtilityTest, Comparison)
{
	EXPECT_EQ(Min(0, 1), 0);
	EXPECT_EQ(Max(0, 1), 1.0f);

	EXPECT_NEAR(Min(0.0f, 1.0f), 0, Epsilon);
	EXPECT_NEAR(Max(0.0f, 1.0f), 1.0f, Epsilon);

	constexpr int arrayI[10]{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	constexpr float arrayF[10]{ 0.0f, 1.0f, 2.0f,
		3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f };

	EXPECT_EQ(Min(arrayI[0], arrayI[1], arrayI[2], arrayI[3], arrayI[4],
		arrayI[5], arrayI[6], arrayI[7], arrayI[8], arrayI[9]), 0);

	EXPECT_EQ(Max(arrayI[0], arrayI[1], arrayI[2], arrayI[3], arrayI[4],
		arrayI[5], arrayI[6], arrayI[7], arrayI[8], arrayI[9]), 9);

	EXPECT_NEAR(Min(arrayF[0], arrayF[1], arrayF[2], arrayF[3], arrayF[4],
		arrayF[5], arrayF[6], arrayF[7], arrayF[8], arrayF[9]), 0.0f, Epsilon);

	EXPECT_NEAR(Max(arrayF[0], arrayF[1], arrayF[2], arrayF[3], arrayF[4],
		arrayF[5], arrayF[6], arrayF[7], arrayF[8], arrayF[9]), 9.0f, Epsilon);

	EXPECT_EQ(Clamp(2, 1, 3), 2);
	EXPECT_EQ(Clamp(0, 1, 3), 1);
	EXPECT_EQ(Clamp(4, 1, 3), 3);
}

TEST(UtilityTest, Abs)
{
	EXPECT_EQ(Abs(1), 1);
	EXPECT_EQ(Abs(-1), 1);
	EXPECT_NEAR(Abs(1.0f), 1.0f, Epsilon);
	EXPECT_NEAR(Abs(-1.0f), 1.0f, Epsilon);
}

TEST(UtilityTest, Sign)
{
	EXPECT_EQ(Sign(10), 1);
	EXPECT_EQ(Sign(-10), -1);
	EXPECT_NEAR(Sign(10.0f), 1.0f, Epsilon);
	EXPECT_NEAR(Sign(-10.0f), -1.0f, Epsilon);
}

TEST(UtilityTest, GetRangePct)
{
	EXPECT_NEAR(GetRangePct(5, 0, 10), 0.5f, Epsilon);
}

TEST(UtilityTest, Lerp)
{
	EXPECT_NEAR(Lerp(0.0f, 10.0f, 0.5f), 5.0f, Epsilon);
}

TEST(UtilityTest, Sqrt)
{
	for (size_t l = 1; l <= 2; ++l)
	{
		for (float i = 1.0f; i < 10.0f; i += 1.0f)
		{
			const float sqrt = std::sqrt(i);
			EXPECT_NEAR(Sqrt(i, l), sqrt, 0.00001f);
			EXPECT_NEAR(InvSqrt(i, l), 1 / sqrt, 0.00001f);
		}
	}
}

TEST(UtilityTest, FloatToInt)
{
	EXPECT_EQ(Fmod(5.0f, 2.0f), 1.0f);

	EXPECT_EQ(Trunc(0.5f), 0);
	EXPECT_EQ(Ceil(0.5f), 1);
	EXPECT_EQ(Round(0.5f), 1);
	EXPECT_EQ(Floor(0.5f), 0);

	EXPECT_EQ(Trunc(-0.5f), 0);
	EXPECT_EQ(Ceil(-0.5f), 0);
	EXPECT_EQ(Round(-0.5f), -1);
	EXPECT_EQ(Floor(-0.5f), -1);
}
