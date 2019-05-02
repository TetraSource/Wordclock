#pragma once

#include "Config.hpp"
#include <FastLED.h>

#ifdef VERTICAL_STRIPES
#define WIDTH (LED_COUNT / ROW_LENGTH)
#define HEIGHT ROW_LENGTH
#else
#define WIDTH ROW_LENGTH
#define HEIGHT (LED_COUNT / ROW_LENGTH)
#endif

#define MAX_LENGTH (WIDTH >= HEIGHT ? WIDTH : HEIGHT)

namespace Wordclock
{
	/// provides the means for setting the color of all LEDs in the matrix.
	/// Painting methods are suppossed to be called from the paint method of
	/// the ModeBase class or a child class of it.
	class Painter
	{
		friend class Wordclock;
		friend class Core;

	protected:
		static CRGB buffer[LED_COUNT];

		static CRGB leds[LED_COUNT];
		static CRGB curr;
		
		static void arrange();

	public:
		/// the width of the display.
		static const uint8_t width;

		/// the height of the display.
		static const uint8_t height;

		/// the length of the longest side of the display.
		static const uint8_t maxLength;

		/// returns the color for the next painting operations.
		/// @returns the current color
		static CRGB getColor();

		/// returns true if the current color is every color except black.
		/// @returns whether the current color is not equal (0, 0, 0)
		static bool hasColor();

		/// sets the color for the next painting operations.
		/// @param - the new color
		static void setColor(const CRGB &color);

		/// Returns the color of the pixel at the specified position.
		/// The top left pixel has the coordinate (0, 0).
		/// @param x - the x coordinate of the pixel
		/// @param y - the y coordinate of the pixel
		/// @returns the color of the pixel
		static CRGB getPixel(const uint8_t &x, const uint8_t &y);

		/// Paints the entire display with the current color.
		static void paintAll();

		/// Paints the specified pixel with the current color.
		/// The position is the virtual position of the LED on the stripe.
		/// You can obtain it by using the POINT macro.
		/// @param x - the x coordinate of the pixel
		/// @param y - the y coordinate of the pixel
		static void paint(const uint8_t &position);

		/// Paints the specified pixel with the current color.
		/// The top left pixel has the coordinate (0, 0).
		/// @param x - the x coordinate of the pixel
		/// @param y - the y coordinate of the pixel
		static void paint(const uint8_t &x, const uint8_t &y);

		/// Paints the specified area with the current color.
		/// The top left pixel has the coordinate (0, 0).
		/// The minimum size is (1, 1).
		/// @param x - the x coordinate of the pixel
		/// @param y - the y coordinate of the pixel
		/// @param w - the width of the area
		/// @param h - the height of the area
		static void paint(
			const uint8_t &x, const uint8_t &y,
			const uint8_t &w, const uint8_t &h = 1);

		/// returns true when both colors are (almost) equal.
		/// @param c0 - the first color
		/// @param c1 - the second color
		/// @returns whether they are identical or not.
		static bool areIdentical(CRGB c0, CRGB c1);
	};
}