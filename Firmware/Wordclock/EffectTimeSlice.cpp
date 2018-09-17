
#include "EffectTimeSlice.hpp"
#include <FastLED.h>

namespace Wordclock
{
	uint8_t EffectTimeSliceBase::calcLength(const uint32_t &l, const uint8_t &column, const uint8_t &max)
	{
		uint16_t length = (l * ((column << 1) + 1) + 0x8000) >> 16;
		if (length > max)
			return max;
		return (uint8_t)length;
	}

	void EffectTimeSliceBase::paintSlice(Painter* painter, uint32_t rec, uint32_t maxSecs)
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

		painter->setForeground(Wordclock::getCurrentPreset());
		painter->markAll();

		painter->setForeground(Wordclock::getCurrentPreset(1));
		painter->setBackground(Wordclock::getCurrentPreset(2));

		if (rec == 0x10000) {
			painter->markAll();
			return;
		}
		if (rec >= 0x4000) {
			painter->mark(hWidth, 0, Painter::width - hWidth, Painter::height - hHeight);
			if (rec >= 0x8000) {
				painter->mark(rhWidth, rhHeight, hWidth, hHeight);
				if (rec >= 0xC000) {
					painter->mark(0, hHeight, Painter::width - hWidth, Painter::height - hHeight);
				}
			}
		}

		// calculate tangent
		int32_t l = sin16(rec % 0x4000);
		l <<= 16;
		l /= cos16(rec % 0x4000);

		uint8_t length;
		if (rec < 0x4000) {
			for (uint8_t column = 0; column < rhHeight; column++) {
				length = calcLength(l, column, rhWidth);
				painter->mark(hWidth, rhHeight - column - 1, length, 1);
			}
		}
		else if (rec < 0x8000) {
			for (uint8_t column = 0; column < hWidth; column++) {
				length = calcLength(l, column, hHeight);
				painter->mark(rhWidth + column, rhHeight, 1, length);
			}
		}
		else if (rec < 0xC000) {
			for (uint8_t column = 0; column < rhHeight; column++) {
				length = calcLength(l, column, rhWidth);
				painter->mark(rhWidth - length, hHeight + column, length, 1);
			}
		}
		else if (rec < 0x10000) {
			for (uint8_t column = 0; column < hWidth; column++) {
				length = calcLength(l, column, hHeight);
				painter->mark(hWidth - column - 1, hHeight - length, 1, length);
			}
		}
	}

	void EffectTimeSliceBase::select()
	{
		update();
	}

	void EffectTimeSliceBase::update()
	{
		Wordclock::repaint();
		startTimer(1000);
	}
}