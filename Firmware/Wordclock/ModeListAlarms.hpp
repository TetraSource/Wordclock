#pragma once

#include "ModeBase.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"

#if ALARM_COUNT > 0
namespace Wordclock
{
	class ModeListAlarmsBase : public ModeBase
	{
	protected:
		static bool isInc;
		static uint8_t curr;
		static uint8_t n;
		static AlarmTime time;
		static AlarmTime time0;

		static bool listAlarms(const AlarmTime &t);

	public:
		void select();

		void increment(const bool &inc);
	};

	/// allows to view all set alarms.
	/// @tparam letter the position of the letter that represents the mode,
	///                calculated by the POINT macro.
	template <uint8_t letter>
	class ModeListAlarms : public ModeListAlarmsBase
	{
	public:
		void paint();
	};

	// implementation //

	template <uint8_t letter>
	void ModeListAlarms<letter>::paint()
	{
		if (time.weekday > 6) {
			Utilities::printAnswere(false);
		}
		else {
			Utilities::printTime(time.hour, time.minute);
			Painter::paint(time.weekday, 0);
		}

#ifdef SHOW_MODE
		Painter::paint(letter);
#endif
	}
}
#endif