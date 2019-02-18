#pragma once

#include "ModeBase.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	class ModeRearrangeColorPresetBase : public ModeBase
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
		void paint();
	};

	// implementation //

	template <char letter>
	void ModeRearrangeColorPreset<letter>::paint()
	{
		Utilities::printNumber(Wordclock::getColorPresetIndex() + 1);

#ifdef SHOW_MODE
		Painter::paint(letter);
#endif
	}
}