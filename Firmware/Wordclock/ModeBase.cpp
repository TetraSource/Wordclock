
#include "ModeBase.hpp"

namespace Wordclock
{
	uint32_t ModeBase::updateTime = 0;
	uint32_t ModeBase::updateThreshold = 0;

	void ModeBase::startTimer(const uint32_t &time)
	{
		updateTime = time;
		updateThreshold = millis();
	}

	void ModeBase::cancelTimer()
	{
		updateTime = 0;
	}

	void ModeBase::select()
	{}

	void ModeBase::deselect()
	{}

	void ModeBase::increment(const bool &inc)
	{}

	void ModeBase::timer()
	{}
}
