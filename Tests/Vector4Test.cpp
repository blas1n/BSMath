#include <gtest/gtest.h>
#include "BSMath/Vector4.h"

namespace BSMath
{
	constexpr bool IsEqual(const Vector4& lhs, const Vector4& rhs) noexcept
	{
		return IsNearlyEqual(lhs.x, rhs.x) && IsNearlyEqual(lhs.y, rhs.y)
			&& IsNearlyEqual(lhs.z, rhs.z) && IsNearlyEqual(lhs.w, rhs.w);
	}

	TEST(Vector4Test, Construct)
	{
		constexpr static const float Zeros[4]{ 0.0f, 0.0f, 0.0f, 0.0f };
		
		constexpr Vector4 vec0{};
		constexpr Vector4 vec1{ 0.0f };
		constexpr Vector4 vec2{ 0.0f, 0.0f, 0.0f, 0.0f };
		constexpr Vector4 vec3{ Zeros };
		constexpr Vector4 vec4{ Vector4::Zero() };
	
		const bool result = IsEqual(vec0, vec1) && IsEqual(vec0, vec2)
			&& IsEqual(vec0, vec3) && IsEqual(vec0, vec4);

		EXPECT_TRUE(result);
	}

	TEST(Vector4Test, Size)
	{
		Vector4 vec{ 1.0f, 2.0f, 3.0f, 4.0f };
		EXPECT_TRUE(IsNearlyEqual(vec.LengthSquared(), 30.0f));

		vec.Set(3.0f, 3.0f, 3.0f, 3.0f);
		EXPECT_TRUE(vec.Normalize());

		Vector4 target{ 1.0f, 1.0f, 1.0f, 1.0f };
		EXPECT_TRUE(target.Normalize());
		EXPECT_TRUE(IsEqual(vec, target));

		EXPECT_TRUE(IsNearlyZero(Vector4::Distance(vec, target)));
	}

	TEST(Vector4Test, Compare)
	{
		for (float i = -5.0f; i <= 5.1f; ++i)
		{
			for (float j = -5.0f; j <= 5.1f; ++j)
			{
				for (float k = -5.0f; k <= 5.1f; ++k)
				{
					for (float l = -5.0f; l <= 5.1f; ++l)
					{
						Vector4 lhs{ i, j, k, l };
						Vector4 rhs{ i, j, k, l };
						EXPECT_EQ(lhs, rhs);
					}
				}
			}
		}
	}

	TEST(Vector4Test, Operator)
	{
		Vector4 vec{ 1.0f, 0.0f, 0.0f, 0.0f };
		Vector4 target{ -1.0f, 0.0f, 0.0f, 0.0f };
		EXPECT_EQ(-vec, target);

		vec += Vector4{ 9.0f, 5.0f, 3.0f, 1.0f };
		target.Set(10.0f, 5.0f, 3.0f, 1.0f);
		EXPECT_EQ(vec, target);

		vec -= Vector4{ 5.0f, 3.0f, 1.0f, 2.0f };
		target.Set(5.0f, 2.0f, 2.0f, -1.0f);
		EXPECT_EQ(vec, target);

		vec *= Vector4{ 2.0f, 3.0f, 4.0f, 3.0f };
		target.Set(10.0f, 6.0f, 8.0f, -3.0f);
		EXPECT_EQ(vec, target);

		vec *= 2.0f;
		target.Set(20.0f, 12.0f, 16.0f, -6.0f);
		EXPECT_EQ(vec, target);

		vec /= 2.0f;
		target.Set(10.0f, 6.0f, 8.0f, -3.0f);
		EXPECT_EQ(vec, target);

		vec /= Vector4{ 2.0f, 3.0f, 4.0f, 3.0f };
		target.Set(5.0f, 2.0f, 2.0f, -1.0f);
		EXPECT_EQ(vec, target);

		target.Set(2.0f, 2.0f, 3.0f, 3.0f);
		EXPECT_EQ(vec | target, 17.0f);
	}

	TEST(Vector4Test, Accessor)
	{
		constexpr Vector4 vec{ 1.0f, 2.0f, 3.0f, 4.0f };
		EXPECT_EQ(vec[0], 1.0f);
		EXPECT_EQ(vec[1], 2.0f);
		EXPECT_EQ(vec[2], 3.0f);
		EXPECT_EQ(vec[3], 4.0f);
	}

	TEST(Vector4Test, Global)
	{
		Vector4 lhs{ 5.0f, 2.0f, 5.0f, 2.0f };
		Vector4 rhs{ 2.0f, 5.0f, 2.0f, 5.0f };

		Vector4 result = Min(lhs, rhs);
		Vector4 target{ 2.0f };
		EXPECT_EQ(result, target);

		result = Max(lhs, rhs);
		target = Vector4::One() * 5.0f;
		EXPECT_EQ(result, target);

		result.Set(1.0f, 6.0f, 1.0f, 6.0f);
		result = Clamp(result, lhs, rhs);
		target.Set(2.0f, 5.0f, 2.0f, 5.0f);
		EXPECT_EQ(result, target);

		result = Abs(Vector4{ -10.0f, 40.0f, -15.0f, 20.0f });
		target.Set(10.0f, 40.0f, 15.0f, 20.0f);
		EXPECT_EQ(result, target);

		result = Sign(Vector4{ -10.0f, 40.0f, -15.0f, 20.0f });
		target.Set(-1.0f, 1.0f, -1.0f, 1.0f);
		EXPECT_EQ(result, target);

		result.Set(0.0f, 0.0f, 0.0f, 0.0f);
		EXPECT_TRUE(IsNearlyEqual(result, Vector4::Zero()));
		EXPECT_TRUE(IsNearlyZero(result));

		result = Vector4::One() * 5.0f;
		lhs = Vector4::Zero();
		rhs = Vector4::One() * 10.0f;
		result = GetRangePct(result, lhs, rhs);
		EXPECT_TRUE(IsNearlyEqual(result, Vector4::One() * 0.5f));

		result = Lerp(lhs, rhs, 0.5f);
		EXPECT_TRUE(IsNearlyEqual(result, Vector4::One() * 5.0f));
	}
}