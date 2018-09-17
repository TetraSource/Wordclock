#pragma once

#include <Arduino.h>
#include "Config.hpp"

#ifdef VERTICAL_STRIPES
#define WIDTH (LED_COUNT / ROW_LENGTH)
#define HEIGHT ROW_LENGTH
#else
#define WIDTH ROW_LENGTH
#define HEIGHT (LED_COUNT / ROW_LENGTH)
#endif
#define DISPLAY_WIDTH WIDTH
#define DISPLAY_HEIGHT HEIGHT

#define MAX_LENGTH (WIDTH >= HEIGHT ? WIDTH : HEIGHT)

namespace Wordclock
{
	/// the base class for the @c Marker and @c Painter class.
	/// allows to mark LEDs by @c DisplayTool::note.
	/// The purpose of this is defined by a child class of this one.
	class DisplayTool
	{
	protected:
		/// is called with the indexes of all LEDs that are supposed to be marked.
		/// @param index - the index of the LED to be marked.
		virtual inline void note(const uint8_t &index) = 0;

		/// returns whether an LED is masked or not.
		/// @param index - the index of the LED.
		/// @returns whether the LED at the index is marked or not.
		virtual inline bool query(const uint8_t &index) = 0;

	public:
		/// the width of the display.
		static const uint8_t width;

		/// the height of the display.
		static const uint8_t height;

		/// the length of the longest side of the display.
		static const uint8_t maxLength;

		/// marks all LEDs of the display.
		void markAll();

		/// marks the LED at the given spot.
		/// @param x - x coordinate of the LED (most left is 0).
		/// @param y - y coordinate of the LED (topmost is 0).
		void mark(const uint8_t &x, const uint8_t &y);

		/// marks all LEDs in the given area.
		/// @param x - x coordinate of the LED (most left is 0).
		/// @param y - y coordinate of the LED (topmost is 0).
		/// @param w - width of the area.
		/// @param h - height of the area.
		void mark(const uint8_t &x, const uint8_t &y, const uint8_t &w, const uint8_t &h = 1);

		/// marks a LED that lights the given letter. LEDs without neighbors are preferred.
		/// @param letter - the letter to be marked. Pass ' ' (space) to light no letter.
		/// @returns whether the could be marked or not. This is not the case if there is no (free) LED with the given letter.
		bool mark(const char &letter);
	};
}