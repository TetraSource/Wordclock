#pragma once

#include "ModeBase.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	class ModeBaseIntervalBase : public ModeBase
	{
	protected:
		typedef ModeBase super;
		ModeBaseIntervalBase();

	public:
		void deselect();
	};

	// Ensures that the repaint is called at least every interval milliseconds.
	template<uint32_t interval>
	class ModeBaseInterval : public ModeBaseIntervalBase
	{
	protected:
		typedef ModeBaseIntervalBase super;
		ModeBaseInterval()
			: ModeBaseIntervalBase()
		{}

	public:
		void select()
		{
			if (isInTransition()) {
				ModeBase::select();
			}
			else {
				Wordclock::repaint();
				Wordclock::startTimer(this, interval, 0);
			}
		}

		uint32_t timer(const uint8_t &channel)
		{
			if (channel != 0)
				return ModeBase::timer(channel);
			Wordclock::repaint();
			return interval;
		}
	};

	typedef ModeBaseInterval<500> ModeTimeBound;
}