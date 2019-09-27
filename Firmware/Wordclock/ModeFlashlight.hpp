#pragma once

#include "ModeBase.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	class ModeFlashlightBase : public ModeBase
	{
	protected:
		typedef ModeBase super;
		CRGB currColor;
		bool visible;
	public:
		void deselect();
		void paint();
	};

	/// Paints the whole screen of its layer in one color. After a certain
	/// time it takes an optional break and paints the whole screen in the
	/// the same or another color - depending on the color generator.
	/// @tparam Generator - the color generator that generates the colors
	///                     used to light the screen.
	/// @tparam timeOn - the time on color is shown on the display.
	///                  This value must not be 0.
	/// @tparam timeOff - the time of the interruptions between each flash.
	///                   Set this value to 0 to never have any interruptions.
	template <class Generator, uint32_t timeOn, uint32_t timeOff = 0>
	class ModeFlashlight : public ModeFlashlightBase
	{
	protected:
		typedef ModeFlashlightBase super;
		Generator gen;

	public:
		ModeFlashlight()
			: ModeFlashlightBase()
		{
			gen = Generator();
		}

		void select()
		{
			if (isInTransition()) {
				ModeBase::select();
				return;
			}
			visible = true;
			currColor = gen.next();
			Wordclock::startTimer(this, timeOn, 0);
		}

		uint32_t timer(const uint8_t &channel)
		{
			if (channel != 0)
				return ModeBase::timer(channel);
			if (timeOff != 0)
				visible = !visible;
			if (visible)
				currColor = gen.next();
			Wordclock::repaint();
			return visible ? timeOn : timeOff;
		}
	};
}