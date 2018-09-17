#pragma once

#include <Arduino.h>

namespace Wordclock
{
	class Marker;

	/// The base class for all configuration modes.
	class ModeBase
	{
	public:
		/// is called whenever the object of this class is selected.
		virtual void select();

		/// is called before another object, of the same or another class, is selected.
		virtual void deselect();

		/// is called whenever the increment (inc = true) / decrement (inc = false) buttons are pressed once.
		virtual void increment(const bool &inc);

		/// is called whenever the entire display needs to be updated.
		/// It marks the LEDs it needs to be on.
		/// @param marker - the object for marking the LEDs.
		virtual void shape(Marker* marker) = 0;
	};
}