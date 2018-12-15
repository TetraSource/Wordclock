#pragma once

#include "ModeBase.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	class ModeTimeSliceBase : public ModeBase
	{
	protected:
		static uint8_t calcLength(
			const uint32_t &l,
			const uint8_t &column,
			const uint8_t &max);

		static void paintSlice(uint32_t rec, uint32_t maxSecs);

	public:
		void select();

		void timer();
	};

	/// shows the time using a time slice.
	/// @tparam timeType - the unit of the time for the scope
	/// @tparam scope - the scope of time that is shown. If the time exceeds
	///                 this value, it will be displayed as 0 again.
	///                 This value never exceeds the maximal possible time for
	///                 the given time type. You can safely pass 255 to use
	///                 the maximal possible scope always.
	template <TimeTypes timeType = Minutes, uint8_t scope = 255>
	class ModeTimeSlice : public ModeTimeSliceBase
	{
	public:
		void paint();
	};

	template <TimeTypes timeType, uint8_t scope>
	void ModeTimeSlice<timeType, scope>::paint()
	{
		uint32_t maxSecs = scope;
		if (timeType < Years) {
			maxSecs = Wordclock::getUnitSeconds(timeType + 1);
			if (maxSecs >
				((uint32_t)scope) * Wordclock::getUnitSeconds(timeType))
			{
				maxSecs =
					((uint32_t)scope) * Wordclock::getUnitSeconds(timeType);
			}
		}

		paintSlice(Wordclock::getSeconds(timeType) % maxSecs + 1, maxSecs);

		// print time
		Painter::setColor(Wordclock::getCurrentPreset());
		Utilities::printTime();
	}
}
