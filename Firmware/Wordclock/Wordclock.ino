
#include <EEPROM.h>
#include <FastLED.h>
#include "ModeBase.hpp"
#include "Painter.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"
#include "Timer.hpp"
#ifdef WDT
#include <avr/wdt.h>
#endif
#ifndef INTERNAL_TIME
#include <Wire.h>
#endif

namespace Wordclock
{
	// CORE DEFINITION

	class Core {
	private:
		static uint32_t currMillis;
#ifndef INTERNAL_TIME
		static bool saveTimeRequest;
		static void saveTime();
		static void loadTime();
#endif
		inline static void checkButtons();
		inline static void updateTime();
		inline static void checkTimers();
		inline static void updateLEDs();
	
	public:
		inline static void setup();
		inline static void loop();

	friend class Wordclock;
	friend uint32_t internalMillis();
	};

	// WORDCLOCK API IMPLEMENTATION

	const uint8_t maxTime[7] = {
		60,
		60,
		24,
		7,
		31,
		12,
		255,
	};

	uint8_t Wordclock::times[7] = {};

	uint8_t Wordclock::currMode = 0;

	CRGB Wordclock::presets[COLOR_PRESET_COUNT] = {};
	uint8_t Wordclock::currPresetIndex = 0;

	TimerHeap<TIMER_COUNT_TYPE> Wordclock::timers =
		TimerHeap<TIMER_COUNT_TYPE>();
	bool Wordclock::repaintRequest = false;

	bool Wordclock::setMode(const uint8_t &index)
	{
		if (index >= modeCount)
			return false;
		if (index == currMode)
			return true;

		modes[currMode]->deselect();
		currMode = index;
		EEPROM.write(MODE_INDEX, currMode);
		modes[currMode]->select();
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

	bool Wordclock::setTime(const TimeTypes &timeType, uint8_t time)
	{
		if (timeType > Years)
			return false;

		uint8_t maxTime = getMaximumTime(timeType);
		if (time >= maxTime)
			time = maxTime;
		if (time == times[timeType])
			return true;

		times[timeType] = time;
#ifndef INTERNAL_TIME
		Core::saveTimeRequest = true;
#endif
		return true;
	}

	uint8_t Wordclock::getDaysOfMonth()
	{
		if (times[5] == 1)
			return times[6] % 4 == 0 ? 29 : 28;
		else if (times[5] < 6)
			return 31 - times[5] % 2;
		else
			return 30 + times[5] % 2;
	}

	uint8_t Wordclock::getMaximumTime(const TimeTypes &timeType)
	{
		if (timeType >= Years)
			return 0;
		if (timeType == Days)
			return getDaysOfMonth();

		return maxTime[timeType];
	}

	uint32_t Wordclock::getSeconds(const TimeTypes &timeType)
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

	uint32_t Wordclock::getUnitSeconds(const TimeTypes &timeType)
	{
		switch(timeType)
		{
		case Years:
			return times[Years] % 4 == 0 ? 182145024 : 181647360;
		case Months:
			return getDaysOfMonth() * 497664;
		case Days:
		case Weekdays:
			return 86400;
		case Hours:
			return 3600;
		case Minutes:
			return 60;
		case Seconds:
			return 1;
		default:
			return 0;
		}
	}

	void Wordclock::setBrightness(const uint8_t &brightness)
	{
		if (brightness != FastLED.getBrightness()) {
			FastLED.setBrightness(brightness);
			EEPROM.put(BRIGHTNESS_INDEX, brightness);
			repaint();
		}
	}

	// WORDCLOCK API END //
}

#define IMPORT_MODES
#include "Config.hpp"

namespace Wordclock
{
	// CORE IMPLEMENTATION //

	uint32_t Core::currMillis = 0;

#ifndef INTERNAL_TIME
	bool Core::saveTimeRequest = false;

	void Core::saveTime()
	{
		// Write to RTC
		Wire.beginTransmission(0x68);
		Wire.write(0x0);
		uint8_t time;
		for (uint8_t i = 0; i < 7; i++) {
			Wire.write(((Wordclock::times[i] / 10) << 4) +
				(Wordclock::times[i] % 10));
		}
		Wire.write(0x0);
		Wire.endTransmission();
	}

	void Core::loadTime()
	{
		// Read from RTC
		Wire.beginTransmission(0x68);
		Wire.write(0x0);
		Wire.endTransmission();
		Wire.requestFrom(0x68, 7);
		for (uint8_t n = 0; n < 7; n++) {
			uint8_t val = Wire.read();
			Wordclock::times[n] = (((val >> 4) * 10) + (val % 16));
		}
		Wordclock::times[Seconds] &= 0x7f;
		Wordclock::times[Hours] &= 0x3f;
#ifdef DEBUG
		Serial.print("rt "); // read time
		for (uint8_t n = 0; n <= Years; n++) {
			Serial.print(Wordclock::times[n]);
			Serial.print(' ');
		}
		Serial.println();
#endif
	}
#endif

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
		pinMode(INC_BUTTON_PIN, INPUT_PULLUP);
		pinMode(DEC_BUTTON_PIN, INPUT_PULLUP);
		pinMode(NEXT_MODE_BUTTON_PIN, INPUT_PULLUP);
		pinMode(PREV_MODE_BUTTON_PIN, INPUT_PULLUP);
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

		for (uint8_t i = 0; i < 7; i++) {
			Wordclock::times[i] = 0;
		}

#ifdef RESET_EEPROM
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
				index += sizeof(CRGB);
			}
		}
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
#endif
#ifdef JUST_INITIALIZE
		while (true);
#else
#ifndef INTERNAL_TIME
		loadTime();
#endif

		// init configuration and LED mode
		Wordclock::modes[Wordclock::currMode]->select();
		Wordclock::repaint();
#endif
	}

	struct ButtonState
	{
		uint16_t buttonLock : 11;
		bool buttonsLocked : 1;
		bool nextModePush : 1;
		bool prevModePush : 1;
		bool incModePush : 1;
		bool decModePush : 1;
	};

	inline void Core::checkButtons()
	{
		// check button lock
		static ButtonState bs = {0, false, true, true, true, true};
		if (bs.buttonsLocked && ((uint16_t)(millis() - bs.buttonLock)) >
			BUTTON_LOCK_TIME)
		{
			DEBUG_OUT("ub"); // unlock buttons
			bs.buttonsLocked = false;
		}

		// check buttons
		if (!bs.buttonsLocked) {
			bool incButton =
				digitalRead(INC_BUTTON_PIN) != bs.incModePush;
			if (incButton ||
				digitalRead(DEC_BUTTON_PIN) != bs.decModePush) {
				if (incButton)
					bs.incModePush = !bs.incModePush;
				else
					bs.decModePush = !bs.decModePush;
				if (bs.incModePush && bs.decModePush) {
					DEBUG_OUT("cmb"); // change mode buttons
					bs.buttonLock = millis();
					bs.buttonsLocked = true;
					Wordclock::modes[Wordclock::currMode]->
						actionButton(incButton);
				}
			}
			else {
				incButton =
					digitalRead(NEXT_MODE_BUTTON_PIN) != bs.nextModePush;
				if (incButton ||
					digitalRead(PREV_MODE_BUTTON_PIN) != bs.prevModePush) {
					if (incButton)
						bs.nextModePush = !bs.nextModePush;
					else
						bs.prevModePush = !bs.prevModePush;
					if (bs.nextModePush && bs.prevModePush) {
						DEBUG_OUT("mb"); // mode buttons
						bs.buttonLock = millis();
						bs.buttonsLocked = true;
						Wordclock::modes[Wordclock::currMode]->
							modeButton(incButton);
					}
				}
			}
		}
	}

	inline void Core::updateTime()
	{
#ifndef INTERNAL_TIME
		if (saveTimeRequest) {
			saveTimeRequest = false;
			saveTime();
		}
#endif

		// update time
		static bool pulse = true;

#ifdef INTERNAL_PULSE
		static uint16_t timeUpdateThreshold = 0;
		if (((uint16_t)(millis() - timeUpdateThreshold)) >= 500)
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
			if (pulse) {
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
					loadTime();
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
	}

	uint32_t internalMillis()
	{
		return Core::currMillis;
	}

	inline void Core::updateLEDs()
	{
		if (Wordclock::repaintRequest) {
			Wordclock::repaintRequest = false;
			DEBUG_OUT("rp"); // repaint
			Painter::setColor(Wordclock::getCurrentPreset());
			FastLED.clear();
			Wordclock::modes[Wordclock::currMode]->paint();
			FastLED.show();
		}
	}

	inline void Core::loop()
	{
#ifndef JUST_INITIALIZE
		DEBUG_OUT("\nl"); // loop
		checkButtons();
		updateTime();
		currMillis = millis();
		Wordclock::timers.update();
		updateLEDs();
#endif
	}
}

void setup()
{
	Serial.begin(9600);
	Wordclock::Core::setup();
}

void loop()
{
	Wordclock::Core::loop();
}