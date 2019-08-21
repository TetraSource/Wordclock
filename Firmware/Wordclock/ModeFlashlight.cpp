#include "ModeFlashlight.hpp"

namespace Wordclock
{
	ModeFlashlightBase::ModeFlashlightBase()
		: ModeBase()
	{}

	void ModeFlashlightBase::deselect()
	{
		if (isInTransition()) {
			ModeBase::deselect();
			return;
		}
		Wordclock::cancelTimer(this, 0);
	}

	void ModeFlashlightBase::paint()
	{
		if (isInTransition()) {
			ModeBase::paint();
			return;
		}
		if (visible) {
			Painter::setColor(currColor);
			Painter::paintAll();
		}
	}
}