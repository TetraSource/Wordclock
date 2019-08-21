#include "ModeAlarm.hpp"
#include "Utilities.hpp"

namespace Wordclock
{
	ModeAlarmBase::ModeAlarmBase(const uint8_t &initMode)
		: ModeBase()
	{
		currAspect = 3;
		triggered = false;
		alarm = EepromVariable<Alarm>();
		Alarm default_ = { initMode, 0, 0, false };
		alarm.setDefault(default_);
		if (alarm.get().active)
			Wordclock::startTimer(this, 500, 0);
	}

	uint8_t ModeAlarmBase::incAspect(uint8_t val, const uint8_t &max,
		const uint8_t &increment, const bool &inc)
	{
		val += max;
		if (inc)
			val += increment;
		else
			val -= increment;
		return val % max;
	}

	void ModeAlarmBase::internalActionButton(const bool &inc,
		const int8_t &increment, const uint8_t &minMode, const uint8_t &maxMode)
	{
		Wordclock::repaint();
		Alarm &a = alarm.get();
		if (a.active && !(inc && currAspect == 3))
			Wordclock::cancelTimer(this, 0);

		switch (currAspect) {
		case 0:
			a.hour = incAspect(a.hour,
				Wordclock::getMaximumTime(Hours), 1, inc);
			break;
		case 1:
			a.minute = incAspect(a.minute,
				Wordclock::getMaximumTime(Minutes), increment, inc);
			break;
		case 2:
			a.mode = incAspect(a.mode - minMode,
				(maxMode > MODE_COUNT ? MODE_COUNT : maxMode) - minMode,
				1, inc) + minMode;
			break;
		case 3:
			if (inc) {
				currAspect = 0;
				return;
			}
			else {
				a.active = !a.active;
			}
		}
		if (a.active)
			Wordclock::startTimer(this, 500, 0);
		alarm.set(a);
	}

	void ModeAlarmBase::modeButton(const bool &inc)
	{
		if (currAspect == 3) {
			super::modeButton(inc);
			return;
		}
		if (inc)
			currAspect = (currAspect+1) % 3;
		else
			currAspect = 3;
		Wordclock::repaint();
	}

	uint32_t ModeAlarmBase::timer(const uint8_t &channel)
	{
		if (channel != 0)
			return ModeBase::timer(channel);
		Alarm &a = alarm.get();
		if (a.active && Wordclock::getTime(Hours) == a.hour &&
			Wordclock::getTime(Minutes) == a.minute)
		{
			this->trigger();
			return 86350000;
		}
		return 500;
	}

	void ModeAlarmBase::paint()
	{
		if (isInTransition()) {
			ModeBase::paint();
			return;
		}
		Alarm &a = alarm.get();
		Painter::setColor(Wordclock::getCurrentPreset());
		switch (currAspect) {
		case 0:
			Painter::paint(0, 0, a.hour >= 12);
			Utilities::printTime(a.hour, a.minute);
			break;
		case 1:
			Painter::paint(0, 0, a.minute % 5);
			Utilities::printTime(a.hour, a.minute);
			break;
		case 2:
			Utilities::printNumber(a.mode + 1);
			break;
		case 3:
			Utilities::printAnswere(a.active);
		}
	}

	void ModeAlarmBase::trigger()
	{}
}