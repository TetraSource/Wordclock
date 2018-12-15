
#include "ModeFlyingPixels.hpp"

namespace Wordclock
{
	ModeFlyingPixelsBase::FlyingPixel* ModeFlyingPixelsBase::pixels =
		new ModeFlyingPixelsBase::FlyingPixel[0];

	uint8_t ModeFlyingPixelsBase::count = 0;

	ModeFlyingPixelsBase::ModeFlyingPixelsBase(GeneratorBase* generator)
	{
		gen = generator;
	}

	void ModeFlyingPixelsBase::init(const uint8_t &newCount)
	{
		count = 0;
		pixels = new FlyingPixel[newCount];

		startTimer(1);
	}

	void ModeFlyingPixelsBase::deselect()
	{
		delete[] pixels;
	}
}