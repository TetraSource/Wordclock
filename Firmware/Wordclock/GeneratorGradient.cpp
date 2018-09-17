
#include "GeneratorGradient.hpp"

namespace Wordclock
{
	uint8_t GeneratorGradientBase::rep = 0;
	int16_t GeneratorGradientBase::rVec = 0;
	int16_t GeneratorGradientBase::gVec = 0;
	int16_t GeneratorGradientBase::bVec = 0;
	int16_t GeneratorGradientBase::rVal = 0;
	int16_t GeneratorGradientBase::gVal = 0;
	int16_t GeneratorGradientBase::bVal = 0;

	GeneratorGradientBase::GeneratorGradientBase(GeneratorBase* generator)
	{
		CRGB color = generator->nextColor();

		gen = generator;
		rep = 0;
		rVal = color.r << 7;
		gVal = color.g << 7;
		bVal = color.b << 7;
		rVec = 0;
		gVec = 0;
		bVec = 0;
	}

#define ABS(a) (a >= 0 ? a : -a)

	CRGB GeneratorGradientBase::advance(const uint8_t &speed)
	{
		if (rep == 0) {
			CRGB color = gen->nextColor();

			rVec = color.r;
			gVec = color.g;
			bVec = color.b;
			rVec = (rVec << 7) - rVal;
			gVec = (gVec << 7) - gVal;
			bVec = (bVec << 7) - bVal;

			// calculate rep
			rep = (ABS(rVec) >> 7) / speed;
			uint8_t newRep = (ABS(gVec) >> 7) / speed;
			if (newRep > rep) rep = newRep;
			newRep = (ABS(bVec) >> 7) / speed;
			if (newRep > rep) rep = newRep;
			if (rep == 0) rep = 1;

			rVec /= rep;
			gVec /= rep;
			bVec /= rep;
		}

		rVal += rVec;
		gVal += gVec;
		bVal += bVec;
		rep--;

		return CRGB(rVal >> 7, gVal >> 7, bVal >> 7);
	}

#undef ABS

	ColorTypes GeneratorGradientBase::getColorType()
	{
		return gen->getColorType();
	}
}