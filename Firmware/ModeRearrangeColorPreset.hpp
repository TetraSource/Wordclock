#pragma once

#include "ModeColorBase.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	class ModeRearrangeColorPresetBase : public ModeColorBase
	{
	public:
		void increment(const bool &inc);
	};

	/// allows manual changing of the order of the current color preset.
	/// @tparam letter - the letter that represents the mode.
	template <char letter>
	class ModeRearrangeColorPreset : public ModeRearrangeColorPresetBase
	{
	public:
		void shape(Marker* marker);
	};

	// implementation //

	template <char letter>
	void ModeRearrangeColorPreset<letter>::shape(Marker* marker)
	{
		Utilities::shapeNumber(marker, Wordclock::getColorPresetIndex() + 1);

#ifdef SHOW_MODE
		marker->mark(letter);
#endif
	}
}