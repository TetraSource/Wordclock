#pragma once

#include "ModeColorBase.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	class ModeColorPresetBase : public ModeColorBase
	{
	public:
		void increment(const bool &inc);
	};

	/// allows manual choice of current color preset.
	/// @tparam letter - the letter that represents the mode.
	template <char letter>
	class ModeColorPreset : public ModeColorPresetBase
	{
	public:
		void shape(Marker* marker);
	};

	// implementation //

	template <char letter>
	void ModeColorPreset<letter>::shape(Marker* marker)
	{
		Utilities::shapeNumber(marker, Wordclock::getColorPresetIndex() + 1);

#ifdef SHOW_MODE
		marker->mark(letter);
#endif
	}
}