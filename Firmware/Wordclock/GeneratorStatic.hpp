#pragma once

#include "GeneratorBase.hpp"

namespace Wordclock
{
	/// always generates the same color.
	/// @tparam value0 - the red amount or hue of the color.
	/// @tparam value1 - the green amount or saturation of the color.
	/// @tparam value2 - the blue amount or saturation of the color.
	/// @tparam colorType - the color space of the color.
	template <uint8_t value0, uint8_t value1, uint8_t value2, ColorTypes colorType = RGB>
	class GeneratorStatic : public GeneratorBase
	{
	public:
		CRGB nextColor();

		ColorTypes getColorType();
	};

	template <uint8_t value0, uint8_t value1, uint8_t value2, ColorTypes colorType>
	CRGB GeneratorStatic<value0, value1, value2, colorType>::nextColor()
	{
		return CRGB(value0, value1, value2);
	}

	template <uint8_t value0, uint8_t value1, uint8_t value2, ColorTypes colorType>
	ColorTypes GeneratorStatic<value0, value1, value2, colorType>::getColorType()
	{
		return colorType;
	}
}