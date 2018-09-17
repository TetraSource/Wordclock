#pragma once

#include "GeneratorBase.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	enum GeneratorColorPresetModes
	{
		ChooseNext,     /// always choose the next color preset.
		ChoosePrevious, /// always choose the previous color preset.
		ChooseRandom,   /// always choose a random color preset
		ChooseByTime,   /// always choose the color preset in correspondence with the current time (e.g. 11:04 results in the forth preset if the time type is @c Minutes).
	};

	/// cycles throw the color presets and returns them.
	/// @tparam mode determines how the color presets are selected.
	/// @tparam fromPreset the lowest index of a color preset to choose from.
	/// @tparam toPreset the highest index of a color preset to choose from.
	/// @tparam timeType the unit of the time for the mode @c ChooseByTime.
	template <GeneratorColorPresetModes mode, uint8_t fromPreset = 0, uint8_t toPreset = COLOR_PRESET_COUNT - 1, TimeTypes timeType = Minutes>
	class GeneratorColorPreset : public GeneratorBase
	{
	protected:
		uint8_t currPreset;

	public:
		GeneratorColorPreset();

		CRGB nextColor();
	};

	// implementation //

	template <GeneratorColorPresetModes mode, uint8_t fromPreset, uint8_t toPreset, TimeTypes timeType>
	GeneratorColorPreset<mode, fromPreset, toPreset, timeType>::GeneratorColorPreset()
	{
		currPreset = 255;
	}

	template <GeneratorColorPresetModes mode, uint8_t fromPreset, uint8_t toPreset, TimeTypes timeType>
	CRGB GeneratorColorPreset<mode, fromPreset, toPreset, timeType>::nextColor()
	{
		if (currPreset == 255) {
			currPreset = Wordclock::getColorPresetIndex();
			if (currPreset < fromPreset)
				currPreset = fromPreset;
			else if (currPreset > toPreset)
				currPreset = toPreset;
		}

		switch (mode)
		{
		case ChooseNext:
			if (currPreset >= toPreset)
				currPreset = fromPreset;
			else
				currPreset++;
			break;
		case ChoosePrevious:
			if (currPreset <= fromPreset)
				currPreset = toPreset;
			else
				currPreset--;
			break;
		case ChooseRandom:
			currPreset = random(fromPreset, toPreset + 1);
			break;
		case ChooseByTime:
			currPreset = (Wordclock::getTime(timeType) + fromPreset) % toPreset;
		}

		return Wordclock::getColorPreset(currPreset);
	}
}