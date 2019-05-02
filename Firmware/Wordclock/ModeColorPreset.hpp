#pragma once

#include "ModeBase.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	class ModeColorPresetBase : public ModeBase
	{
	public:
		void increment(const bool &inc);
	};

	/// allows manual choice of current color preset.
	/// @tparam letter the position of the letter that represents the mode,
	///                calculated by the POINT macro.
	template <uint8_t letter>
	class ModeColorPreset : public ModeColorPresetBase
	{
	public:
		void paint();
	};

	// implementation //

	template <uint8_t letter>
	void ModeColorPreset<letter>::paint()
	{
		Utilities::printNumber(Wordclock::getColorPresetIndex() + 1);

#ifdef SHOW_MODE
		Painter::paint(letter);
#endif
	}
}