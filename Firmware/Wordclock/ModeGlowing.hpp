#pragma once

#include <FastLED.h>
#include "ModeBase.hpp"

// The brightness of each glowing LEDs is divided by 2^DIMING_FACTOR to
// let them appear darker than the shown time
#define DIMING_FACTOR 3

namespace Wordclock
{
	class ModeGlowingUtilities
	{
	protected:
		typedef ModeBase super;
		static CRGB **glowing;

		static void setGlow(const uint8_t &x, const uint8_t &y,
			const CRGB &color);
		static void setGlowingPixel(const uint8_t &x, const uint8_t &y,
			CRGB color, const uint8_t &intensity, const uint8_t &maxIntensity);
		
	public:
		static void glowPaint(const uint8_t &radius);
	};

	/// turns on the LEDs around the activated ones using the a dimmed color
	/// to let them appear glowing.
	/// @tparam radius - the radius of the area which LEDs are turned on in
	///                  around activated LEDs.
	template <uint8_t radius = 1>
	class ModeGlowing : public ModeBase
	{
	protected:
		typedef ModeBase super;

	public:
		void paint()
		{
			if (isInTransition())
				ModeBase::paint();
			else
				ModeGlowingUtilities::glowPaint(radius + 1);
		}
	};
}