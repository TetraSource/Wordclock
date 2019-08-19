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
	/// @tparam value - the element of the color that the mode changes.
	/// @tparam levelCount - the count of grades the user can change the color values by.
	template <RGBColorValues value, uint8_t levelCount>
	class ModeRGB : public ModeBase
	{
	protected:
		typedef ModeBase super;
	public:
		void actionButton(const bool &inc);
		void paint();
	};

	template <RGBColorValues value, uint8_t levelCount>
	void ModeRGB<value, levelCount>::actionButton(const bool &inc)
	{
		CRGB color = Wordclock::getColorPreset(
			Wordclock::getColorPresetIndex());
		color[value] = Utilities::changeLevel(color[value], levelCount, inc);
		Wordclock::setColorPreset(Wordclock::getColorPresetIndex(), color);
		Wordclock::repaint();
	}

	template <RGBColorValues value, uint8_t levelCount>
	void ModeRGB<value, levelCount>::paint()
	{
		if (isInTransition()) {
			ModeBase::paint();
		}
		else {
			Utilities::printHex(Wordclock::getColorPreset(
				Wordclock::getColorPresetIndex())[value]);
		}
	}
}