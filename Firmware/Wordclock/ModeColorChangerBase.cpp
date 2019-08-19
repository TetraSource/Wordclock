#include "ModeColorChangerBase.hpp"
#include "Painter.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	ModeColorChangerBase::ModeColorChangerBase()
		: ModeBase()
	{
		currColor = CRGB(0, 0, 0);
	}

	void ModeColorChangerBase::select()
	{
		if (isInTransition()) {
			ModeBase::select();
		}
		else {
			uint32_t time = timer(0);
			if (time != 0) {
				Wordclock::startTimer(this, time, 0);
			}
		}
	}

	void ModeColorChangerBase::deselect()
	{
		if (isInTransition()) {
			ModeBase::deselect();
		}
		else {
			Wordclock::cancelTimer(this, 0);
		}
	}

	void ModeColorChangerBase::paint()
	{
		if (isInTransition()) {
			ModeBase::paint();
		}
		else {
			Painter::setColor(currColor);
			Utilities::printTime();
		}
	}
}