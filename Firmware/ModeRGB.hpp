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
	/// @tparam letter - the letter that represents the mode.
	/// @tparam value - the element of the color that the mode changes.
	/// @tparam levelCount - the count of grades the user can change the color values by.
	template <char letter, RGBColorValues value, uint8_t levelCount>
	class ModeRGB : public ModeBase
	{
	public:
		void increment(const bool &inc);

		void paint();
	};

	template <char letter, RGBColorValues value, uint8_t levelCount>
	void ModeRGB<letter, value, levelCount>::increment(const bool &inc)
	{
		CRGB color = Wordclock::getColorPreset(
			Wordclock::getColorPresetIndex());
		color[value] = Utilities::changeLevel(color[value], levelCount, inc);
		Wordclock::setColorPreset(Wordclock::getColorPresetIndex(), color);
	}

	template <char letter, RGBColorValues value, uint8_t levelCount>
	void ModeRGB<letter, value, levelCount>::paint()
	{
		Utilities::printTime();

#ifdef SHOW_MODE
		Painter::paint(letter);
#endif
	}
}