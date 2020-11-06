#include <gtest/gtest.h>
#include "BSMath/Color.h"
#include "BSMath/Hash.h"
#include "BSMath/Matrix.h"
#include "BSMath/Quaternion.h"
#include "BSMath/Rotator.h"
#include "BSMath/Vector.h"

namespace BSMath
{
	TEST(HashTest, Color)
	{
		const Hash<Color> hash;
		EXPECT_EQ(hash(Color::White), hash(Color::White));
	}

	TEST(HashTest, Matrix)
	{
		EXPECT_EQ(Hash<Matrix2>{}(Matrix2::Identity), Hash<Matrix2>{}(Matrix2::Identity));
		EXPECT_EQ(Hash<Matrix3>{}(Matrix3::Identity), Hash<Matrix3>{}(Matrix3::Identity));
		EXPECT_EQ(Hash<Matrix4>{}(Matrix4::Identity), Hash<Matrix4>{}(Matrix4::Identity));
	}

	TEST(HashTest, Quaternion)
	{
		const Hash<Quaternion> hash;
		EXPECT_EQ(hash(Quaternion::Identity), hash(Quaternion::Identity));
	}

	TEST(HashTest, Rotator)
	{
		const Hash<Rotator> hash;
		EXPECT_EQ(hash(Rotator::Zero), hash(Rotator::Zero));
	}

	TEST(HashTest, Vector)
	{
		EXPECT_EQ(Hash<Vector2>{}(Vector2::Zero), Hash<Vector2>{}(Vector2::Zero));
		EXPECT_EQ(Hash<Vector3>{}(Vector3::Zero), Hash<Vector3>{}(Vector3::Zero));
		EXPECT_EQ(Hash<Vector4>{}(Vector4::Zero), Hash<Vector4>{}(Vector4::Zero));
	}
}