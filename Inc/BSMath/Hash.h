#pragma once

#include <functional>
#include <type_traits>
#include "Basic.h"

namespace BSMath
{
	namespace Detail
	{
		template <class T>
		struct HasHash final
		{
		private:
			template <typename C>
			static std::true_type Test(decltype(&(std::hash<C>::operator())));

			template <typename C>
			static std::false_type Test(...);

		public:
			constexpr static bool value = decltype(Test<T>(0))::value;
		};

		template <class T, bool>
		struct HashImpl;

		template <class T>
		struct HashImpl<T, true> : public std::hash<T> {};

		template <class T>
		struct HashImpl<T, false>
		{
			[[nodiscard]] size_t operator()(const T& value) const noexcept
			{
				constexpr static auto HashOffsetBasis = sizeof(size_t) == 8 ? 14695981039346656037ull : 2166136261u;
				constexpr static auto HashPrime = sizeof(size_t) == 8 ? 1099511628211ull : 16777619u;

				size_t val = HashOffsetBasis;
				for (size_t i = 0; i < sizeof(T); ++i)
				{
					val ^= reinterpret_cast<const unsigned char*>(&value)[i];
					val *= HashPrime;
				}
				return val;
			}
		};
	}

	template <class T>
	struct Hash final : public Detail::HashImpl<T, Detail::HasHash<T>::value> {};
}