
#include "ModeWaves.hpp"

namespace Wordclock
{
	CRGB *ModeWavesBase::colors = nullptr;
	uint8_t ModeWavesBase::count = 0;

	ModeWavesBase::ModeWavesBase()
		: ModeBase()
	{}

	void ModeWavesBase::nextColor()
	{
		for (uint8_t i = count-1; i != 0; i--) {
			colors[i] = colors[i - 1];
		}
		Wordclock::repaint();
	}

	void ModeWavesBase::printCenterWaves()
	{
		uint8_t x0 = (Painter::width - 1) >> 1;
		uint8_t y0 = (Painter::height - 1) >> 1;
		uint8_t ringCount =
			Painter::width >= Painter::height ? (Painter::width + 1) >> 1 :
			(Painter::height + 1) >> 1;

		for (uint8_t i = 0; i < ringCount; i++) {
			Painter::setColor(colors[i % count]);

			uint8_t x1 = Painter::width - x0 - 1;
			uint8_t y1 = Painter::height - y0 - 1;

			for (uint8_t x = x0; x <= x1; x++) {
				Painter::paint(x, y0);
				Painter::paint(x, y1);
			}
			for (uint8_t y = y0 + 1; y < y1; y++) {
				Painter::paint(x0, y);
				Painter::paint(x1, y);
			}

			x0--;
			y0--;
		}
	}

	void ModeWavesBase::printBorderWaves()
	{
		uint8_t ringCount =
			Painter::width >= Painter::height ? (Painter::width + 1) >> 1 :
			(Painter::height + 1) >> 1;

		for (uint8_t i = 0; i < ringCount; i++) {
			Painter::setColor(colors[i % count]);

			uint8_t &x0 = i;
			uint8_t &y0 = i;
			uint8_t x1 = Painter::width - x0 - 1;
			uint8_t y1 = Painter::height - y0 - 1;

			for (uint8_t x = x0; x <= x1; x++) {
				Painter::paint(x, y0);
				Painter::paint(x, y1);
			}
			for (uint8_t y = y0 + 1; y < y1; y++) {
				Painter::paint(x0, y);
				Painter::paint(x1, y);
			}
		}
	}
}