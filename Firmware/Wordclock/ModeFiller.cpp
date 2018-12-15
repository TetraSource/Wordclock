
#include "ModeFiller.hpp"
#include "Utilities.hpp"

namespace Wordclock
{
	void ModeFillerBase::select()
	{
		timer();
	}

	void ModeFillerBase::timer()
	{
		Wordclock::repaint();
		startTimer(1000);
	}
}