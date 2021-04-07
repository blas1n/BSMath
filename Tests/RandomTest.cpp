#include "gtest/gtest.h"
#include "BSMath/Color.h"
#include "BSMath/Rotator.h"
#include "BSMath/Vector.h"

// This test only checks whether the build was successful. So, the results are not tested.
using namespace BSMath;

TEST(Random, Color)
{
	ColorRandom colorRand;
	(void)colorRand();
}

TEST(Random, Rotator)
{
	RotatorRandom rotatorRand;
	(void)rotatorRand();
}

TEST(Random, Vector)
{
	Vector2Random vec2Rand;
	(void)vec2Rand();
	(void)vec2Rand(Vector2Random::Parameter{ 0.0f, 1.0f });

	IntVector2Random intVec2Rand;
	(void)intVec2Rand();
	(void)intVec2Rand(IntVector2Random::Parameter{ 0, 1 });

	Vector3Random vec3Rand;
	(void)vec3Rand();
	(void)vec3Rand(Vector3Random::Parameter{ 0.0f, 1.0f });

	IntVector3Random intVec3Rand;
	(void)intVec3Rand();
	(void)intVec3Rand(IntVector3Random::Parameter{ 0, 1 });

	Vector4Random vec4Rand;
	(void)vec4Rand();
	(void)vec4Rand(Vector4Random::Parameter{ 0.0f, 1.0f });

	IntVector4Random intVec4Rand;
	(void)intVec4Rand();
	(void)intVec4Rand(IntVector4Random::Parameter{ 0, 1 });
}
