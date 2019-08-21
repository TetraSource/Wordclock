#pragma once

#include "ModeBaseInterval.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	class ModeTimeSliceBase : public ModeTimeBound
	{
	protected:
		typedef ModeTimeBound super;
		static uint8_t calcLength(
			const uint32_t &l,
			const uint8_t &column,
			const uint8_t &max);

		static void paintSlice(const CRGB &color,
			uint32_t rec, uint32_t maxSecs);
		
		ModeTimeSliceBase();
	};

	/// shows the time using a time slice.
	/// @tparam Generator - generates the color(s) of the time slice.
	/// @tparam timeType - the unit of the time for the scope
	/// @tparam scope - the scope of time that is shown. If the time exceeds
	///                 this value, it will be displayed as 0 again.
	///                 This value never exceeds the maximal possible time for
	///                 the given time type. You can safely pass 255 to use
	///                 the maximal possible scope always.
	template <class Generator, TimeTypes timeType = Minutes, uint8_t scope = 255>
	class ModeTimeSlice : public ModeTimeSliceBase
	{
	protected:
		typedef ModeTimeSliceBase super;
		Generator gen;
	public:
		ModeTimeSlice();
		void paint();
	};

	template <class Generator, TimeTypes timeType, uint8_t scope>
	ModeTimeSlice<Generator, timeType, scope>::ModeTimeSlice()
	{
		gen = Generator();
	}

	template <class Generator, TimeTypes timeType, uint8_t scope>
	void ModeTimeSlice<Generator, timeType, scope>::paint()
	{
		if (isInTransition()) {
			ModeBase::paint();
			return;
		}
		uint32_t maxSecs = scope;
		if (timeType < Years) {
			maxSecs = Wordclock::getUnitSeconds((TimeTypes)(timeType + 1));
			uint32_t maxMaxSecs = scope;
			maxMaxSecs *= Wordclock::getUnitSeconds(timeType);
			if (maxSecs > maxMaxSecs)
				maxSecs = maxMaxSecs;
		}
		paintSlice(gen.next(),
			Wordclock::getSeconds(timeType) % maxSecs + 1, maxSecs);
	}
}