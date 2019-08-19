#include <FastLED.h>
#include "ModeRearrangeColorPreset.hpp"

namespace Wordclock
{
	void ModeRearrangeColorPreset::actionButton(const bool &inc)
	{
		uint8_t oldIndex = Wordclock::getColorPresetIndex();
		uint8_t newIndex =
			Utilities::changeValue(oldIndex, COLOR_PRESET_COUNT, inc);

		// swap colors
		CRGB temp = Wordclock::getColorPreset(oldIndex);
		Wordclock::setColorPreset(oldIndex,
			Wordclock::getColorPreset(newIndex));
		Wordclock::setColorPreset(newIndex, temp);

		Wordclock::setColorPresetIndex(newIndex);
	}

	void ModeRearrangeColorPreset::paint()
	{
		if (isInTransition())
			ModeBase::paint();
		else
			Utilities::printNumber(Wordclock::getColorPresetIndex() + 1);
	}
}