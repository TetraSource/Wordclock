

#include <EEPROM.h>
#include "EffectBase.hpp"
#include <FastLED.h>
#include "Marker.hpp"
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

	AlarmTime::AlarmTime()
		: weekday(7), hour(0), minute(0)
	{}

	AlarmTime::AlarmTime(const uint8_t &weekday, const uint8_t &hour, const uint8_t &minute)
		: weekday(weekday), hour(hour), minute(minute)
	{}

	AlarmTime::AlarmTime(const AlarmTime &time)
	{
		setTo(time);
	}

	void AlarmTime::setTo(const AlarmTime &time)
	{
		weekday = time.weekday;
		hour = time.hour;
		minute = time.minute;
	}

	uint8_t Wordclock::times[7] = {};
	AlarmTime Wordclock::nextAlarm = { 7, 0, 0 };

	CRGB Wordclock::currPreset = { 0, 0, 0 };
	uint8_t Wordclock::currPresetIndex = 0;

	uint8_t Wordclock::currMode = 0;

	uint8_t Wordclock::currEffect = 0;

	bool Wordclock::alarm = false;
	bool Wordclock::reshapeRequest = false;
	bool Wordclock::repaintRequest = false;
	bool Wordclock::saveTimeRequest = false;

	Marker* Wordclock::marker = new Marker();
	Painter* Wordclock::painter = new Painter(Wordclock::marker);

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
		reshape();
		return true;
	}

	bool Wordclock::setEffectIndex(const uint8_t &mode)
	{
		if (mode >= EFFECT_COUNT)
			return false;
		if (mode == currEffect)
			return true;

		effects[currEffect]->deselect();
		EffectBase::cancelTimer();
		currEffect = mode;
		EEPROM.write(EFFECT_INDEX, currEffect);
		effects[mode]->select();
		reshape();
		return true;
	}

	uint8_t Wordclock::getEffectIndex()
	{
		return currEffect;
	}

	bool Wordclock::setColorPreset(const uint8_t &index, const CRGB &color)
	{
		if (index >= COLOR_PRESET_COUNT)
			return false;
		if (index == currPresetIndex)
			currPreset.setRGB(color.r, color.g, color.b);
		EEPROM.put(COLOR_PRESET_INDEX + index * sizeof(CRGB), color);
		repaint();
		return true;
	}

	CRGB Wordclock::getColorPreset(const uint8_t &index)
	{
		if (index >= COLOR_PRESET_COUNT)
			return CRGB(CRGB::Black);
		if (index == currPresetIndex)
			return CRGB(currPreset);
		CRGB color(0, 0, 0);
		EEPROM.get(COLOR_PRESET_INDEX + index * sizeof(CRGB), color);
		return color;
	}

	CRGB Wordclock::getCurrentPreset()
	{
		return CRGB(currPreset);
	}

	CRGB Wordclock::getCurrentPreset(const uint8_t &offset)
	{
		return getColorPreset((currPresetIndex + offset) % COLOR_PRESET_COUNT);
	}

	bool Wordclock::setColorPresetIndex(const uint8_t &index)
	{
		if (index >= COLOR_PRESET_COUNT)
			return false;
		if (index == currPresetIndex)
			return true;
		currPreset = getColorPreset(index);
		currPresetIndex = index;
		EEPROM.write(COLOR_PRESET_INDEX_INDEX, currPresetIndex);
		repaint();
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

	void Wordclock::extractNext(AlarmTime newAlarm)
	{
		uint8_t weekdays = newAlarm.weekday;
		for (uint8_t weekday = 0; weekday < 7; weekday++) {
			if (weekdays & (1 << weekday)) {
				newAlarm.weekday = weekday;
				if (nextAlarm.weekday >= 7) {
					nextAlarm.setTo(newAlarm);
					continue;
				}

				int8_t weekdaysNew = newAlarm.weekday;
				int8_t weekdaysOld = nextAlarm.weekday;
				weekdaysNew -= times[Weekdays];
				weekdaysOld -= times[Weekdays];

				if (weekdaysNew < 0)
					weekdaysNew += 7;
				if (weekdaysOld < 0)
					weekdaysOld += 7;

				if (weekdaysNew == 0 && (newAlarm.hour < times[Hours] ||
					newAlarm.hour == times[Hours] && newAlarm.minute <= times[Minutes]))
					weekdaysNew = 7;
				if (weekdaysOld == 0 && (nextAlarm.hour < times[Hours] ||
					nextAlarm.hour == times[Hours] && nextAlarm.minute <= times[Minutes]))
					weekdaysOld = 7;

				if (weekdaysNew < weekdaysOld || (weekdaysNew == weekdaysOld &&
					(newAlarm.hour < nextAlarm.hour || newAlarm.hour == nextAlarm.hour && newAlarm.minute < nextAlarm.minute))) {
					nextAlarm.setTo(newAlarm);
				}
			}
		}
	}

	void Wordclock::loadNextAlarm()
	{
		uint8_t index = ALARM_INDEX;
		for (uint8_t i = 0; i < ALARM_COUNT; i++) {
			AlarmTime t;
			EEPROM.get(index, t);
			extractNext(t);
			index += sizeof(AlarmTime);
		}
	}

	bool Wordclock::addAlarm(AlarmTime time)
	{
		if (time.weekday & 0x80) {
			time.weekday &= 0x7f;
		}
		else if (time.weekday < 7) {
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
		time.minute -= time.minute % 5;
		
		uint8_t index = ALARM_INDEX;
		for (uint8_t i = 0; i < ALARM_COUNT; i++) {
			AlarmTime t;
			EEPROM.get(index, t);
			if (t.hour == time.hour && t.minute == time.minute) {
				extractNext(AlarmTime(time.weekday ^ t.weekday, time.hour, time.minute));
				t.weekday |= time.weekday;
				EEPROM.put(index, t);
				return true;
			}
			index += sizeof(AlarmTime);
		}
		// check first whether you can stack the alarm with other ones before checking for free slots.

		index = ALARM_INDEX;
		for (uint8_t i = 0; i < ALARM_COUNT; i++) {
			AlarmTime t;
			EEPROM.get(index, t);
			if (t.weekday == 0x00) {
				extractNext(time);
				EEPROM.put(index, time);
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
			AlarmTime t;
			EEPROM.get(index, t);
			uint8_t weekdays = t.weekday;
			for (uint8_t weekday = 0; weekday < 7; weekday++) {
				if (weekdays & (1 << weekday)) {
					t.weekday = weekday;
					if ((*checker)(t)) {
						nextAlarmDeleted = nextAlarmDeleted ||
							weekday == nextAlarm.weekday && t.hour == nextAlarm.hour && t.minute == nextAlarm.minute;
						weekdays &= ~(1 << weekday);
						t.weekday = weekdays;
						EEPROM.put(index, t);
					}
				}
			}
			index += sizeof(AlarmTime);
		}
		if (nextAlarmDeleted) {
			nextAlarm.weekday = 7;
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

	void Wordclock::reshape()
	{
		DEBUG_OUT("rsr"); // reshape request
		reshapeRequest = true;
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
		FastLED.addLeds<CHIPSET, LED_DATA_PIN, COLOR_ORDER>(Wordclock::painter->leds, LED_COUNT);
		FastLED.clear();

		for (uint8_t i = 0; i <= Years; i++) {
			Wordclock::times[i] = 0;
		}
		Wordclock::nextAlarm.weekday = 7;

#if defined(RESET_EEPROM) || defined(RESET_EEPROM_SAFELY)
		// save data
		EEPROM.write(MODE_INDEX, 0);
		EEPROM.write(EFFECT_INDEX, 0);
		EEPROM.write(COLOR_PRESET_INDEX_INDEX, 0);
		EEPROM.write(BRIGHTNESS_INDEX, 255);
		{
			CRGB defaultPresets[COLOR_PRESET_COUNT] = { DEFAULT_COLOR_PRESETS };
			for (uint8_t i = 0; i < COLOR_PRESET_COUNT; i++) {
				EEPROM.put(COLOR_PRESET_INDEX + i * sizeof(CRGB), defaultPresets[i]);
			}
		}
		{
			AlarmTime time = { 0, 0, 0 };
			uint8_t index = ALARM_INDEX;
			for (uint8_t i = 0; i < ALARM_COUNT; i++) {
				EEPROM.put(index, time);
				index += sizeof(AlarmTime);
			}
		}
#endif
#if RESET_EEPROM_SAFELY
		while (true) {};
#else
		Wordclock::currMode = EEPROM.read(MODE_INDEX);
		Wordclock::currEffect = EEPROM.read(EFFECT_INDEX);
		Wordclock::currPresetIndex = EEPROM.read(COLOR_PRESET_INDEX_INDEX);
		FastLED.setBrightness(EEPROM.read(BRIGHTNESS_INDEX));
		EEPROM.get(COLOR_PRESET_INDEX + Wordclock::currPresetIndex * sizeof(CRGB), Wordclock::currPreset);

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

		Wordclock::loadTime();
		Wordclock::loadNextAlarm();

		// init configuration and LED mode
		Wordclock::modes[Wordclock::currMode]->select();
		Wordclock::effects[Wordclock::currEffect]->select();
		Wordclock::reshape();
#endif
	}

	inline void Core::loop()
	{
#ifndef RESET_EEPROM_SAFELY
		DEBUG_OUT("\nl"); // loop

		// check button lock
		static uint16_t buttonLock = 0;
		static bool buttonsLocked = false;
		if (buttonsLocked && ((uint16_t)(millis() - buttonLock)) > BUTTON_LOCK_TIME) {
			buttonsLocked = false;
			DEBUG_OUT("ub"); // unlock buttons
		}

		DEBUG_OUT("cb"); // check buttons

		// check buttons
		static bool nextModePush = true, prevModePush = true, incModePush = true, decModePush = true;

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
					if (Wordclock::currMode + 1 == MODE_COUNT)
						Wordclock::setMode(0);
					else
						Wordclock::setMode(Wordclock::currMode + 1);
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

		DEBUG_OUT("ut"); // update time

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
				Wordclock::reshape();
				if (++Wordclock::times[Seconds] == 60) {
					Wordclock::times[Seconds] = 0;
#ifdef INTERNAL_TIME
					uint8_t timeType = Minutes;
					while (++Wordclock::times[timeType] == Wordclock::getMaximumTime(timeType)) {
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
					if (Wordclock::nextAlarm.minute == Wordclock::times[Minutes] &&
						Wordclock::nextAlarm.hour == Wordclock::times[Hours] &&
						Wordclock::nextAlarm.weekday == Wordclock::times[Weekdays]) {
						DEBUG_OUT("al"); // set off alarm
						Wordclock::setAlarm(true);
						Wordclock::loadNextAlarm();
					}
				}
			}
		}
#ifdef DEBUG
		Serial.print("ct "); // current time
		for (uint8_t i = Years; i != 0; i--) {
			Serial.print(Wordclock::times[i]);
			Serial.print(":");
		}
		Serial.println(Wordclock::times[0]);
#endif

		// check timer
		if (EffectBase::updateTime != 0
			&& ((uint32_t)(millis() - EffectBase::updateThreshold)) > EffectBase::updateTime) {
			EffectBase::updateTime = 0;
			DEBUG_OUT("ue"); // update effect
			Wordclock::effects[Wordclock::currEffect]->update();
		}

		// mask LEDs
		if (Wordclock::reshapeRequest) {
			Wordclock::reshapeRequest = false;
			DEBUG_OUT("rs"); // reshape
			for (uint8_t i = 0; i < LED_COUNT; i++) {
				Wordclock::marker->mask[i] = false;
			}
			Wordclock::modes[Wordclock::currMode]->shape(Wordclock::marker);
			Wordclock::repaint();
		}

		// set LEDs
		if (Wordclock::repaintRequest) {
			Wordclock::repaintRequest = false;
			DEBUG_OUT("rp"); // repaint
			Wordclock::painter->setForeground(CRGB::Black);
			Wordclock::painter->setBackground(CRGB::Black);
			FastLED.clear();
			Wordclock::effects[Wordclock::currEffect]->paint(Wordclock::painter);
			Wordclock::painter->arrange();
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