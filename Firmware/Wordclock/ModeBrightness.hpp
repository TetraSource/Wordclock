#pragma once

#include "ModeDefault.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	/// allows to set the brightness of all LEDs gradually.
	/// @tparam letter - the letter that represents the mode.
	/// @tparam levelCount - the count of grades of brightness the user
	///                      can choose from.
	template <char letter, uint8_t levelCount>
	class ModeBrightness : public ModeBase
	{
	public:
		void increment(const bool &inc);

		void paint();
	};

	// implementation //

	template <char letter, uint8_t levelCount>
	void ModeBrightness<letter, levelCount>::increment(const bool &inc)
	{
		Wordclock::setBrightness(Utilities::changeLevel(
			Wordclock::getBrightness(), levelCount, inc));
	}

	template <char letter, uint8_t levelCount>
	void ModeBrightness<letter, levelCount>::paint()
	{
		Utilities::printTime();

#ifdef SHOW_MODE
		Painter::paint(letter);
#endif
	}
}
