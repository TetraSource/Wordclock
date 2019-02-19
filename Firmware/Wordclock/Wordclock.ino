
#include <EEPROM.h>
#include <FastLED.h>
#include "ModeBase.hpp"
#include "Painter.hpp"
#include "Wordclock.hpp"
#ifdef WDT
#include <avr/wdt.h>
#endif
#ifndef INTERNAL_TIME
#include <Wire.h>
#endif
#define IMPORT_MODES
#include "Config.hpp"

#if ALARM_COUNT == 0
#undef STOP_ALARM_ON_PRESS
#undef ALARM_PULSE
#endif

namespace Wordclock
{
	const uint8_t maxTime[7] = {
		60,
		60,
		24,
		7,
		31,
		12,
		255,
	};

#if ALARM_COUNT > 0
	AlarmTime::AlarmTime()
	{
        setTo(7, 0, 0);
    }

	AlarmTime::AlarmTime(
		const uint8_t &weekday,
		const uint8_t &hour,
		const uint8_t &minute)
	{
        setTo(weekday, hour, minute);
    }

	AlarmTime::AlarmTime(const AlarmTime &time)
	{
        setTo(time.weekday, time.hour, time.minute);
    }

    void AlarmTime::setTo(
        const uint8_t &weekday,
        const uint8_t &hour,
        const uint8_t &minute)
    {
        setTo(weekday, hour, minute);
    }

	void AlarmTime::setTo(const AlarmTime &time)
	{
		weekday = time.weekday;
		hour = time.hour;
		minute = time.minute;
	}

    AlarmTime Wordclock::alarms[ALARM_COUNT] = {};
    AlarmTime Wordclock::currAlarm = { 0, 0, 0 };
    bool Wordclock::alarm = false;
#endif

    uint8_t Wordclock::times[7] = {};
    bool Wordclock::saveTimeRequest = false;

	CRGB Wordclock::presets[COLOR_PRESET_COUNT] = {};
	uint8_t Wordclock::currPresetIndex = 0;

    // Wordclock::modes initialized in Config.hpp
	uint8_t Wordclock::currMode = 0;

    bool Wordclock::repaintRequest = false;

	void Wordclock::saveTime()
	{
#ifndef INTERNAL_TIME
		// Write to RTC
		Wire.beginTransmission(0x68);
		Wire.write(0x0);
		uint8_t time;
		for (uint8_t i = 0; i <= Years; i++) {
			Wire.write(((times[i] / 10) << 4) + (times[i] % 10));
		}
		Wire.write(0x0);
		Wire.endTransmission();
#endif
	}

	void Wordclock::loadTime()
	{
#ifndef INTERNAL_TIME
		// Read from RTC
		Wire.beginTransmission(0x68);
		Wire.write(0x0);
		Wire.endTransmission();
		Wire.requestFrom(0x68, 7);
		for (uint8_t n = 0; n <= Years; n++) {
			uint8_t val = Wire.read();
			times[n] = (((val >> 4) * 10) + (val % 16));
		}
		times[Seconds] &= 0x7f;
		times[Hours] &= 0x3f;
#ifdef DEBUG
		Serial.print("rt "); // read time
		for (uint8_t n = 0; n <= Years; n++) {
			Serial.print(times[n]);
			Serial.print(' ');
		}
		Serial.println();
#endif
#endif
	}

	bool Wordclock::setMode(const uint8_t &index)
	{
		if (index >= MODE_COUNT)
			return false;
		if (index == currMode)
			return true;

		modes[currMode]->deselect();
		currMode = index;
		EEPROM.write(MODE_INDEX, currMode);
		modes[index]->select();
		repaint();
		return true;
	}

	bool Wordclock::setColorPreset(const uint8_t &index, const CRGB &color)
	{
		if (index >= COLOR_PRESET_COUNT)
			return false;
        presets[index] = color;
		EEPROM.put(COLOR_PRESET_INDEX + index * sizeof(CRGB), color);
		repaint();
		return true;
	}

	CRGB Wordclock::getColorPreset(const uint8_t &index)
	{
		if (index >= COLOR_PRESET_COUNT)
			return CRGB(CRGB::Black);
		return presets[index];
	}

	CRGB Wordclock::getCurrentPreset()
	{
		return presets[currPresetIndex];
	}

	CRGB Wordclock::getCurrentPreset(const uint8_t &offset)
	{
		return presets[(currPresetIndex + offset) % COLOR_PRESET_COUNT];
	}

	bool Wordclock::setColorPresetIndex(const uint8_t &index)
	{
		if (index >= COLOR_PRESET_COUNT)
			return false;
		if (index != currPresetIndex) {
            currPresetIndex = index;
            EEPROM.write(COLOR_PRESET_INDEX_INDEX, currPresetIndex);
            repaint();
        }
		return true;
	}

	uint8_t Wordclock::getColorPresetIndex()
	{
		return currPresetIndex;
	}

	uint8_t Wordclock::getTime(const uint8_t &timeType)
	{
		return times[timeType];
	}

	bool Wordclock::setTime(const uint8_t &timeType, uint8_t time)
	{
		if (timeType > Years)
			return false;

		uint8_t maxTime = getMaximumTime(timeType);
		if (time >= maxTime)
			time = maxTime;
		if (time == times[timeType])
			return true;

		times[timeType] = time;
		saveTimeRequest = true;
		return true;
	}

	inline uint8_t Wordclock::getDaysOfMonth()
	{
		if (times[5] == 1)
			return times[6] % 4 == 0 ? 29 : 28;
		else
			return times[5] < 6 ? 31 - times[5] % 2 : 30 + times[5] % 2;
	}

	uint8_t Wordclock::getMaximumTime(const uint8_t &timeType)
	{
		if (timeType >= Years)
			return 0;
		if (timeType == Days)
			return getDaysOfMonth();

		return maxTime[timeType];
	}

	uint32_t Wordclock::getSeconds(const uint8_t &timeType)
	{
		if (timeType > Years)
			return 0;

		uint32_t secs = 0;
		for (uint8_t i = 0; i <= timeType; i++) {
			if (i != Days)
				secs += times[i] * getUnitSeconds(i);
		}
		return secs;
	}

	uint32_t Wordclock::getUnitSeconds(const uint8_t &timeType)
	{
		if (timeType == Years)
			return times[Years] % 4 == 0 ? 182145024 : 181647360;
		else if (timeType == Months)
			return getDaysOfMonth() * 497664;
		else if (timeType == Days || timeType == Weekdays)
			return 86400;
		else if (timeType == Hours)
			return 3600;
		else if (timeType == Minutes)
			return 60;
		else if (timeType == Seconds)
			return 1;
		else
			return 0;
	}

#if ALARM_COUNT > 0
    // checks whether the newAlarm fires before the current one and sets
    // the current one to newAlarm if it is the case.
    // Note that newAlarm.weekday is supposed to be a bit mask.
	void Wordclock::chooseSooner(AlarmTime newAlarm)
	{
		uint8_t weekdays = newAlarm.weekday;

        if (currAlarm.weekday >= 7) {
            currAlarm.setTo(newAlarm);
            return;
        }
		for (newAlarm.weekday = 0; newAlarm.weekday < 7; newAlarm.weekday++) {
			if (weekdays & (1 << newAlarm.weekday)) {
				int8_t weekdaysNew = newAlarm.weekday;
				int8_t weekdaysOld = currAlarm.weekday;
				weekdaysNew -= times[Weekdays];
				weekdaysOld -= times[Weekdays];

				if (weekdaysNew < 0)
					weekdaysNew += 7;
				if (weekdaysOld < 0)
					weekdaysOld += 7;

				if (weekdaysNew == 0 && (
					newAlarm.hour < times[Hours] ||
					newAlarm.hour == times[Hours] &&
					newAlarm.minute <= times[Minutes]))
					weekdaysNew = 7;
				if (weekdaysOld == 0 && (currAlarm.hour < times[Hours] ||
                    currAlarm.hour == times[Hours] &&
					currAlarm.minute <= times[Minutes]))
					weekdaysOld = 7;

				if (weekdaysNew < weekdaysOld ||
					(weekdaysNew == weekdaysOld &&
					(newAlarm.hour < currAlarm.hour ||
						newAlarm.hour == currAlarm.hour &&
						newAlarm.minute < currAlarm.minute)))
				{
                    currAlarm.setTo(newAlarm);
				}
			}
		}
	}

	void Wordclock::loadNextAlarm()
	{
		for (uint8_t i = 0; i < ALARM_COUNT; i++) {
			chooseSooner(alarms[i]);
		}
	}

	bool Wordclock::addAlarm(AlarmTime time)
	{
		if (time.weekday & 0x80) {
			time.weekday &= 0x7f;
		}
		else if (time.weekday < 0x7) {
			time.weekday = 1 << time.weekday;
		}
		else {
			return false;
		}
		if (time.hour >= 24) {
			return false;
		}
		if (time.minute >= 60) {
			return false;
		}

        // Try to stack the alarm with other ones.
        uint8_t index = ALARM_INDEX;
		for (uint8_t i = 0; i < ALARM_COUNT; i++) {
			if (alarms[i].hour == time.hour &&
            alarms[i].minute == time.minute) {
				time.weekday |= alarms[i].weekday;
                alarms[i].weekday = time.weekday;
                EEPROM.put(index, time);
                chooseSooner(alarms[i]);
				return true;
			}
            index += sizeof(AlarmTime);
		}

        // Otherwise, add the alarm directly.
        index = ALARM_INDEX;
		for (uint8_t i = 0; i < ALARM_COUNT; i++) {
			if (alarms[i].weekday == 0x00) {
                alarms[i].setTo(time);
                EEPROM.put(index, time);
                chooseSooner(alarms[i]);
				return true;
			}
            index += sizeof(AlarmTime);
		}
		return false;
	}

	void Wordclock::listAlarms(AlarmChecker checker)
	{
		bool nextAlarmDeleted = false;
		uint8_t index = ALARM_INDEX;
		for (uint8_t i = 0; i < ALARM_COUNT; i++) {
			AlarmTime t = alarms[i];
			uint8_t weekdays = t.weekday;
			for (uint8_t weekday = 0; weekday < 7; weekday++) {
				if (weekdays & (1 << weekday)) {
					t.weekday = weekday;
					if ((*checker)(t)) {
						nextAlarmDeleted = nextAlarmDeleted ||
							weekday == currAlarm.weekday &&
							t.hour == currAlarm.hour &&
							t.minute == currAlarm.minute;
						weekdays &= ~(1 << weekday);
						t.weekday = weekdays;
						EEPROM.put(index, t);
					}
				}
			}
			index += sizeof(AlarmTime);
		}
		if (nextAlarmDeleted) {
			currAlarm.weekday = 7;
			loadNextAlarm();
		}
	}

	void Wordclock::setAlarm(const bool &newAlarm)
	{
		if (alarm != newAlarm) {
			alarm = newAlarm;
#ifdef ALARM_PULSE
			if (!alarm)
				digitalWrite(ALARM_PIN, false);
#else
			digitalWrite(ALARM_PIN, alarm);
#endif
		}
	}

	bool Wordclock::getAlarm()
	{
		return alarm;
	}
#endif

	void Wordclock::setBrightness(const uint8_t &brightness)
	{
		if (brightness != FastLED.getBrightness()) {
			FastLED.setBrightness(brightness);
			EEPROM.put(BRIGHTNESS_INDEX, brightness);
			repaint();
		}
	}

	uint8_t Wordclock::getBrightness()
	{
		return FastLED.getBrightness();
	}

	void Wordclock::repaint()
	{
		DEBUG_OUT("rpr"); // repaint request
		repaintRequest = true;
	}

	// CLASS END //

	// CORE IMPLEMENTATION //

	class Core
	{
	public:
		inline static void setup();
		inline static void loop();
	};

	inline void Core::setup()
	{
#if defined(DEBUG) || defined(DEBUG_DISPLAY) || defined(DEB)
		Serial.begin(BAUD_RATE);
#endif
		DEBUG_OUT("s"); // setup
		
		randomSeed(analogRead(FREE_ANALOG_PIN));

		// init LEDs
		FastLED.addLeds<CHIPSET, LED_DATA_PIN, COLOR_ORDER>(
			Painter::leds, LED_COUNT);
		FastLED.clear();

        // init pins
		pinMode(INC_MODE_PIN, INPUT_PULLUP);
		pinMode(DEC_MODE_PIN, INPUT_PULLUP);
		pinMode(NEXT_MODE_PIN, INPUT_PULLUP);
		pinMode(PREV_MODE_PIN, INPUT_PULLUP);
		pinMode(ALARM_PIN, OUTPUT);
#ifndef INTERNAL_TIME
		Wire.begin();
#endif
#ifndef INTERNAL_PULSE
		pinMode(RTC_PULSE_PIN, INPUT);
		digitalWrite(RTC_PULSE_PIN, HIGH);
#endif
#ifdef SHOW_PULSE
		pinMode(LED_BUILTIN, OUTPUT);
#endif

        for (uint8_t i = 0; i <= Years; i++) {
			Wordclock::times[i] = 0;
		}
#if ALARM_COUNT > 0
		Wordclock::currAlarm.weekday = 7;
#endif

#if defined(RESET_EEPROM) || defined(RESET_EEPROM_SAFELY)
		// save data
        Wordclock::currMode = 0;
		EEPROM.write(MODE_INDEX, Wordclock::currMode);
        Wordclock::currPresetIndex = 0;
		EEPROM.write(COLOR_PRESET_INDEX_INDEX, Wordclock::currPresetIndex);
        FastLED.setBrightness(255);
		EEPROM.write(BRIGHTNESS_INDEX, FastLED.getBrightness());
		{
			CRGB defaultPresets[COLOR_PRESET_COUNT] = { DEFAULT_COLOR_PRESETS };

            uint8_t index = COLOR_PRESET_INDEX;
			for (uint8_t i = 0; i < COLOR_PRESET_COUNT; i++) {
                CRGB preset = defaultPresets[i];
				EEPROM.put(index, preset);
                Wordclock::presets[i] = preset;
                index += sizeof(AlarmTime);
			}
		}
#if ALARM_COUNT > 0
		{
			AlarmTime time = { 0, 0, 0 };
			uint8_t index = ALARM_INDEX;
			for (uint8_t i = 0; i < ALARM_COUNT; i++) {
				EEPROM.put(index, time);
                Wordclock::alarms[i].setTo(time);
                index += sizeof(AlarmTime);
			}
		}
#endif
#else
        Wordclock::currMode = EEPROM.read(MODE_INDEX);
		Wordclock::currPresetIndex = EEPROM.read(COLOR_PRESET_INDEX_INDEX);
		FastLED.setBrightness(EEPROM.read(BRIGHTNESS_INDEX));
        uint8_t index = COLOR_PRESET_INDEX;
        for (uint8_t i = 0; i < COLOR_PRESET_COUNT; i++) {
            CRGB color;
            EEPROM.get(index, color);
            Wordclock::presets[i] = color;
            index += sizeof(CRGB);
        }
#if ALARM_COUNT > 0
        index = ALARM_INDEX;
        for (uint8_t i = 0; i < ALARM_COUNT; i++) {
            AlarmTime time;
            EEPROM.get(index, time);
            Wordclock::alarms[i].setTo(time);
            index += sizeof(AlarmTime);
        }
#endif
#endif
#ifdef RESET_EEPROM_SAFELY
		while (true);
#else

		Wordclock::loadTime();
#if ALARM_COUNT > 0
		Wordclock::loadNextAlarm();
#endif

		// init configuration and LED mode
		Wordclock::modes[Wordclock::currMode]->select();
		Wordclock::repaint();
#endif
	}

	inline void Core::loop()
	{
#ifndef RESET_EEPROM_SAFELY
		//DEBUG_OUT("\nl"); // loop

		// check button lock
		static uint16_t buttonLock = 0;
		static bool buttonsLocked = false;
		if (buttonsLocked && ((uint16_t)(millis() - buttonLock)) >
			BUTTON_LOCK_TIME)
		{
			buttonsLocked = false;
			DEBUG_OUT("ub"); // unlock buttons
		}

		// check buttons
		static bool nextModePush = true, prevModePush = true;
		static bool incModePush = true, decModePush = true;

		if (!buttonsLocked) {
			if (digitalRead(INC_MODE_PIN) != incModePush) {
				incModePush = !incModePush;
				if (incModePush) {
					DEBUG_OUT("imb"); // increment mode button
					buttonLock = millis();
					buttonsLocked = true;
#ifdef STOP_ALARM_ON_PRESS
					Wordclock::setAlarm(false);
#endif
					Wordclock::modes[Wordclock::currMode]->increment(true);
				}
			}
			else if (digitalRead(DEC_MODE_PIN) != decModePush) {
				decModePush = !decModePush;
				if (decModePush) {
					DEBUG_OUT("dmb"); // decrement mode button
					buttonLock = millis();
					buttonsLocked = true;
#ifdef STOP_ALARM_ON_PRESS
					Wordclock::setAlarm(false);
#endif
					Wordclock::modes[Wordclock::currMode]->increment(false);
				}
			}
			else if (digitalRead(NEXT_MODE_PIN) != nextModePush) {
				nextModePush = !nextModePush;
				if (nextModePush) {
					DEBUG_OUT("nmb"); // next mode button
					buttonLock = millis();
					buttonsLocked = true;
#ifdef STOP_ALARM_ON_PRESS
					Wordclock::setAlarm(false);
#endif
					Wordclock::setMode(Wordclock::currMode + 1 < MODE_COUNT ?
						Wordclock::currMode + 1 : 0);
				}
			}
			else if (digitalRead(PREV_MODE_PIN) != prevModePush) {
				prevModePush = !prevModePush;
				if (prevModePush) {
					DEBUG_OUT("pmb"); // previous mode button
					buttonLock = millis();
					buttonsLocked = true;
#ifdef STOP_ALARM_ON_PRESS
					Wordclock::setAlarm(false);
#endif
					if (Wordclock::currMode == 0)
						Wordclock::setMode(MODE_COUNT - 1);
					else
						Wordclock::setMode(Wordclock::currMode - 1);
				}
			}
		}

		if (Wordclock::saveTimeRequest) {
			Wordclock::saveTimeRequest = false;
			Wordclock::saveTime();
		}
		
		// update time
		static bool pulse = false;

#ifdef INTERNAL_PULSE
		static uint16_t timeUpdateThreshold = 0;
		if (((uint16_t)(millis() - timeUpdateThreshold)) > 500)
#else
		if (digitalRead(RTC_PULSE_PIN) != pulse)
#endif
		{
#ifdef INTERNAL_PULSE
			timeUpdateThreshold = millis();
#endif
			pulse = !pulse;
#ifdef SHOW_PULSE
			digitalWrite(LED_BUILTIN, pulse);
#endif
#ifdef ALARM_PULSE
			if (Wordclock::alarm)
				digitalWrite(ALARM_PIN, pulse);
#endif

			if (pulse) {
				Wordclock::repaint();
				if (++Wordclock::times[Seconds] == 60) {
					Wordclock::times[Seconds] = 0;
#ifdef INTERNAL_TIME
					uint8_t timeType = Minutes;
					while (++Wordclock::times[timeType] ==
						Wordclock::getMaximumTime(timeType))
					{
						Wordclock::times[timeType] = 0;
						if (timeType == Hours) {
							if (++Wordclock::times[Weekdays] == 7)
								Wordclock::times[Weekdays] = 0;
							timeType = Days;
						}
						else if (timeType++ == Years) {
							break;
						}
					}
#else
					Wordclock::loadTime();
#endif
#if ALARM_COUNT > 0
					if (Wordclock::currAlarm.minute ==
						Wordclock::times[Minutes] &&
						Wordclock::currAlarm.hour ==
						Wordclock::times[Hours] &&
						Wordclock::currAlarm.weekday ==
						Wordclock::times[Weekdays])
					{
						DEBUG_OUT("al"); // set off alarm
						Wordclock::setAlarm(true);
						Wordclock::loadNextAlarm();
					}
#endif
				}
			}
		}
/*
#ifdef DEBUG
		Serial.print("ct "); // current time
		for (uint8_t i = Years; i != 0; i--) {
			Serial.print(Wordclock::times[i]);
			Serial.print(":");
		}
		Serial.println(Wordclock::times[0]);
#endif
*/

		// check timer
		if (ModeBase::updateTime != 0
			&& ((uint32_t)(millis() - ModeBase::updateThreshold))
			> ModeBase::updateTime)
		{
			ModeBase::updateTime = 0;
			DEBUG_OUT("ue"); // update effect
			Wordclock::modes[Wordclock::currMode]->timer();
		}

		// set LEDs
		if (Wordclock::repaintRequest) {
			Wordclock::repaintRequest = false;
			DEBUG_OUT("rp"); // repaint
			Painter::setColor(Wordclock::getCurrentPreset());
			FastLED.clear();
			Wordclock::modes[Wordclock::currMode]->paint();
			Painter::arrange();
			FastLED.show();
		}
#endif
	}
}

void setup()
{
	Wordclock::Core::setup();
}

void loop()
{
	Wordclock::Core::loop();
}
