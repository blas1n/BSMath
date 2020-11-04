#include <gtest/gtest.h>
#include "BSMath/Color.h"

namespace BSMath
{
	TEST(ColorTest, Operator)
	{
		Color lhs = Color::White;
		Color rhs = Color::White;
		EXPECT_EQ(lhs + rhs, lhs);

		lhs = Color::Black;
		const auto target = Color{ 0, 0, 0, 0 };
		EXPECT_EQ(lhs - rhs, target);
	}
}