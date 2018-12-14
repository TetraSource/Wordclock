
#include "EffectGlowing.hpp"

namespace Wordclock
{
	uint8_t EffectGlowingBase::glowing[HEIGHT][WIDTH] = {};

	void EffectGlowingBase::setGlow(const uint8_t &x, const uint8_t &y, const uint8_t &glow)
	{
		if (glowing[y][x] < glow)
			glowing[y][x] = glow;
	}
}