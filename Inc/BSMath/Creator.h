#pragma once

#include "Matrix.h"
#include "Quaternion.h"
#include "Rotator.h"

namespace BSMath::Creator
{
	namespace Matrix
	{

	}

	namespace Quaternion
	{
		[[nodiscard]] NO_ODR BSMath::Quaternion FromRotator(const BSMath::Rotator& rot) noexcept
		{
			// Ref: https://github.com/bulletphysics/bullet3/blob/master/src/LinearMath/btQuaternion.h

			const float halfYaw = BSMath::Deg2Rad(rot.yaw * 0.5f);
			const float halfPitch = BSMath::Deg2Rad(rot.pitch * 0.5f);
			const float halfRoll = BSMath::Deg2Rad(rot.roll * 0.5f);

			const float cosYaw = BSMath::Cos(halfYaw);
			const float sinYaw = BSMath::Sin(halfYaw);

			const float cosPitch = BSMath::Cos(halfPitch);
			const float sinPitch = BSMath::Sin(halfPitch);

			const float cosRoll = BSMath::Cos(halfRoll);
			const float sinRoll = BSMath::Sin(halfRoll);

			return BSMath::Quaternion
			{
				sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw,
				cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw,
				cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw,
				cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw
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
	}
}