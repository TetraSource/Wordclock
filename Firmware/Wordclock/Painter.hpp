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
		static uint8_t *replace;
		static bool replacing;
		
		static void arrange();
		static void paintPixel(const uint8_t &position);

	public:
		/// the width of the display.
		static const uint8_t width;

		/// the height of the display.
		static const uint8_t height;

		/// the length of the longest side of the display.
		static const uint8_t maxLength;

		/// the length of the shortest side of the display.
		static const uint8_t minLength;

		/// returns the color for the next painting operations.
		/// @returns the current color
		inline static CRGB getColor();

		/// returns true if the current color is every color except black.
		/// @returns whether the current color is not equal (0, 0, 0)
		inline static bool hasColor();

		/// sets the color for the next painting operations.
		/// @param - the new color
		inline static void setColor(const CRGB &color);

		/// Returns the color of the pixel at the specified position.
		/// The top left pixel has the coordinate (0, 0).
		/// @param x - the x coordinate of the pixel
		/// @param y - the y coordinate of the pixel
		/// @returns the color of the pixel
		static CRGB getPixel(const uint8_t &x, const uint8_t &y);

		/// Paints the entire display with the current color.
		static void paintAll();

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

		/// Prepares the paint methods for masking. All paint methods called
		/// after this function are not going to paint but mark a pixel.
		/// The current color has no effect on the masking. Once the
		/// masking finished, you have to call setReplacing.
		static void prepareReplacing();

		/// Before calling the method you must have called prepareReplacing.
		/// Call this method with true first, then paint the screen. Just
		/// the previous masked pixels get painted thereby.
		/// Finally, call this method with false, to reset the Painter to
		/// its normal state.
		static void setReplacing(const bool &);
	};

	CRGB Painter::getColor()
	{
		return curr;
	}

	bool Painter::hasColor()
	{
		return curr.r != 0 || curr.g != 0 || curr.b != 0;
	}

	void Painter::setColor(const CRGB &color)
	{
		curr = color;
	}
}