
#include "GeneratorGradient.hpp"

namespace Wordclock
{
	uint16_t GeneratorGradientBase::rep = 0;
	int16_t GeneratorGradientBase::rVal = 0;
	int16_t GeneratorGradientBase::gVal = 0;
	int16_t GeneratorGradientBase::bVal = 0;
	GeneratorBase* GeneratorGradientBase::lastGen = nullptr;

	GeneratorGradientBase::GeneratorGradientBase(GeneratorBase* generator)
	{
		gen = generator;
		select();
	}

#define ABS(a) (a >= 0 ? a : -a)
#define NEG_DIV(neg, n, d) \
	neg = n < 0; \
	if (neg) n = -n; \
	n /= d; \
	if (neg) n = -n;

	CRGB GeneratorGradientBase::advance(const uint8_t &repFactor)
	{
		if (lastGen != gen)
			select();
		lastGen = gen;

		static int16_t rVec = 0, gVec = 0, bVec = 0;
		while (rep == 0) {
			CRGB color = gen->nextColor();
			rVec = (color.r << 7) - rVal;
			gVec = (color.g << 7) - gVal;
			bVec = (color.b << 7) - bVal;

			uint32_t newRep =
			 (ABS(rVec) >> 7) + (ABS(gVec) >> 7) + (ABS(bVec) >> 7);
			rep = ((newRep == 0 ? 0 : newRep - 1)*repFactor) / 255 + 1;

			if (rep > 0) {
				bool neg;
				NEG_DIV(neg, rVec, rep)
				NEG_DIV(neg, gVec, rep)
				NEG_DIV(neg, bVec, rep)
				break;
			}
		}

		rVal += rVec;
		if (rVal < 0)
			rVal = rVec < 0 ? 0 : 0x7fff;
		gVal += gVec;
		if (gVal < 0)
			gVal = gVec < 0 ? 0 : 0x7fff;
		bVal += bVec;
		if (bVal < 0)
			bVal = bVec < 0 ? 0 : 0x7fff;
		rep--;
		return CRGB(rVal >> 7, gVal >> 7, bVal >> 7);
	}

#undef ABS
#undef ABS_DIV

	ColorTypes GeneratorGradientBase::getColorType()
	{
		return gen->getColorType();
	}

	void GeneratorGradientBase::select()
	{
		CRGB color = gen->nextColor();
		rVal = color.r << 7;
		gVal = color.g << 7;
		bVal = color.b << 7;
		rep = 0;
	}
}
