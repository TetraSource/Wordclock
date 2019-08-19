#include "ModeFiller.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	void modeFillerPaint(const CRGB &color, const Directions &direction,
		const TimeTypes &timeType, uint8_t maxSecs)
	{
		if (timeType < Years) {
			uint32_t maxMaxSecs = maxSecs;
			maxSecs = Wordclock::getUnitSeconds(timeType + 1);
			maxMaxSecs *= Wordclock::getUnitSeconds(timeType);
			if (maxSecs > maxMaxSecs)
				maxSecs = maxMaxSecs;
		}

		uint32_t amount = Wordclock::getSeconds(timeType) % maxSecs + 1;
		while (amount > 0xffffff) {
			amount >>= 1;
			maxSecs >>= 1;
		}

		Painter::setColor(color);
		if (direction == Border) {
			Painter::paintAll();
			Painter::setColor(CRGB(0, 0, 0));
		}

		switch (direction)
		{
		case Top:
		{
			uint8_t filled = (amount * Painter::height) / maxSecs;
			Painter::paint(0, 0, Painter::width, filled);
		}
			break;
		case Bottom:
		{
			uint8_t filled = (amount * Painter::height) / maxSecs;
			Painter::paint(
				0, Painter::height - filled, Painter::width, filled);
		}
			break;
		case Left:
		{
			uint8_t filled = (amount * Painter::height) / maxSecs;
			Painter::paint(0, 0, filled, Painter::height);
		}
			break;
		case Right:
		{
			uint8_t filled = (amount * Painter::height) / maxSecs;
			Painter::paint(
				Painter::width - filled, 0, filled, Painter::height);
		}
			break;
		case Center:
		case Border:
		{
			uint8_t filledW = ((amount * Painter::width) / maxSecs);
			uint8_t filledH = ((amount * Painter::height) / maxSecs);
			filledW = Painter::width % 2 ? filledW | 0x01 : filledW & 0xfe;
			filledH = Painter::height % 2 ? filledH | 0x01 : filledH & 0xfe;
			Painter::paint(
				(Painter::width - filledW) >> 1,
				(Painter::height - filledH) >> 1,
				filledW, filledH);
		}
		}
	}
}