
#include "ModeHSV.hpp"

namespace Wordclock
{
	CHSV ModeHSVBase::curr = { 0, 0, 0 };
	CRGB ModeHSVBase::result = { 0, 0, 0 };

	void ModeHSVBase::select()
	{
		if (isInTransition()) {
			ModeBase::select();
			return;
		}
		if (!Painter::areIdentical(result,
			Wordclock::getColorPreset(Wordclock::getColorPresetIndex())))
		{
			curr = rgb2hsv_approximate(
				Wordclock::getColorPreset(Wordclock::getColorPresetIndex()));
		}
	}
}