#pragma once

#include "SelectorBase.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	/// Selects a number out of a fixed range by mapping the current time
	/// onto it.
	/// @tparam timeType - the unit of the time that gets mapped.
	/// @tparam from - the lower border of the range.
	/// @tparam to - the index of the element behind upper border of the range.
	/// @tparam div - before mapping, the time is divided by this factor.
	/// Example: timeType = Minutes, from = 0, to = 12, div = 5
	///          => For the first five minutes of an hour 0 is returned,
	///             2 for the next five, ... and 11 for the last five.
	template <TimeTypes timeType, STYPE from = 0, STYPE to = 255, STYPE div = 1>
	class SelectorTime : public SelectorBase
	{
	protected:
		typedef SelectorBase super;
	public:
		SelectorTime();
		STYPE next();
	};

	template <TimeTypes timeType, STYPE from, STYPE to, STYPE div>
	SelectorTime<timeType, from, to, div>::SelectorTime()
	{}

	template <TimeTypes timeType, STYPE from, STYPE to, STYPE div>
	STYPE SelectorTime<timeType, from, to, div>::next()
	{
		return from + (Wordclock::getTime(timeType) / div) % (to - from);
	}
}