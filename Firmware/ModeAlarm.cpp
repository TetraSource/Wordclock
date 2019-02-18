
#include "ModeAlarm.hpp"

namespace Wordclock
{
	bool ModeAlarmBase::success = false;
	AlarmTime ModeAlarmBase::time = { 7, 0, 0 };

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