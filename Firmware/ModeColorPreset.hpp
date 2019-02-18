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
	/// @tparam letter - the letter that represents the mode.
	template <char letter>
	class ModeColorPreset : public ModeColorPresetBase
	{
	public:
		void paint();
	};

	// implementation //

	template <char letter>
	void ModeColorPreset<letter>::paint()
	{
		Utilities::printNumber(Wordclock::getColorPresetIndex() + 1);

#ifdef SHOW_MODE
		Painter::paint(letter);
#endif
	}
}