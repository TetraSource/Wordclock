#pragma once

#include "Marker.hpp"
#include "ModeDefault.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	/// allows to set the brightness of all LEDs gradually.
	/// @tparam letter - the letter that represents the mode.
	/// @tparam levelCount - the count of grades of brightness the user can choose from.
	template <char letter, uint8_t levelCount>
	class ModeBrightness : public ModeBase
	{
	public:
		void increment(const bool &inc);

		void shape(Marker* marker);
	};

	// implementation //

	template <char letter, uint8_t levelCount>
	void ModeBrightness<letter, levelCount>::increment(const bool &inc)
	{
		Wordclock::setBrightness(Utilities::changeLevel(Wordclock::getBrightness(), levelCount, inc));
	}

	template <char letter, uint8_t levelCount>
	void ModeBrightness<letter, levelCount>::shape(Marker* marker)
	{
		Utilities::shapeTime(marker);

#ifdef SHOW_MODE
		marker->mark(letter);
#endif
	}
}