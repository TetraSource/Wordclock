#pragma once

#include <FastLED.h>
#include "ModeBase.hpp"

// The brightness of each glowing LEDs is divided by 2^DIMING_FACTOR to
// let them appear darker than the shown time
#define DIMING_FACTOR 3

namespace Wordclock
{
	class ModeGlowingBase : public ModeBase
	{
	protected:
		typedef ModeBase super;
		static CRGB **glowing;

		static void setGlow(const uint8_t &x, const uint8_t &y,
			const CRGB &color);
		static void setGlowingPixel(const uint8_t &x, const uint8_t &y,
			CRGB color, const uint8_t &strength, const uint8_t &maxStrength);
		
		static void glowPaint(const uint8_t &radius);
		ModeGlowingBase();
	};

	/// turns on the LEDs around the activated ones using the a dimmed color
	/// to let them appear glowing.
	/// @tparam radius - the radius of the area which LEDs are turned on in
	///                  around activated LEDs.
	template <uint8_t radius = 1>
	class ModeGlowing : public ModeGlowingBase
	{
	protected:
		typedef ModeGlowingBase super;
	public:
		void paint();
	};

	template <uint8_t radius>
	void ModeGlowing<radius>::paint()
	{
		if (isInTransition())
			ModeBase::paint();
		else
			glowPaint(radius + 1);
	}
}