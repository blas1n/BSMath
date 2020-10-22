#include <gtest/gtest.h>
#include "BSMath/Matrix.h"

namespace BSMath
{
	TEST(MatrixTest, Determinant)
	{
		Matrix4 mat
		{
			 5.0f,  4.0f, 12.0f,  7.0f,
			14.0f,  9.0f,  8.0f,  3.0f,
			 6.0f, 10.0f,  1.0f,  0.0f,
			11.0f,  6.0f,  3.0f,  8.0f
		};

		const float ret =
			mat[0][0] * (
				mat[1][1] * (mat[2][2] * mat[3][3] - mat[2][3] * mat[3][2]) -
				mat[2][1] * (mat[1][2] * mat[3][3] - mat[1][3] * mat[3][2]) +
				mat[3][1] * (mat[1][2] * mat[2][3] - mat[1][3] * mat[2][2])
			) -
			mat[1][0] * (
				mat[0][1] * (mat[2][2] * mat[3][3] - mat[2][3] * mat[3][2]) -
				mat[2][1] * (mat[0][2] * mat[3][3] - mat[0][3] * mat[3][2]) +
				mat[3][1] * (mat[0][2] * mat[2][3] - mat[0][3] * mat[2][2])
			) +
			mat[2][0] * (
				mat[0][1] * (mat[1][2] * mat[3][3] - mat[1][3] * mat[3][2]) -
				mat[1][1] * (mat[0][2] * mat[3][3] - mat[0][3] * mat[3][2]) +
				mat[3][1] * (mat[0][2] * mat[1][3] - mat[0][3] * mat[1][2])
			) -
			mat[3][0] * (
				mat[0][1] * (mat[1][2] * mat[2][3] - mat[1][3] * mat[2][2]) -
				mat[1][1] * (mat[0][2] * mat[2][3] - mat[0][3] * mat[2][2]) +
				mat[2][1] * (mat[0][2] * mat[1][3] - mat[0][3] * mat[1][2])
			);

		EXPECT_NEAR(mat.Determinant(), ret, Epsilon);
	}

	TEST(MatrixTest, Inverse)
	{
		Matrix4 lhs
		{
			 1.0f,  2.0f,  1.0f,  1.0f,
			 1.0f,  1.0f, -1.0f, -2.0f,
			 1.0f, -1.0f, -1.0f,  2.0f,
			 1.0f, -2.0f,  1.0f, -1.0f
		};

		Matrix4 rhs
		{
			 0.25f,  0.25f,  0.25f,  0.25f,
			  0.2f,   0.1f,  -0.1f,  -0.2f,
			 0.25f, -0.25f, -0.25f,  0.25f,
			  0.1f,  -0.2f,   0.2f,  -0.1f
		};

		EXPECT_EQ(lhs.GetInvert(), rhs);
	}

	TEST(MatrixTest, Transpose)
	{
		Matrix4 lhs
		{
			 5.0f,  4.0f, 12.0f,  7.0f,
			14.0f,  9.0f,  8.0f,  3.0f,
			 6.0f, 10.0f,  1.0f,  0.0f,
			11.0f,  6.0f,  3.0f,  8.0f
		};

		Matrix4 rhs
		{
			 5.0f, 14.0f,  6.0f, 11.0f,
			 4.0f,  9.0f, 10.0f,  6.0f,
			12.0f,  8.0f,  1.0f,  3.0f,
			 7.0f,  3.0f,  0.0f,  8.0f
		};

		EXPECT_EQ(lhs.GetTranspose(), rhs);

		lhs.Transpose();
		EXPECT_EQ(lhs.GetTranspose(), rhs.GetTranspose());
	}

	TEST(MatrixTest, Multiply)
	{
		Matrix4 lhs
		{
			 5.0f,  4.0f, 12.0f,  7.0f,
			14.0f,  9.0f,  8.0f,  3.0f,
			 6.0f, 10.0f,  1.0f,  0.0f,
			11.0f,  6.0f,  3.0f,  8.0f
		};

		Matrix4 rhs
		{
			234.0f, 223.0f,  82.0f, 171.0f,
			223.0f, 350.0f, 182.0f, 256.0f,
			 82.0f, 182.0f, 137.0f, 129.0f,
			171.0f, 256.0f, 129.0f, 230.0f
		};

		EXPECT_EQ(lhs * lhs.GetTranspose(), rhs);
	}
}