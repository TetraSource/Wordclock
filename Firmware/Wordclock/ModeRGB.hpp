#pragma once

#include "ModeBase.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	enum RGBColorValues
	{
		Red,
		Blue,
		Green,
	};

	/// allows to manipulate the RGB amounts of the current color preset manually.
	/// @tparam letter the position of the letter that represents the mode,
	///                calculated by the POINT macro.
	/// @tparam value - the element of the color that the mode changes.
	/// @tparam levelCount - the count of grades the user can change the color values by.
	template <uint8_t letter, RGBColorValues value, uint8_t levelCount>
	class ModeRGB : public ModeBase
	{
	public:
		void increment(const bool &inc);

		void paint();
	};

	template <uint8_t letter, RGBColorValues value, uint8_t levelCount>
	void ModeRGB<letter, value, levelCount>::increment(const bool &inc)
	{
		CRGB color = Wordclock::getColorPreset(
			Wordclock::getColorPresetIndex());
		color[value] = Utilities::changeLevel(color[value], levelCount, inc);
		Wordclock::setColorPreset(Wordclock::getColorPresetIndex(), color);
	}

	template <uint8_t letter, RGBColorValues value, uint8_t levelCount>
	void ModeRGB<letter, value, levelCount>::paint()
	{
		Utilities::printTime();

#ifdef SHOW_MODE
		Painter::paint(letter);
#endif
	}
}