#pragma once

#include "BSBase/Base.h"
#include "BSBase/Type.h"

namespace BSMath
{
	using namespace BSBase;

	template <class T, size_t L>
	struct Vector;

	using Vector2 = Vector<float, 2>;
	using IntVector2 = Vector<int, 2>;
	using Vector3 = Vector<float, 3>;
	using IntVector3 = Vector<int, 3>;
	using Vector4 = Vector<float, 4>;
	using IntVector4 = Vector<int, 4>;

	template <class T, size_t L>
	struct Matrix;

	using Matrix2 = Matrix<float, 2>;
	using Matrix3 = Matrix<float, 3>;
	using Matrix4 = Matrix<float, 4>;

	struct Quaternion;
	struct Rotator;
}