#pragma once

#include "EffectBase.hpp"
#include "Painter.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	class EffectFillerBase : public EffectBase
	{
	protected:
		void paintArea(Painter* painter, uint32_t amount, uint32_t maxSecs, uint8_t direction);

	public:
		void select();

		void update();
	};

	/// Shows the time by turning on a certain amount of the display.
	/// The current and the two next color presets are used thereby.
	/// @tparam direction - specifies the side whence the filling advances (enum Wordclock::Directions).
	/// @tparam timeType - the unit of the time type that the progress of which is shown
	/// @tparam scope - the scope of time that is shown. If the time exceeds this value, it will be displayed as 0 again.
	///                 This value never exceeds the maximal possible time for the given time type.
	///                 You can safely pass 255 to exploit the maximal possible scope always.
	template <Directions direction, TimeTypes timeType = Minutes, uint8_t scope = 255>
	class EffectFiller : public EffectFillerBase
	{
	public:
		void paint(Painter* painter);
	};

	template <Directions direction, TimeTypes timeType, uint8_t scope>
	void EffectFiller<direction, timeType, scope>::paint(Painter* painter)
	{
		uint32_t maxSecs = scope;
		if (timeType < Years) {
			maxSecs = Wordclock::getUnitSeconds(timeType + 1);
			if (maxSecs >((uint32_t)scope) * Wordclock::getUnitSeconds(timeType))
				maxSecs = ((uint32_t)scope) * Wordclock::getUnitSeconds(timeType);
		}
		paintArea(painter, Wordclock::getSeconds(timeType) % maxSecs + 1, maxSecs, direction);
	}
}