#pragma once

#include "ModeBase.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"

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
	/// @tparam letter - the letter that represents the mode.
	template <char letter>
	class ModeListAlarms : public ModeListAlarmsBase
	{
	public:
		void paint();
	};

	// implementation //

	template <char letter>
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