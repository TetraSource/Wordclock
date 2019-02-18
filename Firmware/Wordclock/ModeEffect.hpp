#pragma once

#include "Marker.hpp"
#include "ModeBase.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	class ModeEffectBase : public ModeBase
	{
	public:
		void increment(const bool &inc);
	};

	/// allows manual choice of the current effect.
	/// @tparam letter - the letter that represents the mode.
	template <char letter>
	class ModeEffect : public ModeEffectBase
	{
	public:
		void shape(Marker* marker);
	};

	// implementation //

	template <char letter>
	void ModeEffect<letter>::shape(Marker* marker)
	{
		Utilities::shapeNumber(marker, Wordclock::getEffectIndex() + 1);

#ifdef SHOW_MODE
		marker->mark(letter);
#endif
	}
}