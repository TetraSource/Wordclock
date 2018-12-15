#pragma once

#include "ModeBase.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"

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

		void paint();
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
					success = Wordclock::addAlarm(
						AlarmTime(time.weekday >= 7 ? 0xff : time.weekday,
							time.hour, time.minute));
				else
					Wordclock::listAlarms(&removeAlarm);
			}
		}
		Wordclock::repaint();
	}

	template <char letter, ModeAlarmTypes type>
	void ModeAlarm<letter, type>::paint()
	{
		switch (type)
		{
		case AlarmMinute:
			Utilities::printNumber(time.minute == 0 ? 60 : time.minute);
			break;

		case AlarmHour:
			Utilities::printNumber(time.hour == 0 ? 24 : time.hour);
			break;

		case AlarmWeekday:
			if (time.weekday >= 7) {
				Utilities::printNumber(17);
				Painter::paint(4, 5, 3);
			}
			else {
				Utilities::printNumber(time.weekday == 0 ? 7 : time.weekday);
			}
			break;

		case SetAlarm:
			Utilities::printAnswere(success);
		}

#ifdef SHOW_MODE
		Painter::paint(letter);
#endif
	}
}