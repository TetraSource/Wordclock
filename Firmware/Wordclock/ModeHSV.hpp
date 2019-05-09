#pragma once

#include <FastLED.h>
#include "ModeBase.hpp"
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

	class ModeHSVBase : public ModeBase
	{
	protected:
		static CHSV curr;
		static CRGB result;

	public:
		void select();
	};

	/// allows to manipulate the HSV amounts of the current color preset
	/// manually.
	/// @tparam letter the position of the letter that represents the mode,
	///                calculated by the POINT macro.
	/// @tparam value - the element of the color that the mode changes.
	/// @tparam levelCount - the count of grades the user can change the
	/// color values by.
	template <uint8_t letter, HSVColorValues value, uint8_t levelCount>
	class ModeHSV : public ModeHSVBase
	{
	public:
		void increment(const bool &inc);

		void paint();
	};

	// implementation //

	template <uint8_t letter, HSVColorValues value, uint8_t levelCount>
	void ModeHSV<letter, value, levelCount>::increment(const bool &inc)
	{
		uint8_t* colorArray = (uint8_t*)&curr;
		colorArray[value] = Utilities::changeLevel(
			colorArray[value], levelCount, inc);

		hsv2rgb_rainbow(curr, result);
		Wordclock::setColorPreset(Wordclock::getColorPresetIndex(), result);
	}

	template <uint8_t letter, HSVColorValues value, uint8_t levelCount>
	void ModeHSV<letter, value, levelCount>::paint()
	{
		uint8_t* colorArray = (uint8_t*)&curr;
		Utilities::printHex(colorArray[value]);

#ifdef SHOW_MODE
		Painter::paint(letter);
#endif
	}
}