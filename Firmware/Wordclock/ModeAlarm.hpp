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

		static const uint8_t checkAlarmInterval;

		uint8_t currAspect : 4;
		bool triggered : 1;
		bool changed : 1;
		EepromVariable<Alarm> alarm;

		static uint8_t incVal(const uint8_t &val, const uint8_t &max,
			const uint8_t &increment, const bool &inc);

		ModeAlarmBase();
		void internalActionButton(const bool &inc, const uint8_t &increment);
		void internalModeButton(const bool &inc, const uint8_t &max);

	public:
		void modeButton(const bool &inc);
		uint32_t timer(const uint8_t &channel);
		void paint();
	};

	class ModeAlarmChangeModeBase : public ModeAlarmBase
	{
	protected:
		typedef ModeAlarmBase super;

		struct ModeData {
			uint8_t mode;
			uint8_t returnTime;
		};

		static const uint8_t maxReturnTime;

		// You can just have one alarm active at a time.
		static ModeAlarmChangeModeBase *activeAlarm;
		static uint8_t returnMode;

		EepromVariable<ModeData> mode;

		static void aboardActiveAlarm(const uint8_t &layer);

		ModeAlarmChangeModeBase(const uint8_t &minMode);
		void internalActionButton(const bool &inc, const uint8_t &minMode,
			const uint8_t &maxMode, const uint8_t &increment);
		uint32_t internalTimer(const uint8_t &channel, const uint8_t &layer);

	public:
		void modeButton(const bool &inc);
		void paint();
	};

	/// Allows to set an alarm. That is at a certain time a certain mode is
	/// selected automatically.
	/// By default this mode indicates whether the alarm is active or disabled.
	/// Press the decrement button to toggle this bahaviour.
	/// If you press the increment button, you can set the settings of the
	/// alarm (listed below). Use the action buttons to increment or decrement
	/// the current selected setting. Press the next or previous mode button,
	/// to select the next or previous setting. Once you would "select" the
	/// setting before the first mode or the one behind the last one, you
	/// return to normal opering mode and the settings are saved.
	/// The available settings are:
	///  * hour of alarm - the pixel at upper left indicate whether you set
	///                    currently a time at am or pm (pixel visible = pm).
	///  * minute of alarm - the pixels at the upper left indicate five minute
	///                      intervals. If increment is divisible by 5, they
	///                      never appear.
	///  * return time n - After n minutes the alarm return to its previous
	///                    mode after being triggered. If n is 0, a cross is
	///                    displayed and we do not return automatically.
	///                    In addition, if another mode is selected externally
	///                    - by the user for instance - then we do not return
	///                    to the previous mode either.
	///  * mode - the mode that is selected if the alarm is triggered.
	/// @tparam layer - the layer the mode is selected on.
	/// @tparam minMode - the smallest mode that can be choosen.
	/// @tparam maxMode - the mode behind the biggest mode that can be choosen.
	/// @tparam increment - the factor minutes are increment by with
	///                     each button press. This value should divide 60.
	template <uint8_t layer = 0, uint8_t minMode = 0, uint8_t maxMode = 255,
		uint8_t increment = 5>
	class ModeAlarm : public ModeAlarmChangeModeBase
	{
	protected:
		typedef ModeAlarmChangeModeBase super;
		EepromVariable<uint8_t> mode;
	public:
		ModeAlarm()
			: ModeAlarmChangeModeBase(minMode)
		{}

		uint32_t timer(const uint8_t &channel)
		{
			return internalTimer(channel, layer);
		}

		void actionButton(const bool &inc)
		{
			internalActionButton(inc, minMode,
				maxMode > MODE_COUNT ? MODE_COUNT : maxMode, increment);
		}
	};
}