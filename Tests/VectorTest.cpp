#include <cmath>
#include <gtest/gtest.h>
#include "BSMath/Vector2.h"
#include "BSMath/Vector3.h"
#include "BSMath/Vector4.h"

namespace BSMath
{
	TEST(VectorTest, Construct)
	{
		const static float Zeros[3]{ 0.0f, 0.0f, 0.0f };

		Vector3 vec0;
		Vector3 vec1(0.0f);
		Vector3 vec2{ 0.0f, 0.0f, 0.0f };
		Vector3 vec3(Zeros);
		Vector3 vec4(vec0);
		Vector3 vec5;
		vec5 = vec0;
		
		Vector3 vec6(Vector2::Zero);
		Vector3 vec7(Vector3::Zero);
		Vector3 vec8(Vector4::Zero);
		Vector3 vec9(IntVector2::Zero);
		Vector3 vec10(IntVector3::Zero);
		Vector3 vec11(IntVector4::Zero);

		EXPECT_EQ(vec0, vec1);
		EXPECT_EQ(vec0, vec2);
		EXPECT_EQ(vec0, vec3);
		EXPECT_EQ(vec0, vec4);
		EXPECT_EQ(vec0, vec5);
		EXPECT_EQ(vec0, vec6);
		EXPECT_EQ(vec0, vec7);
		EXPECT_EQ(vec0, vec8);
		EXPECT_EQ(vec0, vec9);
		EXPECT_EQ(vec0, vec10);
		EXPECT_EQ(vec0, vec11);

		EXPECT_NEAR(vec0.X(), vec6.X(), Epsilon);
		EXPECT_NEAR(vec0.Y(), vec6.Y(), Epsilon);
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
		EXPECT_NEAR(vec.X(), 3.0f / 5.0f, Epsilon);
		EXPECT_NEAR(vec.Y(), 4.0f / 5.0f, Epsilon);
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

		Vector2 result{ lhs.X() + rhs.Y(), lhs.Y() + rhs.X() };
		target.Set(2.0f, 4.0f);
		EXPECT_EQ(result, target);

		Vector3 lhs3{ 1.0f, 2.0f, 3.0f };
		Vector3 rhs3{ 3.0f, 2.0f, 1.0f };
		Vector3 result3 = lhs3 ^ rhs3;
		Vector3 target3{ -4.0f, 8.0f, -4.0f };
		EXPECT_EQ(result3, target3);
	}

	TEST(VectorTest, Global)
	{
		Vector2 lhs{ 1.0f, 2.0f };
		Vector2 rhs{ 2.0f, 1.0f };

		Vector2 result = Min(lhs, rhs);
		Vector2 target{ 1.0f, 1.0f };
		EXPECT_EQ(result, target);

		result = Max(lhs, rhs);
		target.Set(2.0f, 2.0f);
		EXPECT_EQ(result, target);

		result.Set(3.0f, 0.0f);
		result = Clamp(result, lhs, rhs);
		target.Set(2.0f, 1.0f);
		EXPECT_EQ(result, target);

		result.Set(-10.0f, 10.0f);
		result = Abs(result);
		target.Set(10.0f, 10.0f);
		EXPECT_EQ(result, target);

		result.Set(-10.0f, 10.0f);
		result = Sign(result);
		target.Set(-1.0f, 1.0f);
		EXPECT_EQ(result, target);

		result = target = Vector2::Zero;
		EXPECT_TRUE(IsNearlyEqual(result, target));
		EXPECT_TRUE(IsNearlyZero(result));

		lhs.Set(0.0f, 0.0f);
		rhs.Set(10.0f, 10.0f);
		result = Vector2::One * 5.0f;
		result = GetRangePct(result, lhs, rhs);
		target.Set(0.5f, 0.5f);
		EXPECT_TRUE(IsNearlyEqual(result, target));

		result = Lerp(lhs, rhs, 0.5f);
		target = Vector2::One * 5.0f;
		EXPECT_TRUE(IsNearlyEqual(result, target));
	}
}