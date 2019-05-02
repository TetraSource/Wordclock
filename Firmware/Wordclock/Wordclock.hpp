#pragma once

#include <Arduino.h>
#include "Config.hpp"
#include <FastLED.h>
#include "ModeBase.hpp"

#ifdef DEBUG
#define DEBUG_OUT(s) Serial.println(s)
#define DEBUG_RGB(c) Serial.print(c.r); \
Serial.print("\t"); Serial.print(c.g); \
Serial.print("\t"); Serial.println(c.b)
#else
#define DEBUG_OUT(s)
#define DEBUG_RGB(c)
#endif

// EEPROM memory
#define MODE_INDEX 0
#define COLOR_PRESET_INDEX_INDEX 1
#define BRIGHTNESS_INDEX 2
#define COLOR_PRESET_INDEX 3
#define ALARM_INDEX COLOR_PRESET_INDEX + COLOR_PRESET_COUNT * sizeof(CRGB)
#define LAST_INDEX ALARM_INDEX + ALARM_COUNT * sizeof(AlarmTime)

// allows to specify a collection of directions
// e.g DIR_ITEM(Center, 0) just contains the center
//     DIR_ITEM(Right, DIR_ITEM(Left, 0)) contains the left and right side
#define DIR_ITEM(dir, list) ((1 << dir) | list)

namespace Wordclock
{
	/// specifies the direction for pixel movements.
	enum Directions
	{
		Top,    /// from the top to the bottom
		Bottom, /// from the bottom to the top
		Left,   /// from the left to the right
		Right,  /// from the right to the left
		Center, /// from the center to the border
		Border, /// from the border to the center
	};

	/// specifies the unit of the time.
	enum TimeTypes
	{
		Seconds,  /// time in seconds.
		Minutes,  /// time in minutes.
		Hours,    /// time in hours.
		Weekdays, /// time in weekdays.
		Days,     /// time in days of month.
		Months,   /// time in months.
		Years,    /// time in years as of 2000.
	};

	/// used to set and retrieve alarms.
	struct AlarmTime
	{
		uint8_t weekday;
		uint8_t hour;
		uint8_t minute;

		AlarmTime();

		AlarmTime(
			
			const uint8_t &hour,
			const uint8_t &minute);

		AlarmTime(const AlarmTime &time);

		void AlarmTime::setTo(
			const uint8_t &weekday,
			const uint8_t &hour,
			const uint8_t &minute);

		void AlarmTime::setTo(const AlarmTime &time);
	};

	typedef bool(*AlarmChecker)(const AlarmTime &time);

	/// Access class to the core of the Wordclock.
	class Wordclock
	{
		friend class Core;

	private:
#if ALARM_COUNT > 0
		// alarms[i].weekdays is a bit mask
		static AlarmTime alarms[ALARM_COUNT];
		// currAlarm.weekdays is the id of the day (id 7 = no alarm set).
		static AlarmTime currAlarm;
		static bool alarm;
#endif
		static uint8_t times[7];
		static bool saveTimeRequest;

		static CRGB presets[COLOR_PRESET_COUNT];
		static uint8_t currPresetIndex;

		static ModeBase *modes[MODE_COUNT];
		static uint8_t currMode;

		static bool repaintRequest;

		static void saveTime();
		static void loadTime();

#if ALARM_COUNT > 0
		static void chooseSooner(AlarmTime time);
		static void loadNextAlarm();
#endif

		static bool setMode(const uint8_t &mode);

	protected:
		inline static uint8_t getDaysOfMonth();

	public:
		/// sets the specified time for the specified time type.
		/// @param timeType - specifies the unit of the time
		///                   (see enum Wordclock::TimeTypes).
		/// @param time - the new time
		/// @returns whether the time was a valid one or not.
		static bool setTime(const uint8_t &timeType, uint8_t time);

		/// returns the current time of the current time type.
		/// @param timeType - specifies the unit of the time
		///                   (see enum Wordclock::TimeTypes).
		/// @params - the current time
		static uint8_t getTime(const uint8_t &timeType);

		/// returns the valid maximum for the specified time type.
		/// @param timeType - specifies the unit of the time
		///                   (see enum Wordclock::TimeTypes).
		/// @returns the maximal value
		static uint8_t getMaximumTime(const uint8_t &timeType);

		/// returns the count of seconds since the given time type advanced
		/// last.
		/// @param timeType - the time type that advances last
		///                   (see enum Wordclock::TimeTypes).
		/// @returns the count of seconds.
		static uint32_t getSeconds(const uint8_t &timeType);

		/// returns the count of seconds that one unit of the given time type
		/// takes (e.g. an hour takes at most 3600s).
		/// The result is influenced by the current time and may cause
		/// different results at different times
		/// (e.g. the count of days in Juli in higher than in June).
		/// @param timeType - specifies the unit of the time
		///                   (see enum Wordclock::TimeTypes).
		/// @returns the count of seconds.
		static uint32_t getUnitSeconds(const uint8_t &timeType);

		/// set the color of the color preset at the given index.
		/// @param color - the new color for the preset.
		/// @returns whether the index was a valid one or not.
		static bool setColorPreset(const uint8_t &index, const CRGB &color);

		/// returns the color of the color preset at the given index.
		/// @param index - the index of the color preset.
		/// @returns the color of the preset
		static CRGB getColorPreset(const uint8_t &index);

		/// returns the color of the current color preset.
		/// @returns the color of the preset.
		static CRGB getCurrentPreset();

		/// returns the color of the current color preset.
		/// @param offset - the offset of the chosen index.
		/// @returns the color of the preset.
		static CRGB getCurrentPreset(const uint8_t &offset);

		/// sets the index of the current color preset.
		/// @param index - the new index of the color preset.
		/// @returns whether the index was a valid one or not.
		static bool setColorPresetIndex(const uint8_t &index);

		/// returns the index of the current color preset.
		/// @returns the index of the color preset
		static uint8_t getColorPresetIndex();

#if ALARM_COUNT > 0
		/// sets an alarm for the given time.
		/// @param time - the time the alarm shall be activated at.
		///               Note that the value time.minute is rounded to the
		///               next five minute interval.
		///               Additionally, the value time.weekday can either be a
		///               single day or a bit mask representing a set of days.
		///               In this case the eighth bit of the mask has to be
		///               set to 1.
		/// @returns whether the alarm could be set successfully or not.
		///          Note, that there is an internal limit of alarms.
		static bool addAlarm(AlarmTime time);

		/// allows to get and remove set alarms.
		/// @param checker - the function that is called with all existing
		///                  alarms. If it return true, the alarm will be
		///                  removed.
		static void listAlarms(AlarmChecker checker);

		/// sets whether the alarm is active or not.
		/// @param newAlarm - the new state of the alarm
		static void setAlarm(const bool &newAlarm);

		/// returns whether the alarm is active or not.
		/// @returns the state of the alarm
		static bool getAlarm();
#endif

		/// sets the general brightness of all LEDs.
		/// @param brightness - the desired brightness. 255 means full
		///                     brightness and 0 none.
		static void setBrightness(const uint8_t &brightness);

		/// returns the general brightness of all LEDs.
		/// @returns the current brightness. 255 means full brightness and
		///          0 none.
		static uint8_t getBrightness();

		/// requests an update of the current LEDs by a call to the paint
		/// function of the current mode.
		static void repaint();
	};
}
