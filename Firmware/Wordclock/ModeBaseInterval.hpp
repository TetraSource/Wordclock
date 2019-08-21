#pragma once

#include "ModeBase.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	// Ensures that the repaint is called at least every interval milliseconds.
	template<uint32_t interval>
	class ModeBaseInterval : public ModeBase
	{
	protected:
		typedef ModeBase super;
		ModeBaseInterval();

	public:
		void select();
		void deselect();
		uint32_t timer(const uint8_t&);
	};

	template<uint32_t interval>
	ModeBaseInterval<interval>::ModeBaseInterval()
		: ModeBase()
	{}

	template<uint32_t interval>
	void ModeBaseInterval<interval>::select()
	{
		if (isInTransition()) {
			ModeBase::select();
		}
		else {
			Wordclock::repaint();
			Wordclock::startTimer(this, interval, 0);
		}
	}

	template<uint32_t interval>
	void ModeBaseInterval<interval>::deselect()
	{
		if (isInTransition())
			ModeBase::deselect();
		else
			Wordclock::cancelTimer(this, 0);
	}

	template<uint32_t interval>
	uint32_t ModeBaseInterval<interval>::timer(const uint8_t &channel)
	{
		if (channel != 0)
			return ModeBase::timer(channel);
		Wordclock::repaint();
		return interval;
	}

	typedef ModeBaseInterval<500> ModeTimeBound;
}