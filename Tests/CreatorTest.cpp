#include <gtest/gtest.h>
#include "BSMath/Creator.h"

namespace BSMath
{
	TEST(CreatorTest, Matirx)
	{
		
	}

	TEST(CreatorTest, Quaternion)
	{

	}

	TEST(CreatorTest, Rotator)
	{
		using namespace Creator::Rotator;

		Rotator target{ 1.0f, 1.0f, 1.0f };
		EXPECT_EQ(FromEuler(1.0f, 1.0f, 1.0f), target);
		EXPECT_EQ(FromEuler(Vector3::One), target);

		target.Set(90.0f, 0.0f, 90.0f);
		EXPECT_EQ(FromQuaternion(Quaternion{ 0.5f, 0.5f, 0.5f,  0.5f }), target);
	}
}