
#include "GeneratorGradient.hpp"

namespace Wordclock
{
	uint16_t GeneratorGradientBase::rep = 0;
	int16_t GeneratorGradientBase::rVal = 0;
	int16_t GeneratorGradientBase::gVal = 0;
	int16_t GeneratorGradientBase::bVal = 0;
	int16_t GeneratorGradientBase::rVec = 0;
	int16_t GeneratorGradientBase::gVec = 0;
	int16_t GeneratorGradientBase::bVec = 0;
	GeneratorBase* GeneratorGradientBase::lastGen = nullptr;

	void GeneratorGradientBase::internalSelect(const CRGB &nextColor)
	{
		rVal = nextColor.r << 7;
		gVal = nextColor.g << 7;
		bVal = nextColor.b << 7;
		rep = 0;
	}

#define ABS(a) (a >= 0 ? a : -a)
#define NEG_DIV(neg, n, d) \
	neg = n < 0; \
	if (neg) n = -n; \
	n /= d; \
	if (neg) n = -n;

	bool GeneratorGradientBase::initVec(
		const CRGB &nextColor, const uint8_t &repFactor)
	{
		rVec = (nextColor.r << 7) - rVal;
		gVec = (nextColor.g << 7) - gVal;
		bVec = (nextColor.b << 7) - bVal;

		uint32_t newRep =
			(ABS(rVec) >> 7) + (ABS(gVec) >> 7) + (ABS(bVec) >> 7);
		rep = ((newRep == 0 ? 0 : newRep - 1)*repFactor) / 255 + 1;

		if (rep > 0) {
			bool neg;
			NEG_DIV(neg, rVec, rep)
			NEG_DIV(neg, gVec, rep)
			NEG_DIV(neg, bVec, rep)
			return false;
		}
		return true;
	}
	
	CRGB GeneratorGradientBase::advance()
	{
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
}
