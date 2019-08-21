#pragma once

#include <Arduino.h>
#include "Config.hpp"
#include "EepromArray.hpp"
#include "EepromVariable.hpp"
#include <FastLED.h>
#include "ModeBase.hpp"
#include "Timer.hpp"

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
#define LAST_INDEX COLOR_PRESET_INDEX + COLOR_PRESET_COUNT * sizeof(CRGB)

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

	/// Access class to the core of the Wordclock.
	class Wordclock
	{
	private:
		static uint8_t times[7];

		const static ModeBase *modes[];
		const static EepromArray<uint8_t, LAYER_COUNT> currModes;

		const static EepromArray<CRGB, COLOR_PRESET_COUNT> presets;
		const static EepromVariable<uint8_t> currPresetIndex;

		static TimerHeap<TIMER_COUNT_TYPE> timers;
		static bool repaintRequest;

	protected:
		static uint8_t getDaysOfMonth();

	public:
		/// Sets the current mode of a layer.
		/// @param layer - the layer the mode is changed on.
		/// @param mode - the new mode to switch to.
		/// @param activateTransition - whether the the index of the mode
		///                             should be displayed briefly before
		///                             activating the mode. This process is
		///                             concurrent.
		static bool setMode(const uint8_t &layer, const uint8_t &mode,
			const bool &activateTransition = false);

		/// Returns the current mode of a layer.
		/// @param layer - the layer the mode is active on.
		/// @returns the currently active mode.
		inline static uint8_t getMode(const uint8_t &layer);

		/// Returns the next mode that that is not active on any other layer.
		/// You should always use this function for setting new modes as
		/// many modes must not run on two layers simultaneous.
		/// @param layer - the layer of the new mode
		/// @param inc - whether the next (inc = true) or previous (inc = false)
		///              mode should be searched.
		/// @param lower - the smallest allowed result.
		/// @param upper - the biggest allowed result.
		static uint8_t getNextMode(const uint8_t &layer, const bool &inc = true,
			const uint8_t &lower = 0, uint8_t upper = 255);

		/// Returns a reference to the current mode.
		/// @returns the reference to the current mode.
		inline static ModeBase *accessMode(const uint8_t &mode);

		/// sets the specified time for the specified time type.
		/// @param timeType - specifies the unit of the time
		///                   (see enum Wordclock::TimeTypes).
		/// @param time - the new time
		/// @returns whether the time was a valid one or not.
		static bool setTime(const TimeTypes &timeType, uint8_t time);

		/// returns the current time of the current time type.
		/// @param timeType - specifies the unit of the time
		///                   (see enum Wordclock::TimeTypes).
		/// @params - the current time
		inline static uint8_t getTime(const TimeTypes &timeType);

		/// returns the valid maximum for the specified time type.
		/// @param timeType - specifies the unit of the time
		///                   (see enum Wordclock::TimeTypes).
		/// @returns the maximal value
		static uint8_t getMaximumTime(const TimeTypes &timeType);

		/// returns the count of seconds since the given time type advanced
		/// last.
		/// @param timeType - the time type that advances last
		///                   (see enum Wordclock::TimeTypes).
		/// @returns the count of seconds.
		static uint32_t getSeconds(const TimeTypes &timeType);

		/// returns the count of seconds that one unit of the given time type
		/// takes (e.g. an hour takes at most 3600s).
		/// The result is influenced by the current time and may cause
		/// different results at different times
		/// (e.g. July has more days than June).
		/// @param timeType - specifies the unit of the time
		///                   (see enum Wordclock::TimeTypes).
		/// @returns the count of seconds.
		static uint32_t getUnitSeconds(const TimeTypes &timeType);

		/// set the color of the color preset at the given index.
		/// @param color - the new color for the preset.
		/// @returns whether the index was a valid one or not.
		static bool setColorPreset(const uint8_t &index, const CRGB &color);

		/// returns the color of the color preset at the given index.
		/// @param index - the index of the color preset.
		/// @returns the color of the preset
		static CRGB getColorPreset(const uint8_t &index);

		/// returns the color of the current color preset.
		/// @param offset - the offset of the chosen index.
		/// @returns the color of the preset.
		inline static CRGB getCurrentPreset();
		inline static CRGB getCurrentPreset(const uint8_t &offset);

		/// sets the index of the current color preset.
		/// @param index - the new index of the color preset.
		/// @returns whether the index was a valid one or not.
		static bool setColorPresetIndex(const uint8_t &index);

		/// returns the index of the current color preset.
		/// @returns the index of the color preset
		inline static uint8_t getColorPresetIndex();

		/// sets the general brightness of all LEDs.
		/// @param brightness - the desired brightness. 255 means full
		///                     brightness and 0 none.
		static void setBrightness(const uint8_t &brightness);

		/// returns the general brightness of all LEDs.
		/// @returns the current brightness. 255 means full brightness and
		///          0 none.
		inline static uint8_t getBrightness();

		/// requests an update of the current LEDs by a call to the paint
		/// function of the current mode. Each mode has to call this
		/// function if it changes its appearance.
		inline static void repaint();

		/// Starts a timer with the given runtime. After it expires
		/// mode->timer(channel) is called with the given channel.
		/// channel is not accessed by the timer system and also has no
		/// fixed usage except channel 255 which cannot used.
		/// Note that TIMER_COUNT_TYPE specifies the maximal count of running
		/// timers at a time (see Config.hpp). However, you can have running
		/// multiple timers with the same callback and channel.
		/// In addition, the timer does not use the RTC but the Arduino
		/// clock. If you need to trigger something at a certain time
		/// you should just use the timer to check repeatedly whether it
		/// is that time or not.
		/// @param callback - the mode to be called when the timer expires.
		/// @param time - the runtime of the timer in milliseconds. This value
		///               must not exceed 2^32 - 8192.
		/// @param channel - value for custom use. 255 is invalid.
		inline static void startTimer(const ModeBase *callback,
			const uint32_t &time, const uint8_t &channel);
		
		/// Cancels the timer with the given callback and channel, if it exists.
		/// Elsewise, this function does not do anything. Note that if there
		/// are multiple timers with the same callback and channel and all is
		/// false, just one of them is cancelled. If all is true, all these
		/// timers are canceled however.
		/// @param callback - the mode of the timer (see startTimer).
		/// @param channel - the channel of the timer (see startTimer).
		/// @param all - whether to delete all or just one of the matching
		///              timers. Note that setting the mode to 255 mathes
		///              all timers that match the callback.
		inline static void cancelTimer(const ModeBase *callback,
			const uint8_t &channel = 255, const bool &all = false);
		
		/// Extends the runtime of the timer with the given callback and
		/// channel, if it exists. Elsewise, the function does not do
		/// anything. Note that if there are multiple timers with the same
		/// callback and channel and all is false, just one of them gets
		/// extended. If all is true, all these timers get extended however.
		/// @param callback - the mode of the timer (see startTimer).
		/// @param time - the runtime the timer is extended by in milliseconds.
		/// @param channel - the channel of the timer (see startTimer).
		/// @param all - whether to extend all or just one of the matching
		///              timers. Note that setting the mode to 255 matches
		///              all timers that match the callback.
		inline static void extendTimer(const ModeBase *callback,
			const uint32_t &time, const uint8_t &channel = 255,
			const bool &all = false);

	friend class Core;
	};

	inline uint8_t Wordclock::getMode(const uint8_t &layer)
	{
		return currModes.get(layer);
	}

	inline ModeBase *Wordclock::accessMode(const uint8_t &index)
	{
		return modes[index];
	}

	inline CRGB Wordclock::getCurrentPreset()
	{
		return presets.get(currPresetIndex.get());
	}

	inline CRGB Wordclock::getCurrentPreset(const uint8_t &offset)
	{
		return presets.get(
			(currPresetIndex.get() + offset) % COLOR_PRESET_COUNT);
	}

	inline uint8_t Wordclock::getColorPresetIndex()
	{
		return currPresetIndex.get();
	}

	inline uint8_t Wordclock::getTime(const TimeTypes &timeType)
	{
		return times[timeType];
	}

	inline uint8_t Wordclock::getBrightness()
	{
		return FastLED.getBrightness();
	}

	inline void Wordclock::repaint()
	{
		DEBUG_OUT("rpr"); // repaint request
		repaintRequest = true;
	}

	inline void Wordclock::startTimer(const ModeBase *callback,
		const uint32_t &time, const uint8_t &channel)
	{
		timers.start(callback, channel, time);
	}

	inline void Wordclock::cancelTimer(const ModeBase *callback,
		const uint8_t &channel, const bool &all)
	{
		timers.cancel(callback, channel, all);
	}

	inline void Wordclock::extendTimer(const ModeBase *callback,
		const uint32_t &time, const uint8_t &channel,
		const bool &all)
	{
		timers.extend(time, callback, channel, all);
	}
}