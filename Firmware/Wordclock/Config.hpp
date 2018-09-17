
								  // configuration file //

//////////////////////////////////// HARDWARE SETTINGS ///////////////////////////////////

// PINS

// Don't use the SDA (data line) or SCL (clock line) but for connecting it to the RTC.
#define NEXT_MODE_PIN 8   // Digital button input pin for picking the next mode.
#define PREV_MODE_PIN 9   // Digital button input pin for picking the previous mode.
#define INC_MODE_PIN 10   // Digital button input pin for incrementing the current mode.
#define DEC_MODE_PIN 11   // Digital button input pin for decrementing the current mode.
#define LED_DATA_PIN 5    // Digital LED stripe connection.
#define RTC_PULSE_PIN 4   // Digital input pin for RTC pulse.
#define ALARM_PIN 6       // The digital output pin for the alarm. Is turned on and off once per second when the alarm is active.
#define FREE_ANALOG_PIN 0 // A free analog input pin.

// DISPLAY

#define CHIPSET  WS2812B   // the LED stripe type
#define COLOR_ORDER GRB    // the color order or the LED stripe

#define LED_COUNT    110   // The sum of all LED stripes that form the display
#define ROW_LENGTH    11   // The length of a LED stripe
//#define VERTICAL_STRIPES   // Should exists if the LED stripes are laid vertically. This mode hasn't been tested yet.
//#define UPSIDE_DOWN        // Turns the displayed by 180 degree if this setting exists.
//#define MIRROR             // Mirrors the display horizontally if this setting exists.
#define CHANGE_DIRECTION   // This setting should exists when the LED stripes change their direction in each new row so that they run conversely always.

#ifdef IMPORT_LETTERS // protects this section from being loaded into wrong code sections
#undef IMPORT_LETTERS
namespace Wordclock
{
	// Order of the LEDs on the display. This must always be arranged horizontally (like here)
	// even if the stripes are installed vertically. Please replace non-ASCII letters by \0.
	const char letters[LED_COUNT] = {
		'E', 'S', 'K', 'I', 'S', 'T', 'L', 'F', '\0', 'N', 'F',
		'Z', 'E', 'H', 'N', 'Z', 'W', 'A', 'N', 'Z', 'I', 'G',
		'D', 'R', 'E', 'I', 'V', 'I', 'E', 'R', 'T', 'E', 'L',
		'T', 'G', 'N', 'A', 'C', 'H', 'V', 'O', 'R', 'J', 'M',
		'H', 'A', 'L', 'B', 'X', 'Z', 'W', '\0', 'L', 'F', 'P',
		'Z', 'W', 'E', 'I', 'N', 'S', 'I', 'E', 'B', 'E', 'N',
		'K', 'D', 'R', 'E', 'I', 'R', 'H', 'F', '\0', 'N', 'F',
		'E', 'L', 'F', 'N', 'E', 'U', 'N', 'V', 'I', 'E', 'R',
		'W', 'A', 'C', 'H', 'T', 'Z', 'E', 'H', 'N', 'R', 'S',
		'B', 'S', 'E', 'C', 'H', 'S', 'F', 'M', 'U', 'H', 'R',
	};
}
#endif

////////////////////////////////// SOFTWARE SETTINGS /////////////////////////////////////

// DEBUGGING

#define BAUD_RATE 9600  // Baud rate for serial debug output
//#define DEBUG           // Outputs debug data if given
//#define DEBUG_DISPLAY   // Outputs all marked and painted LEDs when given
//#define SHOW_PULSE      // Utilizes the build in LED to indicate the time pulse if this setting exists

// RTC

#define INTERNAL_PULSE // Lets the Arduino simulate the RTC pulse if this setting exists
//#define INTERNAL_TIME  // Lets the Arduino simulate the time incrementation if this setting exists. Becomes inaccurate easily.

// EEPROM

// Enable this if you upload the program the first time or if you change certain settings such as the count of something.
//#define RESET_EEPROM        // Resets the EEPROM whenever the Wordclock is powered using the default values.
//#define RESET_EEPROM_SAFELY // More robust type of RESET_EEPROM. Note that it the Wordclock cannot operate when this mode is active.

// MISCELLANEOUS

#define BUTTON_LOCK_TIME 250 // the time a button is locked before it can be used again in milliseconds.
#define SHOW_MODE            // Just shows the current configuration mode if this setting exists.
#define LANGUAGE 1           // the language of the time shown by the words.

/*
languages
0 - English
1 - German
*/

// ALARMS

#define ALARM_COUNT 10       // the maximal count of times for all alarms.
#define STOP_ALARM_ON_PRESS  // stops the alarm when any button is pressed if this setting exists.
#define ALARM_PULSE          // uses a pulse instead of a static high signal for the alarm if this setting exists.

// COLOR PRESETS //

// Reset the EEPROM whenever you change these.
// Sets the default colors and count for the color presets.
// Colors can either be specified by their RGB color (CRGB(r, g, b)) or by their HSV color (CRGB(CHSV(h, s, v))) or by their html color CRGB(CRGB::Name).
// The available HTML colors can be found at https://github.com/FastLED/FastLED/wiki/Pixel-reference#predefined-colors-list
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

// MODES AND EFFECTS //

#ifndef RESET_EEPROM_SAFELY
#ifdef IMPORT_MODES // protects this section from being loaded into wrong code sections.

#include "ModeDefault.hpp"
#include "ModeAlarm.hpp"
#include "ModeBrightness.hpp"
#include "ModeColorPreset.hpp"
#include "ModeEffect.hpp"
#include "ModeHSV.hpp"
#include "ModeListAlarms.hpp"
#include "ModeRearrangeColorPreset.hpp"
#include "ModeRGB.hpp"
#include "ModeTime.hpp"

#include "EffectDefault.hpp"
#include "EffectColorChangerTime.hpp"
#include "EffectColorChangerTimer.hpp"
#include "EffectFiller.hpp"
#include "EffectFlyingPixels.hpp"
#include "EffectGlowing.hpp"
#include "EffectTimeSlice.hpp"
#include "EffectWaves.hpp"

#include "GeneratorColorPreset.hpp"
#include "GeneratorGradient.hpp"
#include "GeneratorRandom.hpp"
#include "GeneratorStatic.hpp"

namespace Wordclock {
#endif

#define MODE_COUNT 20 // ALWAYS KEEP THIS NUMBER IN SYNC WITH THE COUNT OF MODES AND RESET THE EEPROM IF YOU CHANGE IT!
#ifdef IMPORT_MODES
	ModeBase* Wordclock::modes[MODE_COUNT] = {
		new ModeDefault<' '>(), // <-- don't move this mode to another index
		new ModeTime<'W', Weekdays>(),
		new ModeTime<'H', Hours>(),
		new ModeTime<'M', Minutes>(),
		new ModeTime<'S', Seconds>(),
		new ModeColorPreset<'P'>(),
		new ModeRearrangeColorPreset<'J'>(),
		new ModeBrightness<'X', 5>(),
		new ModeRGB<'R', Red, 20>(),
		new ModeRGB<'G', Green, 20>(),
		new ModeRGB<'B', Blue, 20>(),
		new ModeHSV<'H', Hue, 20>(),
		new ModeHSV<'S', Saturation, 20>(),
		new ModeHSV<'V', Value, 20>(),
		new ModeAlarm<'W', AlarmWeekday>(),
		new ModeAlarm<'H', AlarmHour>(),
		new ModeAlarm<'M', AlarmMinute>(),
		new ModeAlarm<'A', SetAlarm>(),
		new ModeListAlarms<'V'>(),
		new ModeEffect<'E'>(),
	};
#endif

#define EFFECT_COUNT 1 // ALWAYS KEEP THIS NUMBER IN SYNC WITH THE COUNT OF EFFECTS AND RESET THE EEPROM IF YOU CHANGE IT!
#ifdef IMPORT_MODES
	EffectBase* Wordclock::effects[EFFECT_COUNT] = {
		new EffectDefault(), // <-- don't move this mode to another index
		// picks every hour the next color preset.
		//new EffectColorChangerTime<Hours>(new GeneratorColorPreset<ChooseNext>()),
		// uses the first five colors to show the time in five minute intervals.
		//new EffectColorChangerTime<Minutes>(new GeneratorColorPreset<ChooseByTime, 0, 4, Minutes>()),
		// uses a timer and a gradient to generate a fader.
		//new EffectColorChangerTimer<250>(new GeneratorGradient<2>(new GeneratorColorPreset<ChooseNext>())),
		// fills the display from the bottom to the top to show the advancing of every minute.
		//new EffectFiller<Bottom, Seconds, 60>(), // equal to "new EffectFiller<Bottom, Minutes, 1>"
		// lets pixels fly quickly from the top, bottom and right side of the display to the opposite ones.
		//new EffectFlyingPixels<(1 << Top) | (1 << Bottom) | (1 << Right), 200, 1, 5, 0, 12>(new GeneratorColorPreset<ChooseRandom>()),
		// the matrix!
		//new EffectFlyingPixels<(1 << Top), 250, 3, 5, 20, 20>(new GeneratorStatic<0, 255, 0, RGB>()),
		// turns on pixels around activated ones to let them appear glowing.
		//new EffectGlowing<1>(),
		// shows a time slice in the background to show the time in five minute intervals.
		//new EffectTimeSlice<Minutes, 5>(),
		// tunnel effect with all colors!
		//new EffectWaves<Center, 500>(new GeneratorRandom<0, 255, 0, 255, 0, 255, RGB>()),
		// creates a gradient from the left side to the right one using vibrant, random generated colors.
		//new EffectWaves<Left, 1000>(new GeneratorGradient<25>(new GeneratorRandom<0, 255, 191, 255, 65, 255, HSV>())),
	};
}
#undef IMPORT_MODES
#endif
#else
#define MODE_COUNT 0
#define EFFECT_COUNT 0
#ifdef IMPORT_MODES
#include "ModeBase.hpp"
#include "EffectBase.hpp"

namespace Wordclock
{
	ModeBase* Wordclock::modes[MODE_COUNT] = {};
	EffectBase* Wordclock::effects[EFFECT_COUNT] = {};
}
#undef IMPORT_MODES
#endif
#endif