
#include "ModeColorPreset.hpp"

namespace Wordclock
{
	void ModeColorPresetBase::increment(const bool &inc)
	{
		Wordclock::setColorPresetIndex(
			Utilities::changeValue(
				Wordclock::getColorPresetIndex(), COLOR_PRESET_COUNT, inc));
		Wordclock::repaint();
	}
}