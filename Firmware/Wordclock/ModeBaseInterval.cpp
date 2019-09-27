#include "ModeBaseInterval.hpp"

namespace Wordclock
{
	ModeBaseIntervalBase::ModeBaseIntervalBase()
		: ModeBase()
	{}

	void ModeBaseIntervalBase::deselect()
	{
		if (isInTransition())
			ModeBase::deselect();
		else
			Wordclock::cancelTimer(this, 0);
	}
}