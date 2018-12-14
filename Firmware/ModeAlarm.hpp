#pragma once

#include "Marker.hpp"
#include "ModeBase.hpp"
#include "Wordclock.hpp"
#include "Utilities.hpp"

namespace Wordclock
{
	enum ModeAlarmTypes
	{
		AlarmWeekday,
		AlarmHour,
		AlarmMinute,
		SetAlarm,
	};

	class ModeAlarmBase : public ModeBase
	{
	protected:
		static bool success;
		static AlarmTime time;

		static bool removeAlarm(const AlarmTime &t);

	public:
		void select();
	};

	/// allows to set and remove alarms manually.
	/// @tparam letter the letter that represents the mode.
	/// @tparam type the element that is set by the mode.
	template <char letter, ModeAlarmTypes type>
	class ModeAlarm : public ModeAlarmBase
	{
	public:
		void increment(const bool &inc);

		void shape(Marker* marker);
	};

	// implementation //

	template <char letter, ModeAlarmTypes type>
	void ModeAlarm<letter, type>::increment(const bool &inc)
	{
		switch (type)
		{
		case AlarmMinute:
			if (inc)
				time.minute = time.minute >= 55 ? 0 : time.minute + 5;
			else
				time.minute = time.minute == 0 ? 55 : time.minute - 5;
			break;

		case AlarmHour:
			time.hour = Utilities::changeValue(time.hour, 24, inc);
			break;

		case AlarmWeekday:
			time.weekday = Utilities::changeValue(time.weekday, 8, inc);
			break;

		case SetAlarm:
			if (!success) {
				if (inc)
					success = Wordclock::addAlarm(AlarmTime(time.weekday >= 7 ? 0xff : time.weekday, time.hour, time.minute));
				else
					Wordclock::listAlarms(&removeAlarm);
			}
		}
		Wordclock::reshape();
	}

	template <char letter, ModeAlarmTypes type>
	void ModeAlarm<letter, type>::shape(Marker* marker)
	{
		switch (type)
		{
		case AlarmMinute:
			Utilities::shapeNumber(marker, time.minute == 0 ? 60 : time.minute);
			break;

		case AlarmHour:
			Utilities::shapeNumber(marker, time.hour == 0 ? 24 : time.hour);
			break;

		case AlarmWeekday:
			if (time.weekday >= 7) {
				Utilities::shapeNumber(marker, 17);
				marker->mark(4, 5, 3);
			}
			else {
				Utilities::shapeNumber(marker, time.weekday == 0 ? 7 : time.weekday);
			}
			break;

		case SetAlarm:
			Utilities::shapeAnswere(marker, success);
		}

#ifdef SHOW_MODE
		marker->mark(letter);
#endif
	}
}