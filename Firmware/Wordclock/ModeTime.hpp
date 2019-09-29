#pragma once

#include "ModeBase.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	/// Allows to set the time. First it displays the current time element
	/// (Seconds, Minutes, ...) that is selected for this mode. When you do
	/// no changes, no changes are written to the RTC. And when you do any
	/// changes they are written when you leave the mode. A change means
	/// pressing a action button by the way, so even a sequence of button
	/// presses that resulted in the same value as before, are considered
	/// changes.
	/// When setting seconds it might be helpful to select a time ahead of time
	/// and simply select another mode the instant it becomes the current time.
	/// @tparam timeType - the unit of the time element that can be manipulated.
	template <TimeTypes timeType>
	class ModeTime : public ModeBase
	{
	protected:
		typedef ModeBase super;
		uint8_t newTime;
		bool changed;

	public:
		void select()
		{
			if (isInTransition())
				ModeBase::select();
			else {
				newTime = Wordclock::getTime(timeType);
				changed = false;
			}
		}

		void deselect()
		{
			if (isInTransition())
				ModeBase::deselect();
			else if (changed) {
				// Time was changed. Save that change.
				Wordclock::setTime(timeType, newTime);
			}
		}

		void actionButton(const bool &inc)
		{
			changed = true;
			newTime = Utilities::changeValue(newTime,
				Wordclock::getMaximumTime(timeType), inc);
			Wordclock::repaint();
		}

		void paint()
		{
			if (isInTransition())
				ModeBase::paint();
			else {
				Painter::setColor(Wordclock::getCurrentPreset());
				Utilities::printNumber(newTime == 0 ?
					Wordclock::getMaximumTime(timeType) : newTime);
			}
		}
	};
}