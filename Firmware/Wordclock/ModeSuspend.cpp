#include "ModeSuspend.hpp"

namespace Wordclock
{
	ModeSuspendedBase::ModeSuspendedBase()
		: ModeBase()
	{
		suspended = EepromVariable<bool>();
		suspended.setDefault(false);
	}

	uint32_t ModeSuspendedBase::timer(const uint8_t &channel)
	{
		if (channel != 0)
			return ModeBase::timer(channel);
		suspended.set(!suspended.get());
		Wordclock::repaint();
		return 0;
	}

	void ModeSuspendedBase::paint()
	{
		if (isInTransition()) {
			ModeBase::paint();
		}
		else if (suspended.get()) {
			Painter::setColor(CRGB(0, 0, 0));
			Painter::paintAll();
		}
	}
}