
#include "ModeColorPreset.hpp"

namespace Wordclock
{
	void ModeColorPreset::actionButton(const bool &inc)
	{
		Wordclock::setColorPresetIndex(
			Utilities::changeValue(
				Wordclock::getColorPresetIndex(), COLOR_PRESET_COUNT, inc));
		Wordclock::repaint();
	}

	void ModeColorPreset::paint()
	{
		if (isInTransition())
			ModeBase::paint();
		else {
			Painter::setColor(Wordclock::getCurrentPreset());
			Utilities::printNumber(Wordclock::getColorPresetIndex() + 1);
		}
	}
}