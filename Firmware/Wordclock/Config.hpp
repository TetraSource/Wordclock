
///////////////////////////// HARDWARE SETTINGS ////////////////////////////////

// PINS //

// Don't use the SDA (data line) or SCL (clock line)
// but for connecting it to the RTC.

// Digital button input pin for picking modes.
#define NEXT_MODE_BUTTON_PIN 9
#define PREV_MODE_BUTTON_PIN 8
// Digital button input pin for triggering actions of the current focused mode.
#define INC_BUTTON_PIN 11
#define DEC_BUTTON_PIN 10
// Digital LED stripe connection.
#define LED_DATA_PIN 5
// Digital input pin for RTC pulse.
#define RTC_PULSE_PIN 4
// A free analog input pin.
#define FREE_ANALOG_PIN 0

// DISPLAY //

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

// Macro for calculating the virtual LED position
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

// DEBUGGING //

// Baud rate for serial debug output
#define BAUD_RATE 9600

// Outputs debug data if given
//#define DEBUG

// Outputs all marked and painted LEDs when given
//#define DEBUG_DISPLAY

// Utilizes the build in LED to indicate the time pulse if this setting exists
//#define SHOW_PULSE

// RTC //

// Lets the Arduino simulate the RTC pulse if this setting exists.
#define INTERNAL_PULSE

// If this setting is set, the clock of the Arduino is used instead of the RTC.
// Becomes inaccurate easily.
//#define INTERNAL_TIME

// TIMERS //

// The type of the variable used to save the (maximal) count of running timers.
// Thus this value determines the maximal count of running timers but also
// increases the amount of RAM used by the timers.
// The following types are possible:
//   uint8_t  => 1 byte  => max. 2^8-1 = 255 timers
//   uint16_t => 2 bytes => max. 2^16-1 = 65535 timers
//   uint32_t => 4 bytes => max. 2^32-1 = 4294967295 timers
//   uint64_t => 8 bytes => max. 2^64-1 timers
#define TIMER_COUNT_TYPE uint8_t

// Specifies the maximal count of milliseconds that may pass between a timer
// elapses and the time it is triggered. This is necessary as the timers are
// not checked constantly. If a timer elapses and timerHeap.update is called
// at least TRIGGER_ZONE milliseconds afterward, the timer can still be
// triggered. However, it also lessons the maximal interval to
// 2^32 - TRIGGER_ZONE.
#define TRIGGER_ZONE 8192

// EEPROM //

// Enable those settings if you upload the program the first time or if you
// changed certain settings such as the modes.

// Resets the EEPROM whenever the Wordclock is powered using the
// default values.
#define RESET_EEPROM

// Initialising the EEPROM might require more RAM than the normal operational
// Wordclock status. When you think, reseting the EEPROM might crash the
// Wordclock activate this setting. It just allows the Wordclock to get
// initialized and disables all its modes so that they don't take the RAM.
// The Wordclock is not going to work if this is set of course.
//#define JUST_INITIALIZE

// MISCELLANEOUS //

// the time a button is locked before it can be used again in milliseconds.
// This value must not exceed 2047.
#define BUTTON_LOCK_TIME 500

// Just shows the current configuration mode if this setting exists.
#define SHOW_MODE

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

#ifndef JUST_INITIALIZE
// protects this section from being loaded into wrong code sections.
#ifdef IMPORT_MODES

// default mode
#include "ModeWordclock.hpp"

// configuration modes
#include "ModeBrightness.hpp"
#include "ModeColorPreset.hpp"
#include "ModeHSV.hpp"
#include "ModeRearrangeColorPreset.hpp"
#include "ModeRGB.hpp"
#include "ModeTime.hpp"

// effect modes
#include "ModeColorChangerTime.hpp"
#include "ModeColorChangerTimer.hpp"
#include "ModeFiller.hpp"
#include "ModePixelRain.hpp"
#include "ModeGlowing.hpp"
#include "ModeTimeSlice.hpp"
#include "ModeWaves.hpp"

// color generators
#include "GeneratorAlternating.hpp"
#include "GeneratorColorPreset.hpp"
#include "GeneratorGradient.hpp"
#include "GeneratorHSVAdapter.hpp"
#include "GeneratorRandom.hpp"
#include "GeneratorRGBAdapter.hpp"
#include "GeneratorStatic.hpp"
#include "GeneratorUnion.hpp"

// selectors
#include "SelectorRandom.hpp"
#include "SelectorSuccessive.hpp"
#include "SelectorTime.hpp"

#define PresetGenerator GeneratorColorPreset<SelectorSuccessive<0, COLOR_PRESET_COUNT, 1, 0>>

// ALWAYS KEEP THIS NUMBER IN SYNC WITH THE COUNT OF MODES AND
// RESET THE EEPROM IF YOU CHANGE IT!
namespace Wordclock
{
	const uint8_t Wordclock::modeCount = 23;
	const ModeBase *Wordclock::modes[Wordclock::modeCount] = {
		new ModeWordclock(),
		new ModeTime<Weekdays>(),
		new ModeTime<Hours>(),
		new ModeTime<Minutes>(),
		new ModeTime<Seconds>(),
		new ModeColorPreset(),
		new ModeRearrangeColorPreset(),
		new ModeBrightness<5>(),
		new ModeRGB<Red, 20>(),
		new ModeRGB<Green, 20>(),
		new ModeRGB<Blue, 20>(),
		new ModeHSV<Hue, 20>(),
		new ModeHSV<Saturation, 20>(),
		new ModeHSV<Value, 20>(),

		// uses the first five colors to show the time in five minute
		// intervals.
		new ModeColorChangerTime<GeneratorColorPreset<SelectorTime<Minutes, 0, 6, 5>>, 5, Minutes>(),

		// uses a timer and a gradient to generate a fader.
		new ModeColorChangerTimer<GeneratorGradient<PresetGenerator, 150>, 250>(),

		// turns on pixels around activated ones to let them appear glowing.
		new ModeGlowing<2>(),

		// shows a time slice in the background to show the time in five
		// minute intervals.
		new ModeTimeSlice<GeneratorStatic<255, 255, 0>, Minutes, 5>(),

		// fills the display from the bottom to the top to show the advancing
		// of every minute.
		// equal to "new ModeFiller<Bottom, Minutes, 1>"
		new ModeFiller<GeneratorStatic<255, 100, 0>, Bottom, Seconds, 60>(),

		// lets pixels fly quickly from the top, bottom and right side of the
		// display to the opposite ones.
		new ModePixelRain<GeneratorColorPreset<SelectorRandom<0, COLOR_PRESET_COUNT>>,
			DIR_ITEM(Top, DIR_ITEM(Bottom, DIR_ITEM(Right, 0))), 150, 1, 5, 0, 12>(),

		// The Matrix!
		new ModePixelRain<GeneratorStatic<0, 255, 0>, DIR_ITEM(Top, 0), 150, 3, 5, 30, 40>(),

		// Tunnel effect with all colors!
		new ModeWaves<GeneratorRandom<0, 255, 0, 255, 0, 255>, Center, 500>(),

		// creates a gradient from the left side to the right one using
		// vibrant, random generated colors.
		new ModeWaves<GeneratorGradient<GeneratorRGBAdapter<GeneratorRandom
			<0, 255, 191, 255, 65, 255>>, 220>, Left, 1000>(),
	};
}
#undef IMPORT_MODES
#endif

#else

#ifdef IMPORT_MODES
#include "ModeBase.hpp"
namespace Worclock {
	const uint8_t Wordclock::modeCount = 0;
	ModeBase *Wordclock::modes[Wordclock::modeCount] = {};
}
#undef IMPORT_MODES
#endif

#endif