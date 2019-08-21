
#include <FastLED.h>
#include "ModeTimeSlice.hpp"
#include "Utilities.hpp"

namespace Wordclock
{
	ModeTimeSliceBase::ModeTimeSliceBase()
		: ModeTimeBound()
	{}

	uint8_t ModeTimeSliceBase::calcLength(
		const uint32_t &l,
		const uint8_t &column,
		const uint8_t &max)
	{
		uint16_t length = (l * ((column << 1) + 1) + 0x8000) >> 16;
		if (length > max)
			return max;
		return (uint8_t)length;
	}

	void ModeTimeSliceBase::paintSlice(const CRGB &color,
		uint32_t rec, uint32_t maxSecs)
	{
		const uint8_t hWidth = Painter::width >> 1;
		const uint8_t hHeight = Painter::height >> 1;
		const uint8_t rhWidth = (Painter::width + 1) >> 1;
		const uint8_t rhHeight = (Painter::height + 1) >> 1;

		while (rec > 0xffff) {
			rec >>= 1;
			maxSecs >>= 1;
		}
		rec <<= 16; // rectangle = (rectangle) * (unit of rectangle)
		rec /= maxSecs;

		Painter::setColor(color);

		if (rec == 0x10000) {
			// Paint the entire display if the slice fills it completly.
			Painter::paintAll();
			return;
		}
		// Paint all completely filled quarters of the slice.
		if (rec >= 0x4000) {
			Painter::paint(
				hWidth, 0,
				Painter::width - hWidth, Painter::height - hHeight);
			if (rec >= 0x8000) {
				Painter::paint(rhWidth, rhHeight, hWidth, hHeight);
				if (rec >= 0xC000) {
					Painter::paint(
						0, hHeight,
						Painter::width - hWidth, Painter::height - hHeight);
				}
			}
		}

		// calculate tangent
		int32_t l = sin16(rec % 0x4000);
		l <<= 16;
		l /= cos16(rec % 0x4000);

		// Paint the remaining quarter of the slice.
		uint8_t length;
		if (rec < 0x4000) {
			for (uint8_t column = 0; column < rhHeight; column++) {
				length = calcLength(l, column, rhWidth);
				Painter::paint(hWidth, rhHeight - column - 1, length, 1);
			}
		}
		else if (rec < 0x8000) {
			for (uint8_t column = 0; column < hWidth; column++) {
				length = calcLength(l, column, hHeight);
				Painter::paint(rhWidth + column, rhHeight, 1, length);
			}
		}
		else if (rec < 0xC000) {
			for (uint8_t column = 0; column < rhHeight; column++) {
				length = calcLength(l, column, rhWidth);
				Painter::paint(
					rhWidth - length, hHeight + column,
					length, 1);
			}
		}
		else if (rec < 0x10000) {
			for (uint8_t column = 0; column < hWidth; column++) {
				length = calcLength(l, column, hHeight);
				Painter::paint(
					hWidth - column - 1,
					hHeight - length, 1, length);
			}
		}
	}
}