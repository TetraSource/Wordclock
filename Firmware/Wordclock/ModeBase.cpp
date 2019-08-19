#include "ModeBase.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	const uint8_t ModeBase::transitionChannel = 254;
	const uint16_t ModeBase::transitionTime = 750;

	ModeBase::ModeBase()
		: state(0)
	{}

	void ModeBase::select()
	{}

	void ModeBase::deselect()
	{
#ifdef SHOW_MODE
		if (isInTransition()) {
			state &= 0xfe;
			Wordclock::cancelTimer(this, transitionChannel);
		}
#endif
	}

	void ModeBase::actionButton(const bool &inc)
	{}

	void ModeBase::modeButton(const bool &inc)
	{
		uint8_t newMode = Utilities::changeValue(
			Wordclock::getMode(), Wordclock::modeCount, inc);
#ifdef SHOW_MODE
		if (newMode != Wordclock::getMode()) {
			Wordclock::accessMode(newMode)->state |= 0x01;
			Wordclock::startTimer(Wordclock::accessMode(newMode),
				transitionTime, transitionChannel);
			Wordclock::setMode(newMode);
		}
#else
		Wordclock::setMode(newMode);
#endif
	}

	uint32_t ModeBase::timer(const uint8_t &channel)
	{
#ifdef SHOW_MODE
		if (isInTransition() && channel == transitionChannel) {
			state &= 0xfe;
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
			Painter::setColor(CRGB(255, 0, 0));
			Utilities::printNumber(Wordclock::getMode() + 1);
		}
#endif
	}
}
