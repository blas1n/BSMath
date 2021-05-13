#pragma once

#include <functional>
#include <type_traits>
#include "Basic.h"

namespace BSMath
{
	template <class T, size_t L>
	struct HashRange
	{
		[[nodiscard]] size_t operator()(const T& value) const noexcept
		{
			constexpr static auto HashOffsetBasis = sizeof(size_t) == 8 ? 14695981039346656037ull : 2166136261u;
			constexpr static auto HashPrime = sizeof(size_t) == 8 ? 1099511628211ull : 16777619u;

			size_t val = HashOffsetBasis;
			for (size_t i = 0; i < L; ++i)
			{
				val ^= reinterpret_cast<const unsigned char*>(&value)[i];
				val *= HashPrime;
			}
			return val;
		}
	};

	template <class T, class SFINAE = void>
	struct Hash final
	{
		[[nodiscard]] size_t operator()(const T& value) const noexcept
		{
			return HashRange<T, sizeof(T)>{}(value);
		}
	};

	template <class T>
	struct Hash<T, std::enable_if_t<std::is_constructible_v<std::hash<T>>>> final
	{
		[[nodiscard]] size_t operator()(const T& value) const noexcept
		{
			return std::hash<T>{}(value);
		}
	};
}
