#include "ModeAlarm.hpp"

namespace Wordclock
{
	// The time between each check whether the timer must be triggered in
	// seconds. Higher value might delay the triggering but lower value
	// yield are worse performance.
	// This value must be in range from 1 to 60.
	const uint8_t ModeAlarmBase::checkAlarmInterval = 2;

	ModeAlarmBase::ModeAlarmBase()
		: ModeBase()
	{
		currAspect = 0;
		triggered = false;
		changed = false;
		alarm = EepromVariable<Alarm>();
		Alarm default_ = { 0, 0, false };
		alarm.setDefault(default_);
		if (alarm.get().active)
			// start checking alarm immediately after startup
			Wordclock::startTimer(this, 0, 0);
	}

	uint8_t ModeAlarmBase::incVal(const uint8_t &val, const uint8_t &max,
		const uint8_t &increment, const bool &inc)
	{
		if (inc)
			return (val + increment) % max;
		else
			return (val + max - increment) % max;
	}

	void ModeAlarmBase::internalActionButton(
		const bool &inc, const uint8_t &increment)
	{
		Wordclock::repaint();
		triggered = false;
		Alarm &a = alarm.get();

		if (currAspect == 0) {
			if (a.active)
				Wordclock::cancelTimer(this, 0);
			if (inc) {
				currAspect = 1;
				changed = false;
			}
			else {
				a.active = !a.active;
				if (a.active)
					Wordclock::startTimer(this, 0, 0);
				alarm.save();
			}
			return; // don't set changed = true
		}
		else if (currAspect == 1) {
			a.hour = incVal(a.hour,
				Wordclock::getMaximumTime(Hours), 1, inc);
		}
		else if (currAspect == 2) {
			a.minute = incVal(a.minute,
				Wordclock::getMaximumTime(Minutes), increment, inc);
		}
		changed = true;
	}

	void ModeAlarmBase::internalModeButton(const bool &inc, const uint8_t &max)
	{
		if (currAspect == 0) {
			super::modeButton(inc);
			return;
		}
		currAspect = Utilities::changeValue(currAspect, max+1, inc);
		if (currAspect == 0) {
			if (changed)
				alarm.save();
			if (alarm.get().active)
				Wordclock::startTimer(this, 0, 0);
		}
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
			if (currAspect == 0 &&
				Wordclock::getTime(Hours) == a.hour &&
				Wordclock::getTime(Minutes) == a.minute)
			{
				triggered = true;
				return 60000;
			}
			return 1000*checkAlarmInterval;
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

	// Determines the maximal time in minutes that may be set until the
	// previous selected mode is selected again. Setting it to values higher
	// than 99 causes the return time to be displayed in hexadecimal.
	// You must not set this something in range from 255 or 0.
	const uint8_t ModeAlarmChangeModeBase::maxReturnTime = 99;

	ModeAlarmChangeModeBase *ModeAlarmChangeModeBase::activeAlarm = nullptr;
	uint8_t ModeAlarmChangeModeBase::returnMode = 0;

	void ModeAlarmChangeModeBase::aboardActiveAlarm(const uint8_t &layer)
	{
		if (activeAlarm != nullptr) {
			if (activeAlarm->triggered) {
				activeAlarm->triggered = false;
				Wordclock::setMode(layer, returnMode);
				Wordclock::startTimer(activeAlarm, 0, 0);
			}
			Wordclock::cancelTimer(activeAlarm, 1);
			activeAlarm = nullptr;
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
		if (channel == 1 || channel == 0 &&
			triggered && Wordclock::getMode(layer) != mode.get().mode)
		{
			// If the user selects another mode or its time to return to the
			// previous mode, aboard all alarms here or return to the previous
			// mode.
			if (channel == 0)
				returnMode = Wordclock::getMode(layer);
			aboardActiveAlarm(layer);
			return 0;
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
				// return after a certain time to this mode.
				returnMode = Wordclock::getMode(layer);
				Wordclock::startTimer(this, 60000 * m.returnTime, 1);
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
			if (currAspect == 3) {
				m.returnTime = incVal(m.returnTime,
					maxReturnTime+1, increment, inc);
			}
			else {
				m.mode = incVal(m.mode - minMode,
					maxMode - minMode, 1, inc) + minMode;
			}
			changed = true;
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
			else if (maxReturnTime < 100)
				Utilities::printNumber(m.returnTime);
			else
				Utilities::printHex(m.returnTime);
		}
		else if (currAspect == 4) {
			Utilities::printNumber(m.mode + 1);
		}
	}
}