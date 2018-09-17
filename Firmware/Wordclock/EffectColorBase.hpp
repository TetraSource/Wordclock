#pragma once

#include "EffectBase.hpp"
#include <FastLED.h>

namespace Wordclock
{
	class EffectColorBase : public EffectBase
	{
	protected:
		static CRGB currColor;
	};
}