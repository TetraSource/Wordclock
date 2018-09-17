#pragma once

#include <Arduino.h>
#include "Config.hpp"

namespace Wordclock
{
	class Painter;

	/// the base class for all LED modes.
	class EffectBase
	{
		friend class Wordclock;
		friend class Core;

	private:
		static uint32_t updateTime;
		static uint32_t updateThreshold;

	protected:
		/// starts a timer that cause a call to update once expired.
		/// Note that starting a new timer cancels the previous one.
		/// @param time - the time until the timer expires in milliseconds.
		static void startTimer(const uint32_t &time);

		/// cancels the current timer or does nothing if no timer was set.
		static void cancelTimer();

	public:
		/// is called whenever the object of this class is selected.
		virtual void select();

		/// is called before another object, of the same or another class, is selected.
		virtual void deselect();

		/// called whenever the timer, started by startTimer, expires.
		virtual void update();

		/// determines which LED is on and uses which color.
		/// @param painter - the tool for setting the colors of the LEDs.
		virtual void paint(Painter* painter) = 0;
	};
}