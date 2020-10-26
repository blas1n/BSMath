#include <gtest/gtest.h>
#include "BSMath/Creator.h"

namespace BSMath
{
	TEST(CreatorTest, Matirx)
	{
		using namespace Creator::Matrix;

		Vector3 vecs[3]{ Vector3::Right, Vector3::Up, Vector3::Forward };
		EXPECT_EQ(FromVectors(vecs), Matrix3::Identity);

		{
			Matrix4 target
			{
				 1.0f,  0.0f,  0.0f, 0.0f,
				 0.0f,  1.0f,  0.0f, 0.0f,
				 0.0f,  0.0f,  1.0f, 0.0f,
				10.0f, 10.0f, 10.0f, 1.0f
			};
			EXPECT_EQ(FromTranslation(Vector3::One * 10.0f), target);
		}

		Matrix3 target;
		target[0][0] = 10.0f;
		target[1][1] = 10.0f;
		target[2][2] = 10.0f;

		EXPECT_EQ(FromScale(Vector3::One * 10.0f), target);

		target = Matrix4::Zero;
		target[0][2] = 1.0f;
		target[1][0] = 1.0f;
		target[2][1] = 1.0f;
		target[3][3] = 1.0f;

		EXPECT_EQ(FromQuaternion(Quaternion{ 0.5f, 0.5f, 0.5f, 0.5f }), target);
		EXPECT_EQ(FromRotator(Rotator{ 90.0f, 0.0f, 90.0f }), target);

		EXPECT_EQ(FromTRS(Vector3::Zero, Rotator::Zero, Vector3::One), Matrix4::Identity);
	}

	TEST(CreatorTest, Quaternion)
	{
		using namespace Creator::Quaternion;

		Quaternion target{ 0.5f, 0.5f, 0.5f, 0.5f };
		EXPECT_TRUE(IsNearlyEqual(FromEuler(90.0f, 0.0f, 90.0f), target));

		target.Set(0.0f, 0.0f, 0.0f, 1.0f);
		EXPECT_EQ(FromAngleAxis(Vector3::Zero, 0.0f), target);
	}

	TEST(CreatorTest, Rotator)
	{
		using namespace Creator::Rotator;

		Rotator target{ 1.0f, 1.0f, 1.0f };
		EXPECT_EQ(FromEuler(1.0f, 1.0f, 1.0f), target);
		EXPECT_EQ(FromEuler(Vector3::One), target);

		target.Set(90.0f, 0.0f, 90.0f);
		EXPECT_TRUE(IsNearlyEqual(FromQuaternion(Quaternion{ 0.5f, 0.5f, 0.5f, 0.5f }), target));
	}
}