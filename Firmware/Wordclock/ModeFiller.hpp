#pragma once

#include "ModeBase.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	class ModeFillerBase : public ModeBase
	{
	public:
		void select();

		void timer();
	};

	/// Shows the time by turning on a certain amount of the display.
	/// The current and the two next color presets are used thereby.
	/// @tparam direction - specifies the side whence the filling advances
	///                     (enum Wordclock::Directions).
	/// @tparam timeType - the unit of the time for the scope
	/// @tparam scope - the scope of time that is shown. If the time exceeds
	///                 this value, it will be displayed as 0 again.
	///                 This value never exceeds the maximal possible time for
	///                 the given time type. You can safely pass 255 to use
	///                 the maximal possible scope always.
	template <Directions direction,
		TimeTypes timeType = Minutes,
		uint8_t scope = 255>
	class ModeFiller : public ModeFillerBase
	{
	public:
		void paint();
	};

	template <Directions direction, TimeTypes timeType, uint8_t scope>
	void ModeFiller<direction, timeType, scope>::paint()
	{
		uint32_t maxSecs = scope;
		if (timeType < Years) {
			maxSecs = Wordclock::getUnitSeconds(timeType + 1);
			if (maxSecs > ((uint32_t)scope) *
				Wordclock::getUnitSeconds(timeType))
			{
				maxSecs = ((uint32_t)scope) *
					Wordclock::getUnitSeconds(timeType);
			}
		}

		uint32_t amount = Wordclock::getSeconds(timeType) % maxSecs + 1;
		while (amount > 0xffffff) {
			amount >>= 1;
			maxSecs >>= 1;
		}

		Painter::setColor(Wordclock::getCurrentPreset(1));
		if (direction == Border) {
			Painter::paintAll();
			Painter::setColor(CRGB(0, 0, 0));
		}

		if (direction == Top) {
			uint8_t filled = (amount * Painter::height) / maxSecs;
			Painter::paint(0, 0, Painter::width, filled);
		}
		else if (direction == Bottom) {
			uint8_t filled = (amount * Painter::height) / maxSecs;
			Painter::paint(
				0, Painter::height - filled, Painter::width, filled);
		}
		else if (direction == Left) {
			uint8_t filled = (amount * Painter::height) / maxSecs;
			Painter::paint(
				0, 0, filled, Painter::height);
		}
		else if (direction == Right) {
			uint8_t filled = (amount * Painter::height) / maxSecs;
			Painter::paint(
				Painter::width - filled, 0, filled, Painter::height);
		}
		else if (direction == Center || direction == Border) {
			uint8_t filledW = ((amount * Painter::width) / maxSecs);
			uint8_t filledH = ((amount * Painter::height) / maxSecs);
			filledW = Painter::width % 2 ? filledW | 0x01 : filledW & 0xfe;
			filledH = Painter::height % 2 ? filledH | 0x01 : filledH & 0xfe;
			Painter::paint(
				(Painter::width - filledW) >> 1,
				(Painter::height - filledH) >> 1,
				filledW, filledH);
		}

		// Paint time
		Painter::setColor(Wordclock::getCurrentPreset());
		Utilities::printTime();
	}
}
