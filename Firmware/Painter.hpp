#pragma once

#include "DisplayTool.hpp"
#include <FastLED.h>
#include "Marker.hpp"

namespace Wordclock
{
	/// provides the means for setting the color of all LEDs in the matrix.
	class Painter : public DisplayTool
	{
		friend class Wordclock;
		friend class Core;

	protected:
		static CRGB buffer[LED_COUNT];

		CRGB leds[LED_COUNT];
		CRGB foreground;
		CRGB background;
		Marker* marker;
		
		void arrange();
		void note(const uint8_t &index);
		bool query(const uint8_t &index);

	public:
		/// initializes a new painter.
		/// @param marker - the marker that determines where to use to background and where to use the foreground color.
		Painter(Marker* marker);

		/// returns whether the pixel would be marked with the foreground or background color.
		/// @param x - the x coordinate of the pixel.
		/// @param y - the y coordinate of the pixel.
		/// @returns whether the pixel would be marked with the foreground color or not.
		bool isForeground(const uint8_t &x, const uint8_t &y);

		/// sets the color for the marked LEDs.
		/// @param foreground - the new color
		void setForeground(const CRGB &foreground);

		/// returns the color for the marked LEDs.
		/// @returns the color
		CRGB getForeground();

		/// sets the color for the not marked LEDs.
		/// @params background - the new color
		void setBackground(const CRGB &background);

		/// returns the color for the not marked LEDs.
		/// @returns the color
		CRGB getBackground();

		/// returns true when both colors are equal.
		/// @param c0 - the first color
		/// @param c1 - the second color
		/// @returns whether they are identical or not.
		static bool areIdentical(CRGB c0, CRGB c1);

		/// returns true when the foreground and the background color are equal.
		/// @returns whether they are identical or not.
		bool areIdentical();
	};
}