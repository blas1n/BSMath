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

		EXPECT_EQ(vec0, vec1);
		EXPECT_EQ(vec0, vec2);
		EXPECT_EQ(vec0, vec3);
		EXPECT_EQ(vec0, vec4);
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
		
	}

	TEST(VectorTest, Misc)
	{
		
	}

	TEST(VectorTest, Global)
	{
		
	}
}