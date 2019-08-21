#include "ModeWordclock.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	void ModeWordclock::paint()
	{
		if (isInTransition()) {
			ModeBase::paint();
		}
		else {
			Painter::setColor(Wordclock::getCurrentPreset());
			Utilities::printTime();
		}
	}
}