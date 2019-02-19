
#include "ModeAlarm.hpp"

#if ALARM_COUNT > 0
namespace Wordclock
{
	bool ModeAlarmBase::success = false;
	AlarmTime ModeAlarmBase::time = AlarmTime();

	bool ModeAlarmBase::removeAlarm(const AlarmTime &t)
	{
		if ((time.weekday >= 7 || t.weekday == time.weekday) &
			t.hour == time.hour && t.minute == time.minute) {

			success = true;
			return true;
		}
		return false;
	}

	void ModeAlarmBase::select()
	{
		success = false;
	}
}
#endif