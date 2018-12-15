#pragma once

#include "ModeBase.hpp"
#include <FastLED.h>

namespace Wordclock
{
	class ModeColorBase : public ModeBase
	{
	protected:
		static CRGB currColor;
	};
}