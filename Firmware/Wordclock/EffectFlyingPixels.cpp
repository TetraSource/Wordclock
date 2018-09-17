
#include "EffectFlyingPixels.hpp"

namespace Wordclock
{
	EffectFlyingPixelsBase::FlyingPixel* EffectFlyingPixelsBase::pixels = new EffectFlyingPixelsBase::FlyingPixel[0];
	uint8_t EffectFlyingPixelsBase::count = 0;

	EffectFlyingPixelsBase::EffectFlyingPixelsBase(GeneratorBase* generator)
	{
		gen = generator;
	}

	void EffectFlyingPixelsBase::init(const uint8_t &newCount)
	{
		count = 0;
		pixels = new FlyingPixel[newCount];

		startTimer(1);
	}

	void EffectFlyingPixelsBase::deselect()
	{
		delete[] pixels;
	}
}