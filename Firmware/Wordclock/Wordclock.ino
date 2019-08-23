#include "EepromAccess.hpp"
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

#if MODE_COUNT <= 0
#error Mode count needs to be 1 at least.
#endif
#if LAYER_COUNT <= 0
#error Layer count needs to be 1 at least
#endif

namespace Wordclock
{
	// CORE DEFINITION

	class Core {
	private:
		static EepromAccess<uint8_t> brightnessSlot;
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

	const EepromArray<uint8_t, LAYER_COUNT> Wordclock::currModes =
		EepromArray<uint8_t, LAYER_COUNT>();

	const EepromArray<CRGB, COLOR_PRESET_COUNT> Wordclock::presets =
		EepromArray<CRGB, COLOR_PRESET_COUNT>();
	const EepromVariable<uint8_t> Wordclock::currPresetIndex =
		EepromVariable<uint8_t>();

	TimerHeap<TIMER_COUNT_TYPE> Wordclock::timers =
		TimerHeap<TIMER_COUNT_TYPE>();
	bool Wordclock::repaintRequest = false;

	bool Wordclock::setMode(const uint8_t &layer, const uint8_t &index,
		const bool &activateTransition)
	{
		if (index >= MODE_COUNT || layer >= LAYER_COUNT)
			return false;
		if (index == currModes.get(layer))
			return true;

		modes[currModes.get(layer)]->deselect();

		currModes.set(layer, index);
		if (activateTransition) {
			ModeBase::inTransition = true;
			startTimer(modes[index],
				ModeBase::transitionTime, ModeBase::transitionChannel);
		}
		modes[index]->select();
		repaint();
		return true;
	}

	uint8_t Wordclock::getNextMode(const uint8_t &layer, const bool &inc,
		const uint8_t &lower, uint8_t upper)
	{
		if (layer >= LAYER_COUNT)
			return 0;
		if (upper > MODE_COUNT)
			upper = MODE_COUNT;
		upper -= lower;
		uint8_t newMode = currModes.get(layer);
		uint8_t i = 0;
		do {
			bool valid = true;
			newMode = Utilities::changeValue(newMode - lower, upper, inc) + lower;
			for (uint8_t j = 0; valid && j < LAYER_COUNT; j++)
				valid = j == layer || currModes.get(j) != newMode;
			if (valid)
				return newMode;
		} while (++i != MODE_COUNT);
		return currModes.get(layer);
	}

	bool Wordclock::setColorPreset(const uint8_t &index, const CRGB &color)
	{
		if (index >= COLOR_PRESET_COUNT)
			return false;
		presets.set(index, color);
		repaint();
		return true;
	}

	CRGB Wordclock::getColorPreset(const uint8_t &index)
	{
		if (index >= COLOR_PRESET_COUNT)
			return CRGB(CRGB::Black);
		return presets.get(index);
	}

	bool Wordclock::setColorPresetIndex(const uint8_t &index)
	{
		if (index >= COLOR_PRESET_COUNT)
			return false;
		if (index != currPresetIndex.get()) {
			currPresetIndex.set(index);
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
			Core::brightnessSlot.set(brightness);
			repaint();
		}
	}

	// WORDCLOCK API END //
}

#define IMPORT_MODES
#ifdef JUST_INITIALIZE
#define MODE_COUNT 0
#define LAYER_COUNT 0
const Wordclock::ModeBase *Wordclock::Wordclock::modes[MODE_COUNT] = {};
#else
#include "Config.hpp"
#endif
#undef IMPORT_MODES

namespace Wordclock
{
	// CORE IMPLEMENTATION //

	EepromAccess<uint8_t> Core::brightnessSlot = EepromAccess<uint8_t>();
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

		// initialize variables
		for (uint8_t i = 0; i < 7; i++) {
			Wordclock::times[i] = 0;
		}

		Wordclock::currPresetIndex.setDefault(0);

		uint8_t defaultModes[LAYER_COUNT] = { DEFAULT_MODES };
		CRGB defaultPresets[COLOR_PRESET_COUNT] = { DEFAULT_COLOR_PRESETS };
		for (uint8_t layer = 0; layer < LAYER_COUNT; layer++)
			Wordclock::currModes.setDefault(layer, defaultModes[layer]);
		for (uint8_t i = 0; i < COLOR_PRESET_COUNT; i++)
			Wordclock::presets.setDefault(i, defaultPresets[i]);

		uint8_t brightness = 255;
		brightnessSlot.setDefault(brightness);
		FastLED.setBrightness(brightness);

#ifdef JUST_INITIALIZE
		while (true);
#else
#ifndef INTERNAL_TIME
		loadTime();
#endif

		// init configuration and LED mode
		for (uint8_t i = 0; i < LAYER_COUNT; i++)
			Wordclock::modes[Wordclock::currModes.get(i)]->select();
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
					Wordclock::modes[Wordclock::currModes.get(0)]->
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
						Wordclock::modes[Wordclock::currModes.get(0)]->
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
			FastLED.clear();
			for (uint8_t i = LAYER_COUNT; i != 0;) {
				Wordclock::modes[Wordclock::currModes.get(--i)]->paint();
			}
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
	Wordclock::Core::setup();
}

void loop()
{
	Wordclock::Core::loop();
}