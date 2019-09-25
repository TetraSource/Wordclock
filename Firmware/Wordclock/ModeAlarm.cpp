#include "ModeAlarm.hpp"

namespace Wordclock
{
	ModeAlarmBase::ModeAlarmBase()
		: ModeBase()
	{
		currAspect = 0;
		triggered = false;
		alarm = EepromVariable<Alarm>();
		Alarm default_ = { 0, 0, false };
		alarm.setDefault(default_);
		if (alarm.get().active)
			Wordclock::startTimer(this, 0, 1);
	}

	uint32_t ModeAlarmBase::getAlarmTimerTime(const Alarm &a)
	{
		uint8_t hours = (24 + a.hour - Wordclock::getTime(Hours)) % 24;
		uint16_t minutes = (60 + a.minute - Wordclock::getTime(Minutes)) % 60;
		if (Wordclock::getTime(Minutes) + minutes >= 60) {
			hours--;
		}
		// Start checking the alarm a little earlier since timer and real time
		// might not be in sync.
		minutes += 60*hours;
		if (minutes > 0)
			minutes--;
		return 60000 * minutes;
	}

	uint8_t ModeAlarmBase::incVal(uint8_t val, const uint8_t &max,
		const uint8_t &increment, const bool &inc)
	{
		if (inc)
			val += increment;
		else
			val = val + max - increment;
		return val % max;
	}

	void ModeAlarmBase::internalActionButton(const bool &inc,
		const uint8_t &increment)
	{
		Wordclock::repaint();
		Alarm &a = alarm.get();
		if (a.active && !(inc && currAspect == 0))
			Wordclock::cancelTimer(this, 0);

		if (currAspect == 0) {
			if (inc) {
				currAspect = 1;
				return;
			}
			else {
				a.active = !a.active;
			}
		}
		else if (currAspect == 1) {
			a.hour = incVal(a.hour,
				Wordclock::getMaximumTime(Hours), 1, inc);
		}
		else if (currAspect == 2) {
			a.minute = incVal(a.minute,
				Wordclock::getMaximumTime(Minutes), increment, inc);
		}
		alarm.save();
		if (a.active)
			Wordclock::startTimer(this, getAlarmTimerTime(alarm.get()), 0);
		triggered = false;
	}

	void ModeAlarmBase::internalModeButton(const bool &inc, const uint8_t &max)
	{
		if (currAspect == 0) {
			super::modeButton(inc);
			return;
		}
		currAspect = inc ? currAspect % max + 1 : 0;
		Wordclock::repaint();
	}

	void ModeAlarmBase::modeButton(const bool &inc)
	{
		internalModeButton(inc, 2);
	}

	uint32_t ModeAlarmBase::timer(const uint8_t &channel)
	{
		if (channel == 0) {
			const Alarm &a = alarm.get();
			triggered = triggered || Wordclock::getTime(Hours) == a.hour &&
				Wordclock::getTime(Minutes) == a.minute;
			return triggered ? 86350000 : 500;
		}
		else if (channel == 1) {
			// start looking for alarm after startup
			Wordclock::startTimer(this, getAlarmTimerTime(alarm.get()), 0);
			return 0;
		}
		else
			return ModeBase::timer(channel);
	}

	void ModeAlarmBase::paint()
	{
		if (isInTransition()) {
			ModeBase::paint();
			return;
		}
		const Alarm &a = alarm.get();
		Painter::setColor(Wordclock::getCurrentPreset());
		if (currAspect == 0) {
			Utilities::printAnswere(a.active);
		}
		else if (currAspect == 1) {
			Painter::paint(0, 0, a.hour >= 12);
			Utilities::printTime(a.hour, a.minute);
		}
		else if (currAspect == 2) {
			Painter::paint(0, 0, a.minute % 5);
			Utilities::printTime(a.hour, a.minute);
		}
	}

	// You must not set this to 255 or 0.
	const uint8_t ModeAlarmChangeModeBase::maxReturnTime = 100;

	ModeAlarmChangeModeBase *ModeAlarmChangeModeBase::activeAlarm = nullptr;
	uint8_t ModeAlarmChangeModeBase::returnMode = 0;

	void ModeAlarmChangeModeBase::aboardActiveAlarm(const uint8_t &layer)
	{
		if (activeAlarm != nullptr) {
			if (activeAlarm->triggered) {
				activeAlarm->triggered = false;
				Wordclock::setMode(layer, returnMode);
				Wordclock::startTimer(activeAlarm,
					getAlarmTimerTime(activeAlarm->alarm.get()), 0);
			}
			Wordclock::cancelTimer(activeAlarm, 2);
			Wordclock::cancelTimer(activeAlarm, 3);
		}
	}

	ModeAlarmChangeModeBase::ModeAlarmChangeModeBase(const uint8_t &minMode)
		: ModeAlarmBase()
	{
		mode = EepromVariable<ModeData>();
		ModeData m = { minMode, 0 };
		mode.setDefault(m);
	}

	uint32_t ModeAlarmChangeModeBase::internalTimer(const uint8_t &channel,
		const uint8_t &layer)
	{
		if (channel == 3 || channel == 2 && (!triggered ||
			Wordclock::getMode(layer) != mode.get().mode))
		{
			// If the user selects another mode or its time to return to the
			// previous mode, aboard all alarms here or return to the previous
			// mode.
			aboardActiveAlarm(layer);
			return 0;
		}
		else if (channel == 2) {
			// This exists just in order that the section above is called on a
			// regular basis.
			return BUTTON_LOCK_TIME;
		}

		bool oldTriggered = triggered;
		uint32_t time = super::timer(channel);
		if (triggered && !oldTriggered) {
			aboardActiveAlarm(layer);

			const ModeData &m = mode.get();
			if (m.returnTime == 0) {
				triggered = false;
			}
			else {
				activeAlarm = this;
				returnMode = Wordclock::getMode(layer);
				// check whether the timer is cancel manually.
				Wordclock::startTimer(this, BUTTON_LOCK_TIME, 2);
				// return after a certain time to this mode.
				Wordclock::startTimer(this, 60000 * m.returnTime, 3);
			}
			Wordclock::setMode(layer, m.mode);
		}
		return time;
	}

	void ModeAlarmChangeModeBase::internalActionButton(const bool &inc,
		const uint8_t &minMode, const uint8_t &maxMode,
		const uint8_t &increment)
	{
		Wordclock::repaint();
		if (currAspect == 3 || currAspect == 4) {
			ModeData &m = mode.get();
			if (currAspect == 3)
				m.returnTime = incVal(m.returnTime, 100, increment, inc);
			else
				m.mode = incVal(m.mode - minMode, maxMode - minMode, 1, inc)
					+ minMode;
			mode.save();
		}
		else {
			super::internalActionButton(inc, increment);
		}
	}

	void ModeAlarmChangeModeBase::modeButton(const bool &inc)
	{
		internalModeButton(inc, 4);
	}
	
	void ModeAlarmChangeModeBase::paint()
	{
		super::paint();
		if (isInTransition())
			return;
		const ModeData &m = mode.get();
		if (currAspect == 3) {
			if (m.returnTime == 0)
				Utilities::printAnswere(false);
			else if (maxReturnTime <= 100)
				Utilities::printNumber(m.returnTime);
			else
				Utilities::printHex(m.returnTime);
		}
		else if (currAspect == 4) {
			Utilities::printNumber(m.mode + 1);
		}
	}
}