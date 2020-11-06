#pragma once

#include "Utility.h"

namespace BSMath
{
	struct alignas(4) Color final
	{
	public:
		static const Color Black;
		static const Color Blue;
		static const Color Transparent;
		static const Color Cyan;
		static const Color Gray;
		static const Color Green;
		static const Color Magenta;
		static const Color Red;
		static const Color White;
		static const Color Yellow;

	public:
		constexpr Color() noexcept : a(255), r(255), g(255), b(255) {}
		
		constexpr Color(const Color&) noexcept = default;
		constexpr Color(Color&&) noexcept = default;

		constexpr Color& operator=(const Color&) noexcept = default;
		constexpr Color& operator=(Color&&) noexcept = default;

		~Color() = default;

		explicit constexpr Color(uint8 inR, uint8 inG, uint8 inB, uint8 inA = 255) noexcept
			: a(inA), r(inR), g(inG), b(inB) {}

		explicit Color(uint32 color) noexcept
			: Color()
		{
			*((uint32*)this) = color;
		}

		[[nodiscard]] uint32& DWColor() noexcept
		{
			return *(reinterpret_cast<uint32*>(this));
		}

		[[nodiscard]] const uint32& DWColor() const noexcept
		{
			return *(reinterpret_cast<const uint32*>(this));
		}

		constexpr Color& operator+=(const Color& other) noexcept;
		constexpr Color& operator-=(const Color& other) noexcept;

	public:
		uint8 a;
		uint8 r;
		uint8 g;
		uint8 b;
	};

	// Constatnts

	inline const Color Color::      Black{   0,   0,   0 };
	inline const Color Color::       Blue{   0,   0, 255 };
	inline const Color Color::Transparent{   0,   0,   0, 0 };
	inline const Color Color::       Cyan{   0, 255, 255 };
	inline const Color Color::       Gray{   0, 127, 127 };
	inline const Color Color::      Green{   0, 255,   0 };
	inline const Color Color::    Magenta{ 255,   0, 255 };
	inline const Color Color::        Red{ 255,   0, 255 };
	inline const Color Color::      White{ 255, 255, 255 };
	inline const Color Color::     Yellow{ 255, 255,   0 };

	// Global Operators

	[[nodiscard]] constexpr bool operator==(const Color& lhs, const Color& rhs) noexcept
	{
		return (lhs.r == rhs.r) && (lhs.g == rhs.g) && (lhs.b == rhs.b) && (lhs.a == rhs.a);
	}

	[[nodiscard]] constexpr bool operator!=(const Color& lhs, const Color& rhs) noexcept { return !(lhs == rhs); }

	[[nodiscard]] constexpr Color operator+(const Color& lhs, const Color& rhs) noexcept
	{
		return Color{ lhs } += rhs;
	}

	[[nodiscard]] constexpr Color operator-(const Color& lhs, const Color& rhs) noexcept
	{
		return Color{ lhs } -= rhs;
	}

	// Member Operators

	constexpr Color& Color::operator+=(const Color& other) noexcept
	{
		r = static_cast<uint8>(Min(static_cast<int32>(r) + static_cast<int32>(other.r), 255));
		g = static_cast<uint8>(Min(static_cast<int32>(g) + static_cast<int32>(other.g), 255));
		b = static_cast<uint8>(Min(static_cast<int32>(b) + static_cast<int32>(other.b), 255));
		a = static_cast<uint8>(Min(static_cast<int32>(a) + static_cast<int32>(other.a), 255));
		return *this;
	}

	constexpr Color& Color::operator-=(const Color& other) noexcept
	{
		r = static_cast<uint8>(Max(static_cast<int32>(r) - static_cast<int32>(other.r), 0));
		g = static_cast<uint8>(Max(static_cast<int32>(g) - static_cast<int32>(other.g), 0));
		b = static_cast<uint8>(Max(static_cast<int32>(b) - static_cast<int32>(other.b), 0));
		a = static_cast<uint8>(Max(static_cast<int32>(a) - static_cast<int32>(other.a), 0));
		return *this;
	}

	// Global Functions

	[[nodiscard]] NO_ODR Color Min(const Color& lhs, const Color& rhs) noexcept
	{
		return Color
		{
			Min(lhs.r, rhs.r), Min(lhs.g, rhs.g), Min(lhs.b, rhs.b), Min(lhs.a, rhs.a)
		};
	}

	[[nodiscard]] NO_ODR Color Max(const Color& lhs, const Color& rhs) noexcept
	{
		return Color
		{
			Max(lhs.r, rhs.r), Max(lhs.g, rhs.g), Max(lhs.b, rhs.b), Max(lhs.a, rhs.a)
		};
	}
}