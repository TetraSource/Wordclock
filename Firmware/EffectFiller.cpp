
#include "EffectFiller.hpp"

namespace Wordclock
{
	void EffectFillerBase::paintArea(Painter* painter, uint32_t amount, uint32_t maxSecs, uint8_t direction)
	{
		while (amount > 0xffffff) {
			amount >>= 1;
			maxSecs >>= 1;
		}

		if (direction == Border) {
			painter->setForeground(Wordclock::getCurrentPreset(1));
			painter->setBackground(Wordclock::getCurrentPreset(2));
		}
		else {
			painter->setForeground(Wordclock::getCurrentPreset());
			painter->setBackground(CRGB(0, 0, 0));
		}
		painter->markAll();

		if (direction == Border) {
			painter->setForeground(Wordclock::getCurrentPreset());
			painter->setBackground(CRGB(0, 0, 0));
		}
		else {
			painter->setForeground(Wordclock::getCurrentPreset(1));
			painter->setBackground(Wordclock::getCurrentPreset(2));
		}

		if (direction == Top) {
			uint8_t filled = (amount * Painter::height) / maxSecs;
			painter->mark(0, 0, Painter::width, filled);
		}
		else if (direction == Bottom) {
			uint8_t filled = (amount * Painter::height) / maxSecs;
			painter->mark(0, Painter::height - filled, Painter::width, filled);
		}
		else if (direction == Left) {
			uint8_t filled = (amount * Painter::height) / maxSecs;
			painter->mark(0, 0, filled, Painter::height);
		}
		else if (direction == Right) {
			uint8_t filled = (amount * Painter::height) / maxSecs;
			painter->mark(Painter::width - filled, 0, filled, Painter::height);
		}
		else if (direction == Center || direction == Border) {
			uint8_t filledW = ((amount * Painter::width) / maxSecs);
			uint8_t filledH = ((amount * Painter::height) / maxSecs);
			filledW = Painter::width % 2 ? filledW | 0x01 : filledW & 0xfe;
			filledH = Painter::height % 2 ? filledH | 0x01 : filledH & 0xfe;
			painter->mark((Painter::width - filledW) >> 1, (Painter::height - filledH) >> 1, filledW, filledH);
		}
	}

	void EffectFillerBase::select()
	{
		update();
	}

	void EffectFillerBase::update()
	{
		Wordclock::repaint();
		startTimer(1000);
	}
}