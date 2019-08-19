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
		typedef ModeBase super;
		static CHSV curr;
		static CRGB result;
		ModeHSVBase();
	public:
		void select();
	};

	/// allows to manipulate the HSV amounts of the current color preset
	/// manually.
	/// @tparam value - the element of the color that the mode changes.
	/// @tparam levelCount - the count of grades the user can change the
	/// color values by.
	template <HSVColorValues value, uint8_t levelCount>
	class ModeHSV : public ModeHSVBase
	{
	protected:
		typedef ModeHSVBase super;
	public:
		void actionButton(const bool &inc);
		void paint();
	};

	template <HSVColorValues value, uint8_t levelCount>
	void ModeHSV<value, levelCount>::actionButton(const bool &inc)
	{
		uint8_t* colorArray = (uint8_t*)&curr;
		colorArray[value] = Utilities::changeLevel(
			colorArray[value], levelCount, inc);

		hsv2rgb_rainbow(curr, result);
		Wordclock::setColorPreset(Wordclock::getColorPresetIndex(), result);
	}

	template <HSVColorValues value, uint8_t levelCount>
	void ModeHSV<value, levelCount>::paint()
	{
		if (isInTransition()) {
			ModeBase::paint();
		}
		else {
			uint8_t* colorArray = (uint8_t*)&curr;
			Utilities::printHex(colorArray[value]);
		}
	}
}