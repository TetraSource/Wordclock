#pragma once

#include "EffectBase.hpp"
#include "Painter.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	class EffectTimeSliceBase : public EffectBase
	{
	protected:
		static uint8_t calcLength(const uint32_t &l, const uint8_t &column, const uint8_t &max);

		static void paintSlice(Painter* painter, uint32_t rec, uint32_t maxSecs);

	public:
		void select();

		void update();
	};

	/// shows the time using a time slice.
	/// @tparam timeType - the unit of the time type that the progress of which is shown
	/// @tparam scope - the scope of time that is shown. If the time exceeds this value, it will be displayed as 0 again.
	///                 This value never exceeds the maximal possible time for the given time type.
	///                 You can safely pass 255 to exploit the maximal possible scope always.
	template <TimeTypes timeType = Minutes, uint8_t scope = 255>
	class EffectTimeSlice : public EffectTimeSliceBase
	{
	public:
		void paint(Painter* painter);
	};

	template <TimeTypes timeType, uint8_t scope>
	void EffectTimeSlice<timeType, scope>::paint(Painter* painter)
	{
		uint32_t maxSecs = scope;
		if (timeType < Years) {
			maxSecs = Wordclock::getUnitSeconds(timeType + 1);
			if (maxSecs > ((uint32_t)scope) * Wordclock::getUnitSeconds(timeType))
				maxSecs = ((uint32_t)scope) * Wordclock::getUnitSeconds(timeType);
		}
		paintSlice(painter, Wordclock::getSeconds(timeType) % maxSecs + 1, maxSecs);
	}
}