#pragma once

#include "Marker.hpp"
#include "ModeBase.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	/// The default mode that doesn't allow to do anything.
	/// @tparam letter - the letter that represents the mode.
	template <char letter>
	class ModeDefault : public ModeBase
	{
	public:
		void shape(Marker* marker);
	};

	// implementation //

	template <char letter>
	void ModeDefault<letter>::shape(Marker* marker)
	{
		Utilities::shapeTime(marker);

#ifdef SHOW_MODE
		marker->mark(letter);
#endif
	}
}