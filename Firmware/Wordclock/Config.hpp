
///////////////////////////// HARDWARE SETTINGS ////////////////////////////////

// PINS

// Don't use the SDA (data line) or SCL (clock line)
// but for connecting it to the RTC.

// Digital button input pin for picking the next mode.
#define NEXT_MODE_PIN 8
// Digital button input pin for picking the previous mode.
#define PREV_MODE_PIN 9
// Digital button input pin for incrementing the current mode.
#define INC_MODE_PIN 10
// Digital button input pin for decrementing the current mode.
#define DEC_MODE_PIN 11
// Digital LED stripe connection.
#define LED_DATA_PIN 5
// Digital input pin for RTC pulse.
#define RTC_PULSE_PIN 4
// The digital output pin for the alarm.
// Is turned on and off once per second when the alarm is active.
#define ALARM_PIN 6
// A free analog input pin.
#define FREE_ANALOG_PIN 0

// DISPLAY

#define CHIPSET  WS2812B   // the LED stripe type
#define COLOR_ORDER GRB    // the color order or the LED stripe

// the language of the time shown by the words.
// languages
//  0 - English (not supported jet)
//  1 - German
#define LANGUAGE 1           

// The sum of all LED stripes that form the display
#define LED_COUNT    110
// The length of a horizontal row that text is displayed alongside.
#define ROW_LENGTH    11
// The length of a vertical row is the LED_COUNT divided by the ROW_LENGTH

// Macro for calculating the virutal led position
// of a point with the coord (x, y). Note that (0, 0)
// specifies the top left point.
#define POINT(x, y) (x + y * ROW_LENGTH)
#define NO_POINT (LED_COUNT+1)

// protect this section from being loaded into wrong code sections
#ifdef IMPORT_LAYOUT
#undef IMPORT_LAYOUT
namespace Wordclock
{
	// The array specifies the arrangement of the LEDs. It maps each virtual
	// position of a LED onto its physical position. The virtual position is
	// calculated by the macro POINT above.
	const uint8_t arrangement[LED_COUNT] = {
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11,
		22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
		43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33,
		44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54,
		65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55,
		66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76,
		87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77,
		88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98,
		109, 108, 107, 106, 105, 104, 103, 102, 101, 100, 99,
	};
}
#endif

/////////////////////////////// SOFTWARE SETTINGS //////////////////////////////

// DEBUGGING

// Baud rate for serial debug output
#define BAUD_RATE 9600
// Outputs debug data if given
//#define DEBUG
// Outputs all marked and painted LEDs when given
//#define DEBUG_DISPLAY
// Utilizes the build in LED to indicate the time pulse if this setting exists
//#define SHOW_PULSE

// RTC

// Lets the Arduino simulate the RTC pulse if this setting exists.
#define INTERNAL_PULSE
// Lets the Arduino simulate the time incrementation if this setting exists.
// Becomes inaccurate easily.
//#define INTERNAL_TIME

// EEPROM

// Enable those settings if you upload the program the first time or if you
// changed certain settings such as the modes.

// Resets the EEPROM whenever the Wordclock is powered using the
// default values.
//#define RESET_EEPROM
// More robust type of RESET_EEPROM.
// Note that the Wordclock appears to be dead when this mode is enabled.
//#define RESET_EEPROM_SAFELY

// MISCELLANEOUS

// the time a button is locked before it can be used again in milliseconds.
#define BUTTON_LOCK_TIME 250
// Just shows the current configuration mode if this setting exists.
#define SHOW_MODE

// ALARMS

// the maximal count of times for all alarms.
#define ALARM_COUNT 0
// stops the alarm when any button is pressed if this setting exists.
#define STOP_ALARM_ON_PRESS
// uses a pulse instead of a static high signal for the alarm
// if this setting exists.
#define ALARM_PULSE

// COLOR PRESETS //

// Reset the EEPROM whenever you change these.
// Sets the default colors and count for the color presets.
// Colors can either be specified by their RGB color (CRGB(r, g, b)) or by
// their HSV color (CRGB(CHSV(h, s, v))) or by their html color
// CRGB(CRGB::Name). The available HTML colors can be found at
// https://github.com/FastLED/FastLED/wiki/Pixel-reference#predefined-colors-list
#define COLOR_PRESET_COUNT 12
#define DEFAULT_COLOR_PRESETS \
CRGB(CRGB::White), \
CRGB(CRGB::Yellow), \
CRGB(CRGB::DarkOrange), \
CRGB(CRGB::Red), \
CRGB(CRGB::DeepPink), \
CRGB(CRGB::DarkViolet), \
CRGB(CRGB::DarkBlue), \
CRGB(CRGB::DeepSkyBlue), \
CRGB(CRGB::SeaGreen), \
CRGB(CRGB::DarkCyan), \
CRGB(CRGB::Lime), \
CRGB(CRGB::DarkGreen), \

// MODES //

#ifndef RESET_EEPROM_SAFELY
// protects this section from being loaded into wrong code sections.
#ifdef IMPORT_MODES

// default mode
#include "ModeDefault.hpp"

// configuration modes
#include "ModeAlarm.hpp"
#include "ModeBrightness.hpp"
#include "ModeColorPreset.hpp"
#include "ModeHSV.hpp"
#include "ModeListAlarms.hpp"
#include "ModeRearrangeColorPreset.hpp"
#include "ModeRGB.hpp"
#include "ModeTime.hpp"

// effect modes
#include "ModeColorChangerTime.hpp"
#include "ModeColorChangerTimer.hpp"
#include "ModeFiller.hpp"
#include "ModeFlyingPixels.hpp"
#include "ModeGlowing.hpp"
#include "ModeTimeSlice.hpp"
#include "ModeWaves.hpp"

// color generators
#include "GeneratorColorPreset.hpp"
#include "GeneratorGradient.hpp"
#include "GeneratorRandom.hpp"
#include "GeneratorStatic.hpp"
#endif

// ALWAYS KEEP THIS NUMBER IN SYNC WITH THE COUNT OF MODES AND
// RESET THE EEPROM IF YOU CHANGE IT!
#if ALARM_COUNT > 0
#define ALARM_MODES_ 5
#else
#define ALARM_MODES_ 0
#endif
#define MODE_COUNT 3 + ALARM_MODES_ // 23
#ifdef IMPORT_MODES
namespace Wordclock
{
	ModeBase* Wordclock::modes[MODE_COUNT] = {
		new ModeDefault<NO_POINT>(), // Don't move this mode to another index.
		/*
		new ModeTime<POINT(5, 1), Weekdays>(), // letter is W
		new ModeTime<POINT(2, 1), Hours>(), // letter is H
		new ModeTime<POINT(7, 10), Minutes>(), // letter is M
		new ModeTime<POINT(1, 0), Seconds>(), // letter is S
		new ModeColorPreset<POINT(2, 9)>(), // letter is C
		new ModeRearrangeColorPreset<POINT(9, 3)>(), // letter is J
		new ModeBrightness<POINT(8, 1), 5>(), // letter is Z
		new ModeRGB<POINT(0, 10), Red, 20>(), // letter is R
		new ModeRGB<POINT(10, 1), Green, 20>(), // letter is G
		new ModeRGB<POINT(10, 10), Blue, 20>(), // letter is B
		new ModeHSV<POINT(5, 5), Hue, 20>(), // letter is H
		new ModeHSV<POINT(5, 3), Saturation, 20>(), // letter is S
		new ModeHSV<POINT(4, 2), Value, 20>(), // letter is V
		*/
#if ALARM_COUNT > 0
		new ModeAlarm<POINT(0, 8), AlarmWeekday>(), // letter is W
		new ModeAlarm<POINT(4, 10), AlarmHour>(), // letter is H
		new ModeAlarm<POINT(7, 10), AlarmMinute>(), // letter is M
		new ModeAlarm<POINT(1, 8), SetAlarm>(), // letter is A
		new ModeListAlarms<POINT(1, 7)>(), // letter is L
#endif
		// uses the first five colors to show the time in five minute
		// intervals.
		new ModeColorChangerTime<Minutes>(new GeneratorColorPreset<ChooseByTime, 0, 4, Minutes>()),

		// uses a timer and a gradient to generate a fader.
		new ModeColorChangerTimer<250>(new GeneratorGradient<2>(new GeneratorColorPreset<ChooseNext>())),

		// fills the display from the bottom to the top to show the advancing
		// of every minute.
		// equal to "new ModeFiller<Bottom, Minutes, 1>"
		new ModeFiller<Bottom, Seconds, 60>(),

		// lets pixels fly quickly from the top, bottom and right side of the
		// display to the opposite ones.
		new ModeFlyingPixels<DIR_ITEM(Top, DIR_ITEM(Bottom, DIR_ITEM(Right, 0))), 200, 1, 5, 0, 12>(new GeneratorColorPreset<ChooseRandom>()),

		// The matrix!
		new ModeFlyingPixels<DIR_ITEM(Top, 0), 150, 3, 5, 30, 40>(new GeneratorStatic<0, 255, 0, RGB>()),

		// turns on pixels around activated ones to let them appear glowing.
		new ModeGlowing<2>(),

		// shows a time slice in the background to show the time in five
		// minute intervals.
		new ModeTimeSlice<Minutes, 5>(),

		// Tunnel effect with all colors!
		new ModeWaves<Center, 500>(new GeneratorRandom<0, 255, 0, 255, 0, 255, RGB>()),

		// creates a gradient from the left side to the right one using
		// vibrant, random generated colors.
		new ModeWaves<Left, 1000>(new GeneratorGradient<25>(new GeneratorRandom<0, 255, 191, 255, 65, 255, HSV>())),
	};
}
#undef IMPORT_MODES
#endif

#else

#define MODE_COUNT 0
#ifdef IMPORT_MODES
#include "ModeBase.hpp"
Wordclock::ModeBase* Wordclock::Wordclock::modes[MODE_COUNT] = {};
#undef IMPORT_MODES
#endif

#endif
