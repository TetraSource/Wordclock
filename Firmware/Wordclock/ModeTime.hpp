#pragma once

#include "ModeBase.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	/// allows manual setting of the time.
	/// @tparam timeType - the time element that is manipulated by the mode.
	template <TimeTypes timeType>
	class ModeTime : public ModeBase
	{
	protected:
		typedef ModeBase super;
		uint8_t newTime;
		bool changed;
	public:
		void select();
		void deselect();
		void actionButton(const bool &inc);
		void paint();
	};

	template <TimeTypes timeType>
	void ModeTime<timeType>::select()
	{
		if (isInTransition())
			ModeBase::select();
		else {
			newTime = Wordclock::getTime(timeType);
			changed = false;
		}
	}

	template <TimeTypes timeType>
	void ModeTime<timeType>::deselect()
	{
		if (isInTransition())
			ModeBase::deselect();
		else if (changed) {
			// Time was changed. Save that change.
			Wordclock::setTime(timeType, newTime);
		}
	}

	template <TimeTypes timeType>
	void ModeTime<timeType>::actionButton(const bool &inc)
	{
		changed = true;
		newTime = Utilities::changeValue(newTime,
			Wordclock::getMaximumTime(timeType), inc);
		Wordclock::repaint();
	}

	template <TimeTypes timeType>
	void ModeTime<timeType>::paint()
	{
		if (isInTransition())
			ModeBase::paint();
		else {
			Painter::setColor(Wordclock::getCurrentPreset());
			Utilities::printNumber(
				newTime == 0 ? Wordclock::getMaximumTime(timeType) : newTime);
		}
	}
}