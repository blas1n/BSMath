#include <cmath>
#include <gtest/gtest.h>
#include "BSMath/Vector2.h"

namespace BSMath
{
	TEST(VectorTest, Construct)
	{
		const static float Zeros[2]{ 0.0f, 0.0f };

		Vector2 vec0;
		Vector2 vec1(0.0f);
		Vector2 vec2{ 0.0f, 0.0f };
		Vector2 vec3(Zeros);
		Vector2 vec4(Vector2::Zero);
		Vector2 vec5(vec0);
		Vector2 vec6;
		vec6 = vec0;

		EXPECT_EQ(vec0, vec1);
		EXPECT_EQ(vec0, vec2);
		EXPECT_EQ(vec0, vec3);
		EXPECT_EQ(vec0, vec4);
		EXPECT_EQ(vec0, vec5);
		EXPECT_EQ(vec0, vec6);

		EXPECT_NEAR(vec0.x, vec6.x, Epsilon);
		EXPECT_NEAR(vec0.y, vec6.y, Epsilon);
	}

	TEST(VectorTest, Size)
	{
		Vector2 vec{ 3.0f, 4.0f };
		EXPECT_NEAR(vec.GetMin(), 3.0f, Epsilon);
		EXPECT_NEAR(vec.GetMax(), 4.0f, Epsilon);

		EXPECT_NEAR(vec.LengthSquared(), 25.0f, Epsilon);

		Vector2 target{ 1, 1 };
		EXPECT_NEAR(Vector2::DistanceSquared(vec, target), 13.0f, Epsilon);

		vec.Normalize();
		EXPECT_NEAR(vec.x, 3.0f / 5.0f, Epsilon);
		EXPECT_NEAR(vec.y, 4.0f / 5.0f, Epsilon);
	}

	TEST(VectorTest, Operator)
	{
		Vector2 lhs{ 1.0f, 2.0f };
		Vector2 rhs{ 2.0f, 1.0f };
		Vector2 target{ 3.0f, 3.0f };

		EXPECT_EQ(lhs + rhs, target);
		
		target.Set(-1.0f, 1.0f);
		EXPECT_EQ(lhs - rhs, target);

		target.Set(2.0f, 2.0f);
		EXPECT_EQ(lhs * rhs, target);

		target.Set(0.5f, 2.0f);
		EXPECT_EQ(lhs / rhs, target);

		target.Set(3.0f, 6.0f);
		EXPECT_EQ(lhs * 3.0f, target);

		target.Set(2.0f, 4.0f);
		EXPECT_EQ(lhs / 0.5f, target);

		EXPECT_EQ(lhs / Vector2::Zero, lhs);
		EXPECT_EQ(lhs / 0.0f, lhs);

		EXPECT_NEAR(lhs | rhs, 4.0f, Epsilon);

		EXPECT_NEAR(lhs(0, 0), 1.0f, Epsilon);
		EXPECT_NEAR(lhs(0, 1), 2.0f, Epsilon);

		EXPECT_NEAR(lhs[0], 1.0f, Epsilon);
		EXPECT_NEAR(lhs[1], 2.0f, Epsilon);

		Vector2 result{ lhs.x + rhs.y, lhs.y + rhs.x };
		target.Set(2.0f, 4.0f);
		EXPECT_EQ(result, target);
	}

	TEST(VectorTest, Global)
	{
		
	}
}