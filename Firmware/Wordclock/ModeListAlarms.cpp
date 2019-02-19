
#include "ModeListAlarms.hpp"

#if ALARM_COUNT > 0
namespace Wordclock
{
	bool ModeListAlarmsBase::isInc = false;
	uint8_t ModeListAlarmsBase::curr = 0;
	uint8_t ModeListAlarmsBase::n = 0;
	AlarmTime ModeListAlarmsBase::time = { 0, 0, 0 };
	AlarmTime ModeListAlarmsBase::time0 = { 0, 0, 0 };

	bool ModeListAlarmsBase::listAlarms(const AlarmTime &t)
	{
		if (isInc) {
			if (n == 0) {
				time0.setTo(t);
			}
			if (n == curr) {
				time.setTo(t);
			}
		}
		else {
			time0.setTo(t);

			if (curr > 0 && n == curr) {
				time.setTo(t);
			}
		}
		n++;
		return false;
	}

	void ModeListAlarmsBase::select()
	{
		isInc = true;
		n = 0;
		curr = 0;

		time.weekday = 7;
		Wordclock::listAlarms(listAlarms);
	}

	void ModeListAlarmsBase::increment(const bool &inc)
	{
		isInc = inc;
		n = 0;
		if (isInc || curr > 0)
			curr = inc ? curr + 1 : curr - 1;

		time.weekday = 7;
		Wordclock::listAlarms(listAlarms);
		if (time.weekday > 6) {
			curr = inc ? 0 : n - 1;
			time.setTo(time0);
		}
	}
}
#endif