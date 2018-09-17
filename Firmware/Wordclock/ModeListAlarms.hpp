#pragma once

#include "Marker.hpp"
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
		void shape(Marker* marker);
	};

	// implementation //

	template <char letter>
	void ModeListAlarms<letter>::shape(Marker* marker)
	{
		if (time.weekday > 6) {
			Utilities::shapeAnswere(marker, false);
		}
		else {
			Utilities::shapeTime(marker, time.hour, time.minute);
			marker->mark(time.weekday, 0);
		}

#ifdef SHOW_MODE
		marker->mark(letter);
#endif
	}
}