#include <gtest/gtest.h>
#include "BSMath/Vector3.h"
#include "BSMath/Utility.h"

namespace BSMath
{
	constexpr bool IsEqual(const Vector3& lhs, const Vector3& rhs) noexcept
	{
		return IsNearlyEqual(lhs.x, rhs.x) &&
			IsNearlyEqual(lhs.y, rhs.y) && IsNearlyEqual(lhs.z, rhs.z);
	}

	TEST(Vector3Test, Construct)
	{
		constexpr static const float Zeros[3]{ 0.0f, 0.0f, 0.0f };
		
		constexpr Vector3 vec0{};
		constexpr Vector3 vec1{ 0.0f };
		constexpr Vector3 vec2{ 0.0f, 0.0f, 0.0f };
		constexpr Vector3 vec3{ Zeros };
		constexpr Vector3 vec4{ Vector3::Zero() };
	
		bool result = IsEqual(vec0, vec1) && IsEqual(vec0, vec2)
			&& IsEqual(vec0, vec3) && IsEqual(vec0, vec4);

		EXPECT_TRUE(result);
	}

	TEST(Vector3Test, Size)
	{
		Vector3 vec{ 1.0f, 2.0f, 3.0f };
		EXPECT_TRUE(IsNearlyEqual(vec.LengthSquared(), 14.0f));

		vec.Set(3.0f, 3.0f, 3.0f);
		EXPECT_TRUE(vec.Normalize());

		Vector3 target{ 1.0f, 1.0f, 1.0f };
		EXPECT_TRUE(target.Normalize());
		EXPECT_TRUE(IsEqual(vec, target));

		EXPECT_TRUE(IsNearlyZero(Vector3::Distance(vec, target)));
	}

	TEST(Vector3Test, Compare)
	{
		for (float i = -5.0f; i <= 5.1f; ++i)
		{
			for (float j = -5.0f; j <= 5.1f; ++j)
			{
				for (float k = -5.0f; k <= 5.1f; ++k)
				{
					Vector3 lhs{ i, j, k };
					Vector3 rhs{ i, j, k };
					EXPECT_EQ(lhs, rhs);
				}
			}
		}
	}

	TEST(Vector3Test, Operator)
	{
		Vector3 vec{ 1.0f, 0.0f, 0.0f };
		Vector3 target{ -1.0f, 0.0f, 0.0f };
		EXPECT_EQ(-vec, target);

		vec += Vector3{ 9.0f, 5.0f, 3.0f };
		target.Set(10.0f, 5.0f, 3.0f);
		EXPECT_EQ(vec, target);

		vec -= Vector3{ 5.0f, 3.0f, 1.0f };
		target.Set(5.0f, 2.0f, 2.0f);
		EXPECT_EQ(vec, target);

		vec *= Vector3{ 2.0f, 3.0f, 4.0f };
		target.Set(10.0f, 6.0f, 8.0f);
		EXPECT_EQ(vec, target);

		vec *= 2.0f;
		target.Set(20.0f, 12.0f, 16.0f);
		EXPECT_EQ(vec, target);

		vec /= 2.0f;
		target.Set(10.0f, 6.0f, 8.0f);
		EXPECT_EQ(vec, target);

		vec /= Vector3{ 2.0f, 3.0f, 4.0f };
		target.Set(5.0f, 2.0f, 2.0f);
		EXPECT_EQ(vec, target);

		target.Set(2.0f, 2.0f, 3.0f);
		EXPECT_EQ(vec | target, 20.0f);

		vec ^= Vector3{ 1.0f, 2.0f, 3.0f };
		target.Set(2.0f, -13.0f, 8.0f);
		EXPECT_EQ(vec, target);
	}

	TEST(Vector3Test, Accessor)
	{
		constexpr Vector3 vec{ 1.0f, 2.0f, 3.0f };
		EXPECT_EQ(vec[0], 1.0f);
		EXPECT_EQ(vec[1], 2.0f);
		EXPECT_EQ(vec[2], 3.0f);
	}

	TEST(Vector3Test, Global)
	{
		Vector3 lhs{ 5.0f, 2.0f, 5.0f };
		Vector3 rhs{ 2.0f, 5.0f, 2.0f };

		Vector3 result = Min(lhs, rhs);
		Vector3 target{ 2.0f };
		EXPECT_EQ(result, target);

		result = Max(lhs, rhs);
		target.Set(5.0f, 5.0f, 5.0f);
		EXPECT_EQ(result, target);

		result.Set(1.0f, 6.0f, 1.0f);
		result = Clamp(result, lhs, rhs);
		target.Set(2.0f, 5.0f, 2.0f);
		EXPECT_EQ(result, target);

		result = Abs(Vector3{ -10.0f, 40.0f, -15.0f });
		target.Set(10.0f, 40.0f, 15.0f);
		EXPECT_EQ(result, target);

		result = Sign(Vector3{ -10.0f, 40.0f, -15.0f });
		target.Set(-1.0f, 1.0f, -1.0f);
		EXPECT_EQ(result, target);

		result.Set(0.0f, 0.0f, 0.0f);
		EXPECT_TRUE(IsNearlyEqual(result, Vector3::Zero()));
		EXPECT_TRUE(IsNearlyZero(result));

		result = Vector3::One() * 5.0f;
		lhs = Vector3::Zero();
		rhs = Vector3::One() * 10.0f;
		result = GetRangePct(result, lhs, rhs);

		EXPECT_TRUE(IsNearlyEqual(result, Vector3::One() * 0.5f));
	}
}