#pragma once

#include "EepromVariable.hpp"
#include "ModeBase.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	class ModeSuspendedBase : public ModeBase
	{
	protected:
		typedef ModeBase super;
		EepromVariable<bool> suspended;
	public:
		ModeSuspendedBase();
		uint32_t timer(const uint8_t &channel);
		void paint();
	};

	/// Paints the whole screen black in order to lock it or turn it off.
	/// Once the increment button is pressed, this is temporarily changed.
	/// Once the decrement button is pressed, this behaviour gets inverted.
	/// @tparam lock - the time the display is locked in milliseconds.
	template <uint32_t lock = 1000>
	class ModeSuspend : public ModeSuspendedBase
	{
	protected:
		typedef ModeSuspendedBase super;
		
	public:
		void actionButton(const bool &inc)
		{
			suspended.set(!suspended.get());
			Wordclock::repaint();
			if (inc)
				Wordclock::startTimer(this, lock, 0);
		}
	};
}