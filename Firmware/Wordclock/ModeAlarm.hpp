#pragma once

#include "EepromVariable.hpp"
#include "ModeBaseInterval.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	class ModeAlarmBase : public ModeBase
	{
	protected:
		typedef ModeBase super;

		struct Alarm
		{
			uint8_t minute : 6;
			uint8_t hour : 5;
			bool active : 1;
		};

		uint8_t currAspect : 4;
		bool triggered : 1;
		EepromVariable<Alarm> alarm;

		ModeAlarmBase();
		uint8_t incVal(uint8_t val, const uint8_t &max,
			const uint8_t &increment, const bool &inc);
		void internalActionButton(const bool &inc, const int8_t &increment);
		void internalModeButton(const bool &inc, const uint8_t &max);

	public:
		void modeButton(const bool &inc);
		uint32_t timer(const uint8_t &channel);
		void paint();
		virtual void trigger();
	};

	/// Allows to set an alarm. That is at a certain time a certain mode is
	/// selected automatically.
	/// By default this mode indicates whether the alarm is active or disabled.
	/// Press the decrement button to toggle this bahaviour.
	/// If you press the increment button, you can set the hour, minute and
	/// mode to be selected using the increment and decrement button.
	/// The mode increment and decrement buttons cannot be used to change
	/// the mode during this process. Instead, you can use the mode increment
	/// button to change the aspect - hour, minute or mode - you are setting.
	/// The mode decrement button is used to confirm the configuration and to
	/// return to normal operation mode. While setting the minutes
	/// 0 to 4 LEDs on the upper left corner indicate the exact minute by
	/// showing five minute intervals. Same holds true for hours, however,
	/// there is just one LED turned on, if the hours are greater than 12.
	/// @tparam layer - the layer the mode is selected on.
	/// @tparam minMode - the smallest mode that can be choosen.
	/// @tparam maxMode - the mode behind the biggest mode that can be choosen.
	/// @tparam increment - the factor minutes are increment by with
	///                     each button press. This value should divide 60.
	template <uint8_t layer = 0,
		uint8_t minMode = 0, uint8_t maxMode = 255, uint8_t increment = 5>
	class ModeAlarm : public ModeAlarmBase
	{
	protected:
		typedef ModeAlarmBase super;
		EepromVariable<uint8_t> mode;
		
	public:
		ModeAlarm();
		void actionButton(const bool &inc);
		void modeButton(const bool &inc);
		void trigger();
		void paint();
	};

	template <uint8_t layer, uint8_t minMode, uint8_t maxMode, uint8_t increment>
	ModeAlarm<layer, minMode, maxMode, increment>::ModeAlarm()
		: ModeAlarmBase()
	{
		mode = EepromVariable<uint8_t>();
		mode.setDefault(minMode);
	}

	template <uint8_t layer, uint8_t minMode, uint8_t maxMode, uint8_t increment>
	void ModeAlarm<layer, minMode, maxMode, increment>::actionButton(
		const bool &inc)
	{
		Wordclock::repaint();
		if (currAspect == 3) {
			mode.set(incVal(mode.get() - minMode,
				(maxMode > MODE_COUNT ? MODE_COUNT : maxMode) - minMode,
				1, inc) + minMode);
		}
		else
			internalActionButton(inc, increment);
	}

	template <uint8_t layer, uint8_t minMode, uint8_t maxMode, uint8_t increment>
	void ModeAlarm<layer, minMode, maxMode, increment>::modeButton(
		const bool &inc)
	{
		internalModeButton(inc, 3);
	}

	template <uint8_t layer, uint8_t minMode, uint8_t maxMode, uint8_t increment>
	void ModeAlarm<layer, minMode, maxMode, increment>::paint()
	{
		super::paint();
		if (!isInTransition() && currAspect == 3)
			Utilities::printNumber(mode.get() + 1);
	}

	template <uint8_t layer, uint8_t minMode, uint8_t maxMode, uint8_t increment>
	void ModeAlarm<layer, minMode, maxMode, increment>::trigger()
	{
		Wordclock::setMode(layer, mode.get());
	}
}