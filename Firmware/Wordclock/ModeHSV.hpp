#pragma once

#include <FastLED.h>
#include "Marker.hpp"
#include "ModeColorBase.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	enum HSVColorValues
	{
		Hue,
		Saturation,
		Value,
	};

	class ModeHSVBase : public ModeColorBase
	{
	protected:
		static CHSV curr;
		static CRGB result;

	public:
		void select();
	};

	/// allows to manipulate the HSV amounts of the current color preset manually.
	/// @tparam letter - the letter that represents the mode.
	/// @tparam value - the element of the color that the mode changes.
	/// @tparam levelCount - the count of grades the user can change the color values by.
	template <char letter, HSVColorValues value, uint8_t levelCount>
	class ModeHSV : public ModeHSVBase
	{
	public:
		void increment(const bool &inc);

		void shape(Marker* marker);
	};

	// implementation //

	template <char letter, HSVColorValues value, uint8_t levelCount>
	void ModeHSV<letter, value, levelCount>::increment(const bool &inc)
	{
		uint8_t* colorArray = (uint8_t*)&curr;
		colorArray[value] = Utilities::changeLevel(colorArray[value], levelCount, inc);
		/*
		if (val == Hue)
		curr.h = Utilities::changeLevel(curr.h, HSV_LEVEL_COUNT, inc);
		//color.h = inc ? color.h + 256 / HSV_LEVEL_COUNT : color.h - 256 / HSV_LEVEL_COUNT;
		else if (val == Saturation)
		curr.s = Utilities::changeLevel(curr.s, HSV_LEVEL_COUNT, inc);
		//color.s = inc ? color.s + 256 / HSV_LEVEL_COUNT : color.s - 256 / HSV_LEVEL_COUNT;
		else if (val == Value)
		curr.v = Utilities::changeLevel(curr.v, HSV_LEVEL_COUNT, inc);
		//color.v = inc ? color.v + 256 / HSV_LEVEL_COUNT : color.v - 256 / HSV_LEVEL_COUNT;
		*/
		hsv2rgb_rainbow(curr, result);
		Wordclock::setColorPreset(Wordclock::getColorPresetIndex(), result);
	}

	template <char letter, HSVColorValues value, uint8_t levelCount>
	void ModeHSV<letter, value, levelCount>::shape(Marker* marker)
	{
		Utilities::shapeTime(marker);

#ifdef SHOW_MODE
		marker->mark(letter);
#endif
	}
}