#include "ModeAlarm.hpp"

namespace Wordclock
{
	ModeAlarmBase::ModeAlarmBase()
		: ModeBase()
	{
		currAspect = 0;
		triggered = false;
		mode = EepromVariable<uint8_t>();
		alarm = EepromVariable<Alarm>();
		Alarm default_ = { 0, 0, false };
		alarm.setDefault(default_);
		if (alarm.get().active)
			Wordclock::startTimer(this, 500, 1);
	}

	uint32_t ModeAlarmBase::getAlarmTimerTime()
	{
		Alarm &a = alarm.get();
		uint8_t hours = (24 + a.hour - Wordclock::getTime(Hours)) % 24;
		uint8_t minutes = (60 + a.minute - Wordclock::getTime(Minutes)) % 60;
		if (Wordclock::getTime(Minutes) + minutes >= 60) {
			hours--;
		}
		if (minutes > 0)
			minutes--;
		else {
			minutes = 59;
			hours--;
		}
		return (hours * 60 + minutes) * 60000;
	}

	uint8_t ModeAlarmBase::incVal(uint8_t val, const uint8_t &max,
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
		const int8_t &increment)
	{
		Wordclock::repaint();
		Alarm &a = alarm.get();
		if (a.active && !(inc && currAspect == 0))
			Wordclock::cancelTimer(this, 0);

		switch (currAspect) {
		case 0:
			if (inc) {
				currAspect = 1;
				return;
			}
			else {
				a.active = !a.active;
			}
			break;
		case 1:
			a.hour = incVal(a.hour,
				Wordclock::getMaximumTime(Hours), 1, inc);
			break;
		case 2:
			a.minute = incVal(a.minute,
				Wordclock::getMaximumTime(Minutes), increment, inc);
		}
		alarm.set(a);
		if (a.active)
			Wordclock::startTimer(this, getAlarmTimerTime(), 0);
	}

	void ModeAlarmBase::internalModeButton(const bool &inc, const uint8_t &max)
	{
		if (currAspect == 0) {
			super::modeButton(inc);
			return;
		}
		if (inc)
			currAspect = currAspect % max + 1;
		else
			currAspect = 0;
		Wordclock::repaint();
	}

	void ModeAlarmBase::modeButton(const bool &inc)
	{
		internalModeButton(inc, 2);
	}

	uint32_t ModeAlarmBase::internalTimer(const uint8_t &channel)
	{
		if (channel != 0)
			return channel == 1 ? getAlarmTimerTime() : ModeBase::timer(channel);
		Alarm &a = alarm.get();
		return (Wordclock::getTime(Hours) == a.hour &&
			Wordclock::getTime(Minutes) == a.minute) ? 0xffffffff : 500;
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
			Utilities::printAnswere(a.active);
			break;
		case 1:
			Painter::paint(0, 0, a.hour >= 12);
			Utilities::printTime(a.hour, a.minute);
			break;
		case 2:
			Painter::paint(0, 0, a.minute % 5);
			Utilities::printTime(a.hour, a.minute);
		}
	}
}