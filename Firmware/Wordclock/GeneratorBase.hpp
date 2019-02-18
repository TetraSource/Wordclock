#pragma once

#include <Arduino.h>
#include <FastLED.h>

namespace Wordclock
{
	/// the color space a color is defined in.
	enum ColorTypes
	{
		RGB, /// color in RGB color space.
		HSV, /// color in HSV color space.
	};

	/// the base class for all color generators.
	class GeneratorBase
	{
	public:
		// Returns a color that was selected by the color generator.
		// The color space can be RGB or HSV. The return type of the function is CRGB always though.
		// @return the chosen color
		virtual CRGB nextColor() = 0;

		// Returns whether the class generates colors in HSV color space or in RGB color space.
		// @return whether the class generates colors in HSV color space or not.
		virtual ColorTypes getColorType();

		// Returns the color that was selected by the color generator and
		// converts it to RGB color space if it was in HSV color space.
		// @return the chosen color in RGB color space
		CRGB nextRGBColor();

		// Returns the color that was selected by the color generator and
		// converts it to HSV color space if it was in HSV color space.
		// @return the chosen color in HSV color space
		CHSV nextHSVColor();
	};
}