
#include "EffectWaves.hpp"

namespace Wordclock
{
	CRGB* EffectWavesBase::colors = new CRGB[0];
	uint8_t EffectWavesBase::count = 0;

	void EffectWavesBase::nextColor()
	{
		for (uint8_t i = count; i > 0; i--) {
			colors[i] = colors[i - 1];
		}

		colors[0] = gen->nextRGBColor();
		if (Painter::areIdentical(Wordclock::getColorPreset(Wordclock::getColorPresetIndex()), colors[0]))
			colors[0] = gen->nextRGBColor();

		Wordclock::repaint();
	}

	void EffectWavesBase::init(const uint8_t &newCount)
	{
		count = newCount;
		colors = new CRGB[count];

		CRGB curr = Wordclock::getColorPreset(Wordclock::getColorPresetIndex());
		for (uint8_t i = 0; i < count; i++) {
			colors[i] = gen->nextRGBColor();
			if (Painter::areIdentical(curr, colors[i]))
				colors[i] = gen->nextRGBColor();
		}

		startTimer(1);
	}

	EffectWavesBase::EffectWavesBase(GeneratorBase* generator)
	{
		gen = generator;
	}

	void EffectWavesBase::deselect()
	{
		count = 0;
		delete[] colors;
	}
}