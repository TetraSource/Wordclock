#pragma once

#include "Marker.hpp"
#include "ModeBase.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	class Marker;

	class ModeTimeBase : public ModeBase
	{
	protected:
		static uint8_t newTime;
		static bool changed;
	};


	/// allows manual setting of the time.
	/// @tparam letter - the letter that represents the mode.
	/// @tparam timeType - the time element that is manipulated by the mode.
	template <char letter, TimeTypes timeType>
	class ModeTime : public ModeTimeBase
	{
	public:
		void select();

		void deselect();

		void increment(const bool &inc);

		void shape(Marker* marker);
	};

	// implementation //

	template <char letter, TimeTypes timeType>
	void ModeTime<letter, timeType>::select()
	{
		newTime = Wordclock::getTime(timeType);
		changed = false;
	}

	template <char letter, TimeTypes timeType>
	void ModeTime<letter, timeType>::deselect()
	{
		if (changed) {
			Wordclock::setTime(timeType, newTime);
		}
	}

	template <char letter, TimeTypes timeType>
	void ModeTime<letter, timeType>::increment(const bool &inc)
	{
		changed = true;
		newTime = Utilities::changeValue(newTime, Wordclock::getMaximumTime(timeType), inc);
		Wordclock::reshape();
	}

	template <char letter, TimeTypes timeType>
	void ModeTime<letter, timeType>::shape(Marker* marker)
	{
		Utilities::shapeNumber(marker, newTime == 0 ? Wordclock::getMaximumTime(timeType) : newTime);

#ifdef SHOW_MODE
		marker->mark(letter);
#endif
	}
}