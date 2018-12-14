#pragma once

#include "Marker.hpp"
#include "ModeColorBase.hpp"
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
	class ModeRGB : public ModeColorBase
	{
	public:
		void increment(const bool &inc);

		void shape(Marker* marker);
	};

	template <char letter, RGBColorValues value, uint8_t levelCount>
	void ModeRGB<letter, value, levelCount>::increment(const bool &inc)
	{
		CRGB color = Wordclock::getColorPreset(Wordclock::getColorPresetIndex());
		color[value] = Utilities::changeLevel(color[value], levelCount, inc);
		Wordclock::setColorPreset(Wordclock::getColorPresetIndex(), color);
	}

	template <char letter, RGBColorValues value, uint8_t levelCount>
	void ModeRGB<letter, value, levelCount>::shape(Marker* marker)
	{
		Utilities::shapeTime(marker);

#ifdef SHOW_MODE
		marker->mark(letter);
#endif
	}
}