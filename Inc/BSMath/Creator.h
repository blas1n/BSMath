#pragma once

#include "Matrix.h"
#include "Quaternion.h"
#include "Rotator.h"

namespace BSMath::Creator
{
	namespace Detail
	{
		NO_ODR void GetSinCosOnce(float angle, float& c, float& s)
		{
			if (BSMath::IsNearlyEqual(angle, 0.0f))
			{
				c = 1.0f;
				s = 0.0f;
			}
			else if (BSMath::IsNearlyEqual(angle, 90.0f))
			{
				c = 0.0f;
				s = 1.0f;
			}
			else if (BSMath::IsNearlyEqual(angle, 180.0f))
			{
				c = -1.0f;
				s = 0.0f;
			}
			else if (BSMath::IsNearlyEqual(angle, 270.0f))
			{
				c = 0.0f;
				s = -1.0f;
			}
			else
			{
				const float rad = BSMath::Deg2Rad(angle);
				c = BSMath::Cos(rad);
				s = BSMath::Sin(rad);
			}
		}

		NO_ODR void GetSinCos(const BSMath::Rotator& rot, float& cy, float& sy, float& cp, float& sp, float& cr, float& sr)
		{
			GetSinCosOnce(rot.yaw, cy, sy);
			GetSinCosOnce(rot.pitch, cp, sp);
			GetSinCosOnce(rot.roll, cr, sr);
		}
	}

	// Todo: Add ortho, perspective, lookat
	namespace Matrix
	{
		template <size_t L>
		[[nodiscard]] NO_ODR BSMath::Matrix<float, L> FromVectors(const BSMath::Vector<float, L>(&vecs)[L]) noexcept
		{
			BSMath::Matrix<float, L> ret;
			for (size_t i = 0; i < L; ++i)
				std::copy_n(&(vecs[i].x), L, ret[i]);
			return ret;
		}

		[[nodiscard]] NO_ODR BSMath::Matrix4 FromTranslation(const BSMath::Vector3& pos) noexcept
		{
			return BSMath::Matrix4
			{
				 1.0f,  0.0f,  0.0f, 0.0f,
				 0.0f,  1.0f,  0.0f, 0.0f,
				 0.0f,  0.0f,  1.0f, 0.0f,
				pos.x, pos.y, pos.z, 1.0f
			};
		}

		[[nodiscard]] NO_ODR BSMath::Matrix3 FromScale(const BSMath::Vector3& scale) noexcept
		{
			return BSMath::Matrix3
			{
				scale.x,    0.0f,    0.0f,
				   0.0f, scale.y,    0.0f,
				   0.0f,    0.0f, scale.z
			};
		}

		[[nodiscard]] NO_ODR BSMath::Matrix3 FromQuaternion(const BSMath::Quaternion& quat) noexcept
		{
			// Ref: https://github.com/bulletphysics/bullet3/blob/master/src/LinearMath/btMatrix3x3.h

			const float s = 2.0f / (quat | quat);
			const float xs = quat.x *  s, ys = quat.y *  s, zs = quat.z *  s;
			const float wx = quat.w * xs, wy = quat.w * ys, wz = quat.w * zs;
			const float xx = quat.x * xs, xy = quat.x * ys, xz = quat.x * zs;
			const float yy = quat.y * ys, yz = quat.y * zs, zz = quat.z * zs;

			return BSMath::Matrix3
			{
				1.0f - (yy + zz),         xy - wz,         xz + wy,
						 xy + xz, 1.0f - (xx + zz),        yz - wx,
						 xz - wy,         yz + wx, 1.0f - (xx + yy)
			};
		}

		[[nodiscard]] NO_ODR BSMath::Matrix3 FromRotator(const BSMath::Rotator& rot) noexcept
		{
			// Ref: https://github.com/bulletphysics/bullet3/blob/master/src/LinearMath/btMatrix3x3.h

			float cy, sy, cp, sp, cr, sr;
			Detail::GetSinCos(rot, cy, sy, cp, sp, cr, sr);

			const float cc = cr * cy;
			const float cs = cr * sy;
			const float sc = sr * cy;
			const float ss = sr * sy;

			return BSMath::Matrix3
			{
				cp * cy, sp * sc - cs, sp * cc + ss,
				cp * sy, sp * ss + cc, sp * cs - sc,
					-sp,      cp * sr,      cp * cr
			};
		}

		[[nodiscard]] NO_ODR BSMath::Matrix4 FromTRS(const BSMath::Vector3& pos,
			const BSMath::Rotator& rot, const BSMath::Vector3& scale) noexcept
		{
			// Ref: https://github.com/bulletphysics/bullet3/blob/master/src/LinearMath/btMatrix3x3.h

			float cy, sy, cp, sp, cr, sr;
			Detail::GetSinCos(rot, cy, sy, cp, sp, cr, sr);

			return BSMath::Matrix4
			{
				                (cp * cy) * scale.x,                (cp * sy) * scale.x,         sp * scale.x, 0.0f,
				 (sr * sp * cy - cr * sy) * scale.y, (sr * sp * sy + cr * cy) * scale.y, (-sr * cp) * scale.y, 0.0f,
				-(cr * sp * cy + sr * sy) * scale.z, (cy * sr - cr * sp * sy) * scale.z,  (cr * cp) * scale.z, 0.0f,
				                              pos.x,                              pos.y,                pos.z, 1.0f
			};
		}

		[[nodiscard]] NO_ODR BSMath::Matrix4 FromLookAt(const Vector3& eye, const Vector3& target, const Vector3& up)
		{
			const auto zaxis = Vector3::GetNormal(target - eye);
			const auto xaxis = Vector3::GetNormal(up ^ zaxis);
			const auto yaxis = Vector3::GetNormal(zaxis ^ xaxis);

			const Vector3 trans{ -(xaxis | eye), -(yaxis | eye), -(zaxis | eye)	};
			return Matrix4
			{
				xaxis.x, yaxis.x, zaxis.x, 0.0f,
				xaxis.y, yaxis.y, zaxis.y, 0.0f,
				xaxis.z, yaxis.z, zaxis.z, 0.0f,
				trans.x, trans.y, trans.z, 1.0f
			};
		}

		[[nodiscard]] NO_ODR BSMath::Matrix4 FromOrtho(const Vector2& screen, float near, float far)
		{
			return Matrix4
			{
				2.0f / screen.x,            0.0f,                0.0f, 0.0f,
						   0.0f, 2.0f / screen.y,                0.0f, 0.0f,
						   0.0f,            0.0f, 1.0f / (far - near), 0.0f,
						   0.0f,            0.0f, near / (near - far), 1.0f
			};
		}

		[[nodiscard]] NO_ODR BSMath::Matrix4 FromPerspective(const Vector2& screen, float near, float far, float fov)
		{
			const auto yScale = 1.0f / Tan(fov * 0.5f);
			const auto xScale = yScale * screen.y / screen.x;

			return Matrix4
			{
				xScale,   0.0f,                       0.0f, 0.0f,
				  0.0f, yScale,                       0.0f, 0.0f,
				  0.0f,   0.0f,         far / (far - near), 0.0f,
				  0.0f,   0.0f, -near * far / (far - near), 1.0f
			};
		}

		[[nodiscard]] NO_ODR BSMath::Matrix4 FromViewProjection(const Vector2& screen)
		{
			return Matrix4
			{
				2.0f / screen.x,            0.0f, 0.0f, 0.0f,
				           0.0f, 2.0f / screen.y, 0.0f, 0.0f,
				           0.0f,            0.0f, 1.0f, 0.0f,
				           0.0f,            0.0f, 1.0f, 1.0f
			};
		}
	}

	namespace Quaternion
	{
		[[nodiscard]] NO_ODR BSMath::Quaternion FromRotator(const BSMath::Rotator& rot) noexcept
		{
			// Ref: https://github.com/bulletphysics/bullet3/blob/master/src/LinearMath/btQuaternion.h

			float cy, sy, cp, sp, cr, sr;
			Detail::GetSinCos(rot * 0.5f, cy, sy, cp, sp, cr, sr);

			return BSMath::Quaternion
			{
				sr * cp * cy - cr * sp * sy,
				cr * sp * cy + sr * cp * sy,
				cr * cp * sy - sr * sp * cy,
				cr * cp * cy + sr * sp * sy
			};
		}

		[[nodiscard]] NO_ODR BSMath::Quaternion FromEuler(float roll, float pitch, float yaw) noexcept
		{
			return FromRotator(BSMath::Rotator{ roll, pitch, yaw });
		}

		[[nodiscard]] NO_ODR BSMath::Quaternion FromEuler(const BSMath::Vector3& euler) noexcept
		{
			return FromRotator(BSMath::Rotator{ euler.x, euler.y, euler.z });
		}

		[[nodiscard]] NO_ODR BSMath::Quaternion FromAngleAxis(const BSMath::Vector3& axis, float angle) noexcept
		{
			const auto half = angle * 0.5f;
			const auto vec = axis * Sin(half);
			return BSMath::Quaternion{ vec.x, vec.y, vec.z, Cos(half) };
		}

		[[nodiscard]] NO_ODR BSMath::Quaternion FromMatrix(const BSMath::Matrix3& mat) noexcept
		{
			// Ref: https://github.com/bulletphysics/bullet3/blob/master/src/LinearMath/btMatrix3x3.h

			const float trace = mat[0][0] + mat[1][1] + mat[2][2];
			float temp[4];

			if (trace > 0.0f)
			{
				float s = BSMath::Sqrt(trace + 1.0f);
				temp[3] = s * 0.5f;
				
				s = 0.5f / s;
				temp[0] = (mat[2][1] - mat[1][2]) * s;
				temp[1] = (mat[0][2] - mat[2][0]) * s;
				temp[2] = (mat[1][0] - mat[0][1]) * s;
			}
			else
			{
				const int i = mat[0][0] < mat[1][1] ? (mat[1][1] < mat[2][2] ? 2 : 1) : (mat[0][0] < mat[2][2] ? 2 : 0);
				const int j = (i + 1) % 3;
				const int k = (i + 2) % 3;

				float s = BSMath::Sqrt(mat[i][i] - mat[j][j] - mat[k][k] + 1.0f);
				temp[i] = s * 0.5f;
				
				s = 0.5f / s;
				temp[3] = (mat[k][j] - mat[j][k]) * s;
				temp[j] = (mat[j][i] + mat[i][j]) * s;
				temp[k] = (mat[k][i] + mat[i][k]) * s;
			}

			return BSMath::Quaternion{ temp };
		}
	}

	namespace Rotator
	{		
		[[nodiscard]] NO_ODR BSMath::Rotator FromQuaternion(const BSMath::Quaternion& quat) noexcept
		{
			// Ref: https://github.com/bulletphysics/bullet3/blob/master/src/LinearMath/btQuaternion.h

			constexpr static float Threshold = 0.99999f;
			const float test = -2.0f * (quat.x * quat.z - quat.w * quat.y);
			BSMath::Rotator ret;

			// If the pitch angle is PI/2 or -PI/2, we can only compute
			// the sum roll + yaw.  However, any combination that gives
			// the right sum will produce the correct orientation, so we
			// set rollX = 0 and compute yawZ.
			if (test >= BSMath::Abs(Threshold))
			{
				const float sign = BSMath::Sign(Threshold);
				ret.roll = 0.0f;
				ret.pitch = BSMath::Rad2Deg(BSMath::Pi * 0.5f * sign);
				ret.yaw = 2.0f * BSMath::Rad2Deg(BSMath::Atan2(quat.x, quat.y * sign));
			}
			else
			{
				const float sqx = BSMath::Square(quat.x);
				const float sqy = BSMath::Square(quat.y);
				const float sqz = BSMath::Square(quat.z);
				const float sqw = BSMath::Square(quat.w);

				ret.roll = BSMath::Rad2Deg(BSMath::Atan2(2.0f *
					(quat.y * quat.z + quat.w * quat.x), sqw - sqx - sqy + sqz));

				ret.pitch = BSMath::Rad2Deg(BSMath::Asin(test));
				
				ret.yaw = BSMath::Rad2Deg(BSMath::Atan2(2.0f *
					(quat.x * quat.y + quat.w * quat.z), sqw + sqx - sqy - sqz));
			}

			return ret;
		}

		[[nodiscard]] constexpr BSMath::Rotator FromEuler(float roll, float pitch, float yaw) noexcept
		{
			return BSMath::Rotator{ roll, pitch, yaw };
		}

		[[nodiscard]] constexpr BSMath::Rotator FromEuler(const BSMath::Vector3& euler) noexcept
		{
			return BSMath::Rotator{ euler.x, euler.y, euler.z };
		}

		[[nodiscard]] NO_ODR BSMath::Rotator FromMatrix(const BSMath::Matrix3& mat) noexcept
		{
			// Ref: https://github.com/bulletphysics/bullet3/blob/master/src/LinearMath/btMatrix3x3.h

			BSMath::Rotator ret;

			// Check that pitch is not at a singularity
			if (BSMath::Abs(mat[2][0]) >= 1.0f)
			{
				ret.yaw = 0.0f;

				// From difference of angles formula
				const float delta = BSMath::Atan2(mat[0][0], mat[0][2]);
				const float sign = BSMath::Sign(mat[2][0]);

				ret.pitch = BSMath::Rad2Deg(BSMath::Pi * sign * 0.5f);
				ret.roll = BSMath::Rad2Deg((ret.pitch * sign) + delta);
			}
			else
			{
				ret.pitch = -BSMath::Rad2Deg(BSMath::Asin(mat[2][0]));

				ret.roll = BSMath::Rad2Deg(BSMath::Atan2(mat[2][1] / BSMath::Cos(ret.pitch),
					mat[2][2] / BSMath::Cos(ret.pitch)));

				ret.yaw = BSMath::Rad2Deg(BSMath::Atan2(mat[1][0] / BSMath::Cos(ret.pitch),
					mat[0][0] / BSMath::Cos(ret.pitch)));
			}

			return ret;
		}
	}
}
