#pragma once

#include "ModeBaseInterval.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	/// allows manual setting of the time.
	/// @tparam timeType - the time element that is manipulated by the mode.
	template <TimeTypes timeType>
	class ModeTime : public ModeBaseInterval<1000>
	{
	protected:
		typedef ModeBaseInterval<1000> super;
		uint8_t newTime;
	public:
		void select();
		void deselect();
		void actionButton(const bool &inc);
		void paint();
	};

	template <TimeTypes timeType>
	void ModeTime<timeType>::select()
	{
		if (isInTransition()) {
			ModeBase::select();
		}
		else {
			newTime = Wordclock::getTime(timeType);
			setState(1, false);
		}
	}

	template <TimeTypes timeType>
	void ModeTime<timeType>::deselect()
	{
		if (isInTransition()) {
			ModeBase::deselect();
		}
		else if (getState(1)) {
			// Time was chaned. Save that changes.
			Wordclock::setTime(timeType, newTime);
		}
	}

	template <TimeTypes timeType>
	void ModeTime<timeType>::actionButton(const bool &inc)
	{
		setState(1, true);
		newTime = Utilities::changeValue(newTime,
			Wordclock::getMaximumTime(timeType), inc);
		Wordclock::repaint();
	}

	template <TimeTypes timeType>
	void ModeTime<timeType>::paint()
	{
		if (isInTransition())
			ModeBase::paint();
		else
			Utilities::printNumber(
				newTime == 0 ? Wordclock::getMaximumTime(timeType) : newTime);
	}
}