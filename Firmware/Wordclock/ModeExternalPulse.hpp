#pragma once

#include "ModeBase.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	/// Powers the specified pin with a certain pulse with a fixed time low
	/// and a fixed time high.
	/// @tparam pin - the Arduino pin to be powered.
	/// @tparam timeOn - the time the pulse is high after it was low.
	///                  This value must not be 0.
	/// @tparam timeOff - the time the pulse is low after it was high.
	///                   Set this value to 0 to have a constantly high signal.
	template <uint8_t pin, uint32_t timeOn, uint32_t timeOff>
	class ModeExternalPulse : public ModeBase
	{
	protected:
		typedef ModeBase super;
		bool active;

	public:
		ModeExternalPulse()
			: ModeBase()
		{
			active = false;
		}

		void select()
		{
			if (isInTransition())
				ModeBase::select();
			else {
				if (timeOff != 0)
					Wordclock::startTimer(this, timeOn, 0);
				digitalWrite(pin, true);
				active = true;
			}
		}

		void deselect()
		{
			if (isInTransition())
				ModeBase::select();
			else {
				if (timeOff != 0)
					Wordclock::cancelTimer(this, 0);
				digitalWrite(pin, false);
				active = false;
			}
		}

		uint32_t timer(const uint8_t &channel)
		{
			if (channel != 0)
				return ModeBase::timer(channel);
			if (timeOff != 0) {
				active = !active;
				digitalWrite(pin, active);
				return active ? timeOn : timeOff;
			}
			return 0;
		}
	};
}