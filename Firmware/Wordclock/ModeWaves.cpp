
#include "ModeWaves.hpp"

namespace Wordclock
{
	CRGB* ModeWavesBase::colors = new CRGB[0];
	uint8_t ModeWavesBase::count = 0;

	void ModeWavesBase::nextColor()
	{
		for (uint8_t i = count; i > 0; i--) {
			colors[i] = colors[i - 1];
		}

		colors[0] = gen->nextRGBColor();
		if (Painter::areIdentical(Wordclock::getColorPreset(
			Wordclock::getColorPresetIndex()), colors[0]))
		{
			colors[0] = gen->nextRGBColor();
		}

		Wordclock::repaint();
	}

	void ModeWavesBase::init(const uint8_t &newCount)
	{
		count = newCount;
		colors = new CRGB[count];
		CRGB curr = Wordclock::getColorPreset(
			Wordclock::getColorPresetIndex());

		for (uint8_t i = 0; i < count; i++) {
			colors[i] = gen->nextRGBColor();
			if (Painter::areIdentical(curr, colors[i]))
				colors[i] = gen->nextRGBColor();
		}

		startTimer(1);
	}

	ModeWavesBase::ModeWavesBase(GeneratorBase* generator)
	{
		gen = generator;
	}

	void ModeWavesBase::deselect()
	{
		count = 0;
		delete[] colors;
	}
}