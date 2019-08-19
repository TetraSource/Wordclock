#pragma once

#include <Arduino.h>
#include "Painter.hpp"

namespace Wordclock
{
	class Core;

	class ModeBase
	{
	protected:
		const static uint8_t transitionChannel;
		const static uint16_t transitionTime;
		uint8_t state;
		ModeBase();

		inline bool setInTransition(const bool &transition);
		inline bool isInTransition();

		inline void setState(const uint8_t &bit, const bool &value);
		inline bool getState(const uint8_t &bit);

	public:
		/// is called whenever the object of this class is selected.
		virtual void select();

		/// is called before another object, of the same or another class,
		/// is selected. In this case a timer is also canceled if running.
		/// Note that if you don't cancel all timers your mode started, the
		/// timer function of your mode gets called without calling select
		/// beforehand.
		virtual void deselect();

		/// is called whenever one of the action buttons is pressed.
		/// @param inc - indicates whether the mode for
		///              incrementing/confirming/... or
		///              decrementing/canceling/... was pressed.
		virtual void actionButton(const bool &inc);

		/// is called whenever one of the mode buttons is pressed.
		/// @param inc - indicates whether the next or previous mode is
		///              supposed to be selected.
		virtual void modeButton(const bool &inc);

		/// called whenever a timer started by Wordclock::startTimer elapses.
		/// You may start, cancel or extend any timer within this function.
		/// @param channel - the channel value that was passed to
		///                  Wordclock::startTimer.
		/// @returns the value n. If n == 0, is the timer canceled. Otherwise,
		///          it is extended by n milliseconds.
		virtual uint32_t timer(const uint8_t &channel);

		/// is called whenever the entire display needs to be repainted.
		/// Please use the methods provided by the Painter class to do so.
		/// The current color of the painter is set to the current color
		/// preset and all LEDs are painted black always, before this method
		/// is called.
		/// Also note that if your mode needs to be repainted you must not call
		/// this method directly. Instead you must call Wordclock::repaint()
		/// to request the core to call this method.
		virtual void paint();
	
	friend class Core;
	};

	inline void ModeBase::setState(const uint8_t &bit, const bool &value)
	{
		state = value ? state | (1 << bit) : state & (1 << bit);
	}

	inline bool ModeBase::getState(const uint8_t &bit)
	{
		return state & (1 << bit);
	}

	inline bool ModeBase::setInTransition(const bool &transition)
	{
		setState(0, transition);
	}

	inline bool ModeBase::isInTransition()
	{
		return getState(0);
	}
}
