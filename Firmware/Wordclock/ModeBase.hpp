#pragma once

#include <Arduino.h>
#include "Painter.hpp"

namespace Wordclock
{
	class Core;

	/// The base class for all configuration modes.
	class ModeBase
	{
		friend class Core;

	protected:
		// may be static since just one mode can be selected at one time.
		static uint32_t updateTime;
		static uint32_t updateThreshold;

		/// starts a timer that cause a call to the timer method once expired.
		/// Note that starting a new timer cancels the previous one.
		/// @param time - the time until the timer expires in milliseconds.
		static void startTimer(const uint32_t &time);

		/// cancels the current timer if set.
		static void cancelTimer();

	public:
		/// is called whenever the object of this class is selected.
		virtual void select();

		/// is called before another object, of the same or another class, is selected.
		virtual void deselect();

		/// is called whenever the increment (inc = true) / decrement (inc = false) buttons are pressed once.
		virtual void increment(const bool &inc);

		/// called whenever a timer started by startTimer elapses.
		virtual void timer();

		/// is called whenever the entire display needs to be repainted.
		/// Please use the methods provided by the Painter class to do so.
		/// The current color of the painter is set to the current color
		/// preset and all LEDs are painted black always, before this method
		/// is called.
		virtual void paint() = 0;
	};
}
