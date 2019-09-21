#include "ModeBase.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	const uint8_t ModeBase::transitionChannel = 254;
	const uint16_t ModeBase::transitionTime = 750;

	bool ModeBase::inTransition = false;

	bool ModeBase::isInTransition()
	{
		return inTransition &&
			Wordclock::accessMode(Wordclock::getMode(0)) == this;
	}

	ModeBase::ModeBase()
	{}

	void ModeBase::select()
	{}

	void ModeBase::deselect()
	{
#ifdef SHOW_MODE
		if (isInTransition()) {
			Wordclock::cancelTimer(this, transitionChannel);
		}
#endif
	}

	void ModeBase::actionButton(const bool &inc)
	{}

	void ModeBase::modeButton(const bool &inc)
	{
		Wordclock::setMode(0,
			Wordclock::getNextMode(0, inc, 0, SELECTABLE_MODES), true);
	}

	uint32_t ModeBase::timer(const uint8_t &channel)
	{
#ifdef SHOW_MODE
		if (isInTransition() && channel == transitionChannel) {
			inTransition = false;
			Wordclock::repaint();
			select();
		}
#endif
		return 0;
	}

	void ModeBase::paint()
	{
#ifdef SHOW_MODE
		if (isInTransition()) {
			Painter::setColor(CRGB(0, 0, 0));
			Painter::paintAll();
			Painter::setColor(Wordclock::getCurrentPreset());
			Painter::paint(0, Painter::height-1, Painter::width);
			Utilities::printNumber(Wordclock::getMode(0) + 1, 1);
		}
#endif
	}
}
